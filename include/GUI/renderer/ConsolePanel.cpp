#include "GUI/renderer/ConsolePanel.hpp"
#include "GUI/GuiConfig.hpp"
#include <algorithm>

using namespace GuiConfig;

ConsolePanel::ConsolePanel(sf::RenderWindow& w, const sf::Font& f, const sf::Font& b)
    : win(w), font(f), bold(b), dice(f), scrollOffset(0) {}

void ConsolePanel::update(float dt) {
    dice.update(dt);
    cursorTimer += dt;
    if (cursorTimer > 0.55f) { cursorTimer = 0.f; showCursor = !showCursor; }
}

void ConsolePanel::draw(const IGuiState& state) {
    drawHeader(state);
    drawOutput(state);
    drawDice(state);
    drawInputField();
}

void ConsolePanel::drawHeader(const IGuiState& state) {
    float px = (float)CON_X, pw = (float)CON_W;
    sf::RectangleShape hdr({pw,(float)CON_HEADER_H});
    hdr.setPosition(px, 0.f); hdr.setFillColor({38,44,60}); win.draw(hdr);
    sf::RectangleShape sep({pw, 2.f});
    sep.setPosition(px,(float)CON_HEADER_H-1.f); sep.setFillColor({80,90,120}); win.draw(sep);

    std::string ts = "Turn " + std::to_string(state.currentTurn());
    if (state.maxTurn() > 0) ts += " / " + std::to_string(state.maxTurn());
    sf::Text tt; tt.setFont(bold); tt.setString(ts);
    tt.setCharacterSize(13); tt.setFillColor({160,175,210});
    tt.setPosition(px+CON_PAD, 8.f); win.draw(tt);

    const auto& pls = state.players();
    float rowH = 18.f, startY = 28.f;
    for (int i = 0; i < (int)pls.size(); ++i) {
        const auto& p = pls[i];
        float ry = startY + i * rowH;
        if (p.isCurrent) {
            sf::RectangleShape hl({pw-2.f*CON_PAD,rowH-2.f});
            hl.setPosition(px+CON_PAD,ry); hl.setFillColor({55,65,95}); win.draw(hl);
        }
        sf::Color tc = (p.state=="BANKRUPT") ? sf::Color{100,100,100}
            : (i==0?sf::Color{220,55,55}:i==1?sf::Color{40,100,220}:
               i==2?sf::Color{30,170,60}:sf::Color{230,180,0});
        sf::CircleShape tok(6.f); tok.setFillColor(tc);
        tok.setPosition(px+CON_PAD,ry+2.f); win.draw(tok);

        sf::Text nm; nm.setFont(p.isCurrent?bold:font); nm.setString(p.username);
        nm.setCharacterSize(12);
        nm.setFillColor(p.isCurrent?sf::Color{240,240,255}:sf::Color{170,175,195});
        nm.setPosition(px+CON_PAD+16.f,ry+1.f); win.draw(nm);

        sf::Text money; money.setFont(font);
        money.setString("M"+std::to_string(p.money)); money.setCharacterSize(11);
        money.setFillColor(p.state=="BANKRUPT"?sf::Color{150,80,80}:sf::Color{130,210,130});
        auto mb=money.getLocalBounds();
        money.setPosition(px+pw-CON_PAD-mb.width-mb.left,ry+2.f); win.draw(money);

        if (p.state=="JAILED"||p.state=="BANKRUPT") {
            sf::Text stag; stag.setFont(bold);
            stag.setString(p.state=="JAILED"?"[JAILED]":"[BANKRUPT]");
            stag.setCharacterSize(9);
            stag.setFillColor(p.state=="JAILED"?sf::Color{230,180,40}:sf::Color{210,80,80});
            auto sb=stag.getLocalBounds();
            stag.setPosition(px+pw-CON_PAD-sb.width-sb.left-55.f,ry+3.f); win.draw(stag);
        }
    }
}

