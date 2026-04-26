CXX      := g++
STD      := -std=c++17
WARN     := -Wall -Wextra -Wno-unused-parameter
INC      := -I include

# Source directories
SRC_DIR := src
OBJ_DIR := build

# ─── Shared model/util/view sources (used by both CLI and GUI) ───────────────
SHARED_SRCS := $(shell find $(SRC_DIR) -name '*.cpp' \
    ! -name 'main.cpp' \
    ! -name 'main_gui.cpp' \
    ! -path '$(SRC_DIR)/GuiApp.cpp')

# CLI = shared + main.cpp  (no SFML)
CLI_SRCS  := $(SHARED_SRCS) $(SRC_DIR)/main.cpp
CLI_OBJS  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/cli/%.o, $(CLI_SRCS))
CLI_BIN   := bin/game

# GUI = shared + main_gui.cpp + GuiApp.cpp  (needs SFML)
GUI_SRCS  := $(SHARED_SRCS) $(SRC_DIR)/main_gui.cpp $(SRC_DIR)/GuiApp.cpp
GUI_OBJS  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/gui/%.o, $(GUI_SRCS))
GUI_BIN   := bin/game_gui

# GUI cpp files that live inside include/GUI/  (SFML renderer, animation, style, etc.)
GUI_EXTRA_SRCS := $(shell find include/GUI -name '*.cpp')
GUI_EXTRA_OBJS := $(patsubst include/GUI/%.cpp, $(OBJ_DIR)/gui_extra/%.o, $(GUI_EXTRA_SRCS))

SFML_LIBS  := -lsfml-graphics -lsfml-window -lsfml-system
THREAD_LIB := -pthread

# ─── Default: build CLI only ──────────────────────────────────────────────────
.PHONY: all cli gui clean

all: cli

cli: $(CLI_BIN)
	@echo "Build CLI sukses → $(CLI_BIN)"

gui: $(GUI_BIN)
	@echo "Build GUI sukses → $(GUI_BIN)"

# ─── CLI link ────────────────────────────────────────────────────────────────
$(CLI_BIN): $(CLI_OBJS)
	@mkdir -p bin
	$(CXX) $(STD) $^ -o $@ $(THREAD_LIB)

# CLI compile rule
$(OBJ_DIR)/cli/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARN) $(INC) -c $< -o $@

# ─── GUI link ─────────────────────────────────────────────────────────────────
$(GUI_BIN): $(GUI_OBJS) $(GUI_EXTRA_OBJS)
	@mkdir -p bin
	$(CXX) $(STD) $^ -o $@ $(SFML_LIBS) $(THREAD_LIB)

# GUI compile rule (src/) — needs include/GUI for GuiApp.hpp etc.
$(OBJ_DIR)/gui/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARN) $(INC) -c $< -o $@

# GUI extra compile rule (include/GUI/ renderer + animation + style cpp files)
# -I include/GUI so renderer files find each other with "renderer/Dice.hpp" style paths
$(OBJ_DIR)/gui_extra/%.o: include/GUI/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARN) $(INC) -I include/GUI -c $< -o $@

# ─── Clean ───────────────────────────────────────────────────────────────────
clean:
	rm -rf $(OBJ_DIR) bin