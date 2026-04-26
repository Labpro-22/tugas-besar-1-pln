#include "GUI/style/TileStyle.hpp"

TileStyle::TileStyle() {
    // Color group → band color
    group["COKLAT"]    = {139, 90,  43};
    group["BIRU_MUDA"] = { 90,190,240};
    group["MERAH_MUDA"]= {230,130,180};
    group["ORANGE"]    = {240,140, 30};
    group["MERAH"]     = {210, 40, 40};
    group["KUNING"]    = {240,210, 30};
    group["HIJAU"]     = { 40,170, 70};
    group["BIRU_TUA"]  = { 30, 60,200};
    group["ABU_ABU"]   = {160,160,160};
    group["DEFAULT"]   = {200,200,200};

    // Tile type → background color
    bg["GO"]          = {220,235,200};
    bg["JAIL"]        = {220,210,165};
    bg["FREE_PARKING"]= {230,225,195};
    bg["GO_TO_JAIL"]  = {230,180,150};
    bg["INCOME_TAX"]  = {205,205,225};
    bg["LUXURY_TAX"]  = {205,205,225};
    bg["FESTIVAL"]    = {200,230,200};
    bg["COMMUNITY"]   = {215,225,240};
    bg["CHANCE"]      = {240,225,195};
    bg["STREET"]      = {245,242,228};
    bg["RAILROAD"]    = {230,230,230};
    bg["UTILITY"]     = {230,230,230};
    bg["DEFAULT"]     = {235,232,215};

    // Player piece fill colors
    piece = { sf::Color{220,55,55},  sf::Color{40,100,220},
              sf::Color{30,170,60},  sf::Color{230,180,0} };

    // Player piece rim/outline colors
    rim   = { sf::Color{160,20,20},  sf::Color{20,60,160},
              sf::Color{15,110,35},  sf::Color{160,120,0} };
}

sf::Color TileStyle::groupColor(const std::string& colorGroup) const {
    auto it = group.find(colorGroup);
    return it != group.end() ? it->second : group.at("DEFAULT");
}

sf::Color TileStyle::tileBg(const std::string& tileType) const {
    auto it = bg.find(tileType);
    return it != bg.end() ? it->second : bg.at("DEFAULT");
}

sf::Color TileStyle::pieceColor(int idx) const {
    if (idx < 0 || idx >= 4) return {180,180,180};
    return piece[idx];
}

sf::Color TileStyle::pieceRim(int idx) const {
    if (idx < 0 || idx >= 4) return {100,100,100};
    return rim[idx];
}

std::string TileStyle::symbol(const std::string& tileType) const {
    if (tileType == "GO")           return "GO";
    if (tileType == "JAIL")         return "PENJARA";
    if (tileType == "FREE_PARKING") return "PARKIR";
    if (tileType == "GO_TO_JAIL")   return "KE PENJARA";
    if (tileType == "INCOME_TAX")   return "PPH";
    if (tileType == "LUXURY_TAX")   return "PBM";
    if (tileType == "FESTIVAL")     return "FESTIVAL";
    if (tileType == "COMMUNITY")    return "DANA UMUM";
    if (tileType == "CHANCE")       return "KESEMPATAN";
    if (tileType == "RAILROAD")     return "STASIUN";
    if (tileType == "UTILITY")      return "UTILITAS";
    return "";
}

bool TileStyle::hasColorBand(const std::string& tileType) const {
    return tileType == "STREET";
}

sf::Color TileStyle::contrastText(const sf::Color& bg) {
    // Luminance formula: use white on dark, dark on light
    float lum = 0.299f*(float)bg.r + 0.587f*(float)bg.g + 0.114f*(float)bg.b;
    return lum < 128.f ? sf::Color{240,240,240} : sf::Color{30,30,30};
}