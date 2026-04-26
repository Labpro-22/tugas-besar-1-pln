# Makefile for C++ OOP Project

CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I include
LDFLAGS  := # -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR     := src
OBJ_DIR     := build
BIN_DIR     := bin
DATA_DIR    := data
CONFIG_DIR  := config

ifeq ($(OS),Windows_NT)
TARGET := $(BIN_DIR)/game.exe
else
TARGET := $(BIN_DIR)/game
endif

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRCS := $(call rwildcard,$(SRC_DIR)/,*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
define make-dir
if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
endef

define remove-dir
if exist "$(subst /,\,$1)" rmdir /s /q "$(subst /,\,$1)"
endef
else
define make-dir
mkdir -p $1
endef

define remove-dir
rm -rf $1
endef
endif

all: directories $(TARGET)

directories:
	@$(call make-dir,$(OBJ_DIR))
	@$(call make-dir,$(BIN_DIR))
	@$(call make-dir,$(DATA_DIR))
	@$(call make-dir,$(CONFIG_DIR))

$(TARGET): $(OBJS)
	$(file >$(BIN_DIR)/objects.rsp,$^)
	$(CXX) $(CXXFLAGS) @$(BIN_DIR)/objects.rsp -o $@ $(LDFLAGS)
	@echo "Build successful! Executable is at $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(call make-dir,$(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	$(TARGET)

clean:
	@$(call remove-dir,$(OBJ_DIR))
	@$(call remove-dir,$(BIN_DIR))
	@echo "Cleaned up $(OBJ_DIR) and $(BIN_DIR)"

rebuild: clean all

.PHONY: all clean rebuild run directories