void ConsolePanel::drawOutput(const IGuiState& state) {
    float px=(float)CON_X, pw=(float)CON_W;
    int top=CON_HEADER_H, bot=WINDOW_H-CON_INPUT_H-DICE_SZ-60;
    float aH=(float)(bot-top);
    sf::RectangleShape bg({pw,aH}); bg.setPosition(px,(float)top);
    bg.setFillColor({20,23,34}); win.draw(bg);

    const auto& lines=state.consoleLines();
    int maxV=(int)(aH/CON_LINE_H), total=(int)lines.size();
    scrollOffset=std::max(0,std::min(scrollOffset,std::max(0,total-maxV)));
    int startL=std::max(0,total-maxV-scrollOffset), endL=std::min(total,startL+maxV);

    sf::View clip=sf::View({px+pw/2.f,(float)top+aH/2.f},{pw,aH});
    clip.setViewport({px/(float)WINDOW_W,(float)top/(float)WINDOW_H,
                      pw/(float)WINDOW_W,aH/(float)WINDOW_H});
    win.setView(clip);

    for (int i=startL; i<endL; ++i) {
        const std::string& line=lines[i];
        sf::Color c={195,200,215};
        if (line.find("===")!=std::string::npos)                           c={220,180,60};
        else if (line.find("BANGKRUT")!=std::string::npos||
                 line.find("ERROR")!=std::string::npos)                    c={220,80,80};
        else if (line.find("menerima")!=std::string::npos||
                 line.find("Menerima")!=std::string::npos)                 c={80,220,120};
        else if (line.find("Membayar")!=std::string::npos||
                 line.find("membayar")!=std::string::npos)                 c={220,140,80};
        else if (!line.empty()&&line[0]=='>')                              c={130,200,255};
        else if (line.find("Turn")!=std::string::npos)                    c={180,160,230};
        sf::Text t; t.setFont(font);
        std::string disp=line.size()>80?line.substr(0,80):line;
        t.setString(disp); t.setCharacterSize(CON_FONT_SZ); t.setFillColor(c);
        t.setPosition(px+CON_PAD,(float)top+(i-startL)*(float)CON_LINE_H+3.f);
        win.draw(t);
    }
    win.setView(win.getDefaultView());
}

void ConsolePanel::drawDice(const IGuiState& state) {
    float diceX=(float)CON_X+(float)CON_W/2.f-((float)DICE_SZ*2.f+12.f)/2.f;
    float diceY=(float)WINDOW_H-(float)CON_INPUT_H-(float)DICE_SZ-50.f;
    dice.draw(win,diceX,diceY,state.dice());
}

void ConsolePanel::drawInputField() {
    float px=(float)CON_X, pw=(float)CON_W;
    float iy=(float)(WINDOW_H-CON_INPUT_H);
    sf::RectangleShape sep({pw,1.f}); sep.setPosition(px,iy);
    sep.setFillColor({60,70,100}); win.draw(sep);
    sf::RectangleShape bg({pw,(float)CON_INPUT_H}); bg.setPosition(px,iy);
    bg.setFillColor({28,32,48}); win.draw(bg);
    sf::Text prompt; prompt.setFont(bold); prompt.setString(">");
    prompt.setCharacterSize(14); prompt.setFillColor({80,180,255});
    prompt.setPosition(px+CON_PAD,iy+(float)CON_INPUT_H/2.f-9.f); win.draw(prompt);
    sf::Text inp; inp.setFont(font);
    inp.setString(input+(showCursor?"|":" "));
    inp.setCharacterSize(14); inp.setFillColor({230,235,255});
    inp.setPosition(px+CON_PAD+18.f,iy+(float)CON_INPUT_H/2.f-9.f); win.draw(inp);
}

void ConsolePanel::handleEvent(const sf::Event& e, IGuiState& state) {
    if (e.type==sf::Event::TextEntered) {
        if (e.text.unicode==8)      { if (!input.empty()) input.pop_back(); }
        else if (e.text.unicode==13){ if (!input.empty()){ state.submitCommand(input); input.clear(); scrollOffset=0; }}
        else if (e.text.unicode>=32&&e.text.unicode<127){ if(input.size()<80) input+=(char)e.text.unicode; }
    }
    if (e.type==sf::Event::KeyPressed) {
        if (e.key.code==sf::Keyboard::Up)       scrollOffset+=2;
        if (e.key.code==sf::Keyboard::Down)     scrollOffset=std::max(0,scrollOffset-2);
        if (e.key.code==sf::Keyboard::PageUp)   scrollOffset+=10;
        if (e.key.code==sf::Keyboard::PageDown) scrollOffset=std::max(0,scrollOffset-10);
    }
    if (e.type==sf::Event::MouseWheelScrolled) {
        if (e.mouseWheelScroll.delta>0) scrollOffset+=3;
        else scrollOffset=std::max(0,scrollOffset-3);
    }
}