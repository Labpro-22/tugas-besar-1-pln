#pragma once
namespace GuiConfig {
    constexpr int WINDOW_W = 1400;
    constexpr int WINDOW_H = 850;
    constexpr int FPS = 60;

    // Board panel
    constexpr int BOARD_PANEL_W = 810;
    constexpr int TILE_SIZE = 70;
    constexpr int GRID_N = 11;
    constexpr int BOARD_PX = TILE_SIZE * GRID_N;
    constexpr int BOARD_OX = 20;
    constexpr int BOARD_OY = 40;

    // Tile styling
    constexpr int COLOR_BAND = 14;
    constexpr float BORDER_THICK = 1.0f;

    // Player pieces
    constexpr int PIECE_R = 9;
    constexpr int PIECE_BORDER  = 2;

    // Animation
    constexpr float ANIM_HOPS_PER_SEC = 8.0f;
    constexpr float JUMP_HEIGHT = 24.0f;

    // Dice 
    constexpr int DICE_SZ = 56;
    constexpr int DICE_DOT_R = 5;
    constexpr float DICE_SPIN_DUR = 0.55f; 

    // Console 
    constexpr int CON_X = BOARD_PANEL_W;
    constexpr int CON_W = WINDOW_W - BOARD_PANEL_W;
    constexpr int CON_HEADER_H = 100;
    constexpr int CON_INPUT_H = 48;
    constexpr int CON_FONT_SZ = 13;
    constexpr int CON_LINE_H = 18;
    constexpr int CON_PAD = 12;

    // Fonts
    constexpr const char* FONT_REG  = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    constexpr const char* FONT_BOLD = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf";
    constexpr const char* FONT_MONO = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
}
