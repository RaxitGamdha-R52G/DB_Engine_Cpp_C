# Makefile

# ============================================================
# Compiler Configuration
# ============================================================

CC  := gcc
CXX := g++

BUILD	?= debug
VERBOSE	?= 0

MAKEFLAGS += --no-print-directory

# ============================================================
# Phony Targets
# ============================================================

# Default targets
.PHONY: all clean run-debug run-release gdb debug release help

# Example targets
.PHONY: hello run-hello

# Tests targets
.PHONY: test

# ============================================================
# Colors
# ============================================================

RESET	:= \033[0m
BOLD    := \033[1m

RED     := \033[31m
GREEN   := \033[32m
YELLOW  := \033[33m
BLUE    := \033[34m
MAGENTA := \033[35m
CYAN    := \033[36m
GRAY    := \033[38;5;244m

# ============================================================
# Tree Style Prefixes
# ============================================================

TREE_MID  := $(GRAY)├─$(RESET)
TREE_LAST := $(GRAY)└─$(RESET)

# ============================================================
# Verbose / Quiet Mode
# ============================================================

ifeq ($(VERBOSE),1)
	Q := 
else
	Q := @
endif

# ============================================================
# Compiler Flags
# ============================================================

CFLAGS   := -std=c23 -Wall -Wextra -Wpedantic -Iinclude -MMD -MP
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Iinclude -MMD -MP

ifeq ($(BUILD),debug)
	CFLAGS	 += -g3 -O0
	CXXFLAGS += -g3 -O0 -fno-omit-frame-pointer

	BUILD_DIR := build/debug
else
	CFLAGS	 += -O2
	CXXFLAGS += -O2

	BUILD_DIR := build/release
endif

# ============================================================
# Project Structure
# ============================================================

SRC_DIR := src

BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj

# FINAL OUTPUT
TARGET := $(BIN_DIR)/app.exe

# SRC FILES
SRCS_CPP := $(shell find $(SRC_DIR) -type f -name "*.cpp")
SRCS_C   := $(shell find $(SRC_DIR) -type f -name "*.c")

# OBJECT FILES
OBJS_CPP := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS_CPP))
OBJS_C   := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS_C))

OBJS := $(OBJS_CPP) $(OBJS_C)

# Examples Hello
HELLO_SRC := examples/hello/main.c
HELLO_TGT := build/examples/hello.exe

# ============================================================
# Tests
# ============================================================

TEST_DIR := tests
TEST_INC := $(TEST_DIR)/include

TEST_BUILD_DIR := $(BUILD_DIR)/tests

TEST_SRCS := $(shell find $(TEST_DIR) -type f -name "*.c")

TEST_EXES := $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/%.exe,$(TEST_SRCS))

# ============================================================
# Build Messages
# ============================================================

CC_MSG   := $(CYAN)[  CC]$(RESET)
LINK_MSG := $(MAGENTA)[LINK]$(RESET)
CLN_MSG  := $(RED)[ CLN]$(RESET)
RUN_MSG  := $(YELLOW)[ RUN]$(RESET)
DONE_MSG := $(GREEN)[DONE]$(RESET)
MKD_MSG  := $(BLUE)[ MKD]$(RESET)

# ============================================================
# Targets
# ============================================================

# Default Target
all: $(TARGET)
	@printf "$(TREE_LAST) $(DONE_MSG) Build successful --> $(GREEN)%s$(RESET)\n" "$(TARGET)"

# Examples hello
hello: $(HELLO_TGT)

# Test Target
test: $(TEST_EXES)
	@printf "\n$(BOLD)Running tests ($(BUILD))$(RESET)\n"

	@for test in $(TEST_EXES); do \
		printf "$(TREE_MID) $(RUN_MSG) %s\n" "$$test"; \
		$$test || exit 1; \
	done

	@printf "$(TREE_LAST) $(DONE_MSG) All tests passed\n"

# ============================================================
# Linking
# ============================================================

$(TARGET): $(OBJS) | $(BIN_DIR)
	@printf "$(TREE_MID) $(LINK_MSG) Linking objects --> $(GREEN)%s$(RESET)\n" "$(TARGET)"
	$(Q)$(CXX) $(OBJS) -o $(TARGET)

# Examples hello
$(HELLO_TGT): $(HELLO_SRC)
	@mkdir -p build/examples
	$(CC) $< -o $@

# ============================================================
# Build Directories
# ============================================================

$(BIN_DIR):
	@printf "$(TREE_MID) $(MKD_MSG) Creating bin directory\n"
	@mkdir -p $(BIN_DIR)

# ============================================================
# Compilation
# ============================================================

# C++ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d "$(dir $@)" ]; then \
		printf "$(TREE_MID) $(MKD_MSG) Creating directory --> %s\n" "$(dir $@)"; \
		mkdir -p "$(dir $@)"; \
	fi
	@printf "$(TREE_MID) $(CC_MSG) %s --> %s\n" "$<" "$@"
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

# C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if [ ! -d "$(dir $@)" ]; then \
		printf "$(TREE_MID) $(MKD_MSG) Creating directory --> %s\n" "$(dir $@)"; \
		mkdir -p "$(dir $@)"; \
	fi
	@printf "$(TREE_MID) $(CC_MSG) %s --> %s\n" "$<" "$@"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# test files
$(TEST_BUILD_DIR)/%.exe: $(TEST_DIR)/%.c
	@if [ ! -d "$(dir $@)" ]; then \
		printf "$(TREE_MID) $(MKD_MSG) Creating directory --> %s\n" "$(dir $@)"; \
		mkdir -p "$(dir $@)"; \
	fi
	@printf "$(TREE_MID) $(CC_MSG) %s --> %s\n" "$<" "$@"
	$(Q)$(CC) $(CFLAGS) -I$(TEST_INC) $< -o $@

# ============================================================
# Utility Targets
# ============================================================

clean:
	@printf "$(TREE_LAST) $(CLN_MSG) Removing build directory...\n"
	@rm -rf build

run-debug:
	@$(MAKE) BUILD=debug
	@printf "$(TREE_LAST) $(RUN_MSG) Running debug executable\n"
	@./build/debug/bin/app.exe

run-release:
	@$(MAKE) BUILD=release
	@printf "$(TREE_LAST) $(RUN_MSG) Running release executable\n"
	@./build/release/bin/app.exe

gdb:
	@$(MAKE) BUILD=debug
	@gdb ./build/debug/bin/app.exe

# Examples hello
run-hello: hello
	./$(HELLO_TGT)

# ============================================================
# Build Modes
# ============================================================

debug:
	@printf "$(TREE_MID) $(YELLOW)[ CFG]$(RESET) Debug build (-g3 -O0)\n"
	@$(MAKE) BUILD=debug

release:
	@printf "$(TREE_MID) $(YELLOW)[ CFG]$(RESET) Release build (-O2)\n"
	@$(MAKE) BUILD=release

# ============================================================
# Help
# ============================================================

help:
	@echo ""
	@printf "$(BOLD)Available Targets$(RESET)\n"
	@printf "$(BLUE)--------------------------------------------------$(RESET)\n"
	@echo "make                 Build debug version"
	@echo "make debug           Build debug version"
	@echo "make release         Build release version"
	@echo "make run-debug       Build and run debug version"
	@echo "make run-release     Build and run release version"
	@echo "make gdb             Launch GDB on debug build"
	@echo "make clean           Remove build directory"
	@echo "make test            Runs defined tests"
	@echo "make VERBOSE=1       Show compiler commands"
	@echo ""
	@echo ""
	@printf "$(BOLD)Examples Targets$(RESET)\n"
	@printf "$(BLUE)--------------------------------------------------$(RESET)\n"
	@echo "make hello           Build Hello World Example"
	@echo "make run-hello       Build and run Hello World Example"
	@echo ""

# ============================================================
# Auto Dependency Tracking
# ============================================================

-include $(OBJS:.o=.d)

# ============================================================
# Unknown Target Handler
# ============================================================

# .DEFAULT:
# 	@printf "$(TREE_LAST) $(RED)[ERR ]$(RESET) Target '$(YELLOW)%s$(RESET)' does not exist.\n" "$@"
# 	@printf "$(TREE_LAST) Run $(CYAN)make help$(RESET) for a list of available commands.\n"
# 	@exit 1

