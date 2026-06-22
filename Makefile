# Makefile

# ============================================================
# User Configuration
# ============================================================

BUILD   ?= debug
VERBOSE ?= 0

MAKEFLAGS += --no-print-directory

.DEFAULT_GOAL := all

# ============================================================
# Build Validation
# ============================================================

VALID_BUILDS := debug release

ifeq ($(filter $(BUILD),$(VALID_BUILDS)),)
$(error Invalid BUILD='$(BUILD)'. Valid values: debug, release)
endif

# ============================================================
# Toolchain
# ============================================================

CC  := gcc
CXX := g++

# ============================================================
# Platform Configuration
# ============================================================

ifeq ($(OS),Windows_NT)
	EXE := .exe
else
	EXE :=
endif

# ============================================================
# Colors
# ============================================================

RESET   := \033[0m
BOLD    := \033[1m

RED     := \033[31m
GREEN   := \033[32m
YELLOW  := \033[33m
BLUE    := \033[34m
MAGENTA := \033[35m
CYAN    := \033[36m
GRAY    := \033[38;5;244m

# ============================================================
# Tree UI
# ============================================================

TREE_MID  := $(GRAY)├─$(RESET)
TREE_LAST := $(GRAY)└─$(RESET)
TREE_SUB  := $(GRAY)│  └─$(RESET)

# ============================================================
# Message Labels
# ============================================================

CC_MSG   := $(CYAN)[  CC]$(RESET)
LINK_MSG := $(MAGENTA)[LINK]$(RESET)
CLN_MSG  := $(RED)[ CLN]$(RESET)
RUN_MSG  := $(YELLOW)[ RUN]$(RESET)
DONE_MSG := $(GREEN)[DONE]$(RESET)
MKD_MSG  := $(BLUE)[ MKD]$(RESET)

WARN_MSG := $(YELLOW)[WARN]$(RESET)
INFO_MSG := $(CYAN)[INFO]$(RESET)
CFG_MSG  := $(YELLOW)[ CFG]$(RESET)
ERR_MSG  := $(RED)[ERR ]$(RESET)

# ============================================================
# Verbose Helpers
# ============================================================

ifeq ($(VERBOSE),1)

define cmd
	@printf "$(TREE_SUB) $(GRAY)%s$(RESET)\n" "$(1)"
	@$(1)
endef

else

define cmd
	@$(1)
endef

endif

# ============================================================
# Build Configuration
# ============================================================

COMMON_CFLAGS   := -Wall -Wextra -Wpedantic -Iinclude -MMD -MP
COMMON_CXXFLAGS := -Wall -Wextra -Wpedantic -Iinclude -MMD -MP

ifeq ($(BUILD),debug)

    BUILD_DIR := build/debug

    CFLAGS   := $(COMMON_CFLAGS)   -std=c23   -g3 -O0
    CXXFLAGS := $(COMMON_CXXFLAGS) -std=c++23 -g3 -O0 -fno-omit-frame-pointer

else

    BUILD_DIR := build/release

    CFLAGS   := $(COMMON_CFLAGS)   -std=c23   -O2
    CXXFLAGS := $(COMMON_CXXFLAGS) -std=c++23 -O2

endif

# ============================================================
# Project Layout
# ============================================================

SRC_DIR  := src
TEST_DIR := tests

BIN_DIR  := $(BUILD_DIR)/bin
OBJ_DIR  := $(BUILD_DIR)/obj

TARGET := $(BIN_DIR)/app$(EXE)

DEBUG_BUILD_DIR   := build/debug
RELEASE_BUILD_DIR := build/release

DEBUG_TARGET   := $(DEBUG_BUILD_DIR)/bin/app$(EXE)
RELEASE_TARGET := $(RELEASE_BUILD_DIR)/bin/app$(EXE)

# ============================================================
# Source Discovery
# ============================================================

SRCS_CPP := $(shell find $(SRC_DIR) -type f -name "*.cpp")
SRCS_C   := $(shell find $(SRC_DIR) -type f -name "*.c")

OBJS_CPP := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS_CPP))
OBJS_C   := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS_C))

OBJS := $(OBJS_CPP) $(OBJS_C)

# Dependency Files(.d)
DEP_FILES := $(OBJS:.o=.d)

# ============================================================
# Examples
# ============================================================

HELLO_SRC := examples/hello/main.c
HELLO_TGT := build/examples/hello$(EXE)

# ============================================================
# Tests
# ============================================================

TEST_INC := $(TEST_DIR)/include

TEST_BUILD_DIR := $(BUILD_DIR)/tests

TEST_SRCS := $(shell find $(TEST_DIR) -type f -name "*.c")

TEST_EXES := $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/%$(EXE),$(TEST_SRCS))

TEST_OBJS := $(filter-out %main.o,$(OBJS))

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
# Main Targets
# ============================================================

hello: $(HELLO_TGT)

test: $(TEST_EXES)
	@printf "\n$(BOLD)Running tests ($(BUILD))$(RESET)\n"

	@for test in $(TEST_EXES); do \
		printf "$(TREE_MID) $(RUN_MSG) Running $(GREEN)%s$(RESET)\n" "$$test"; \
		$$test || exit 1; \
	done

	@printf "$(TREE_LAST) $(DONE_MSG) All tests passed\n"


# ============================================================
# Default Target & Link Rules
# ============================================================

ifeq ($(strip $(OBJS)),)

all: $(TARGET)
	@printf "$(TREE_LAST) $(INFO_MSG) Nothing to build\n"

$(TARGET):
	@printf "$(TREE_LAST) $(WARN_MSG) No source files found\n"

else

all: $(TARGET)
	@printf "$(TREE_LAST) $(DONE_MSG) Build Successful --> $(GREEN)%s$(RESET)\n" "$(TARGET)"

$(TARGET): $(OBJS) | $(BIN_DIR)
	@for ob_file in $(OBJS); do \
		printf "$(TREE_MID) $(LINK_MSG) Linking object file $(CYAN)%s$(RESET) --> $(MAGENTA)%s$(RESET)\n" "$$ob_file" "$@"; \
	done
	$(call cmd,$(CXX) $(OBJS) -o $(TARGET))

endif

# ============================================================
# Directory Rules
# ============================================================

$(BIN_DIR):
	@printf "$(TREE_MID) $(MKD_MSG) Creating directory --> $(BLUE)%s$(RESET)\n" "$@"
	@mkdir -p $@

# ============================================================
# Compile Rules
# ============================================================

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d "$(dir $@)" ]; then \
		printf "$(TREE_MID) $(MKD_MSG) Creating directory --> $(BLUE)%s$(RESET)\n" "$(dir $@)"; \
		mkdir -p "$(dir $@)"; \
	fi
	@printf "$(TREE_MID) $(CC_MSG) Compiling source file $(CYAN)%s$(RESET) --> $(GREEN)%s$(RESET)\n" "$<" "$@"
	$(call cmd,$(CXX) $(CXXFLAGS) -c $< -o $@)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if [ ! -d "$(dir $@)" ]; then \
		printf "$(TREE_MID) $(MKD_MSG) Creating directory --> $(BLUE)%s$(RESET)\n" "$(dir $@)"; \
		mkdir -p "$(dir $@)"; \
	fi
	@printf "$(TREE_MID) $(CC_MSG) Compiling source file $(CYAN)%s$(RESET) --> $(GREEN)%s$(RESET)\n" "$<" "$@"
	$(call cmd,$(CC) $(CFLAGS) -c $< -o $@)

# ============================================================
# Example Rules
# ============================================================

$(HELLO_TGT): $(HELLO_SRC)
	@mkdir -p build/examples
	@printf "$(TREE_MID) $(CC_MSG) %s\n" "$<"
	$(call cmd,$(CC) $< -o $@)
	@printf "$(TREE_LAST) $(DONE_MSG) %s\n" "$@"

# ============================================================
# Test Rules
# ============================================================

$(TEST_BUILD_DIR)/%$(EXE): $(TEST_DIR)/%.c $(TEST_OBJS)
	@if [ ! -d "$(dir $@)" ]; then \
		printf "$(TREE_MID) $(MKD_MSG) Creating directory --> $(BLUE)%s$(RESET)\n" "$(dir $@)"; \
		mkdir -p "$(dir $@)"; \
	fi

	@printf "$(TREE_MID) $(CC_MSG) Compiling test file $(CYAN)%s$(RESET) --> $(GREEN)%s$(RESET)\n" "$<" "$@"

	$(call cmd,$(CC) $(CFLAGS) -I$(TEST_INC) $^ -o $@)

# ============================================================
# Run Targets
# ============================================================

run-debug:
	@$(MAKE) BUILD=debug
	@if [ -f "$(DEBUG_TARGET)" ]; then \
		printf "$(TREE_LAST) $(RUN_MSG) Running $(GREEN)%s$(RESET)\n" "$(DEBUG_TARGET)"; \
		"./$(DEBUG_TARGET)"; \
	else \
		printf "$(TREE_LAST) $(WARN_MSG) No executable to run\n"; \
	fi

run-release:
	@$(MAKE) BUILD=release
	@if [ -f "$(RELEASE_TARGET)" ]; then \
		printf "$(TREE_LAST) $(RUN_MSG) Running $(GREEN)%s$(RESET)\n" "$(RELEASE_TARGET)"; \
		"./$(RELEASE_TARGET)"; \
	else \
		printf "$(TREE_LAST) $(WARN_MSG) No executable to run\n"; \
	fi

gdb:
	@$(MAKE) BUILD=debug
	@if [ -f "$(DEBUG_TARGET)" ]; then \
		printf "$(TREE_LAST) $(RUN_MSG) Launching GDB\n"; \
		gdb "$(DEBUG_TARGET)"; \
	else \
		printf "$(TREE_LAST) $(WARN_MSG) No executable available for GDB\n"; \
	fi

run-hello: hello
	@printf "$(TREE_LAST) $(RUN_MSG) Running $(GREEN)%s$(RESET)\n" "$(HELLO_TGT)"
	@./$(HELLO_TGT)

# ============================================================
# Build Modes
# ============================================================

debug:
	@printf "$(TREE_MID) $(CFG_MSG) Debug build\n"
	@$(MAKE) BUILD=debug

release:
	@printf "$(TREE_MID) $(CFG_MSG) Release build\n"
	@$(MAKE) BUILD=release

# ============================================================
# Utility Targets
# ============================================================

clean:
	@printf "$(TREE_LAST) $(CLN_MSG) Removing build directory\n"
	@rm -rf build

# ============================================================
# Help
# ============================================================

help:
	@echo ""
	@printf "$(BOLD)Available Targets$(RESET)\n\n"

	@printf "$(CYAN)Build$(RESET)\n"
	@printf "├─ %-18s %s\n" "make"            "Build project using current BUILD mode (default: debug)"
	@printf "├─ %-18s %s\n" "make debug"      "Build project in debug mode (-g3 -O0)"
	@printf "└─ %-18s %s\n\n" "make release"    "Build project in release mode (-O2)"

	@printf "$(CYAN)Run$(RESET)\n"
	@printf "├─ %-18s %s\n" "make run-debug"   "Build and run the debug executable"
	@printf "├─ %-18s %s\n" "make run-release" "Build and run the release executable"
	@printf "└─ %-18s %s\n\n" "make gdb"       "Launch GDB with the debug executable"

	@printf "$(CYAN)Tests$(RESET)\n"
	@printf "└─ %-18s %s\n\n" "make test" "Build and execute all tests"

	@printf "$(CYAN)Examples$(RESET)\n"
	@printf "├─ %-18s %s\n" "make hello"      "Build the Hello World example"
	@printf "└─ %-18s %s\n\n" "make run-hello" "Build and run the Hello World example"

	@printf "$(CYAN)Utilities$(RESET)\n"
	@printf "├─ %-18s %s\n" "make clean"      "Remove all generated build artifacts"
	@printf "└─ %-18s %s\n\n" "make help"       "Show available targets and usage information"

	@printf "$(CYAN)Configuration$(RESET)\n"
	@printf "├─ %-18s %s\n" "BUILD=debug"   "Build using debug configuration"
	@printf "├─ %-18s %s\n" "BUILD=release" "Build using release configuration"
	@printf "└─ %-18s %s\n\n" "VERBOSE=1"     "Show compiler and linker commands"
	@echo ""

# ============================================================
# Dependencies
# ============================================================

-include $(wildcard $(DEP_FILES))

# ============================================================
# Unknown Target Handler
# ============================================================

.DEFAULT:
	@printf "$(TREE_LAST) $(ERR_MSG) Unknown target: $(YELLOW)%s$(RESET)\n" "$@"
	@printf "$(TREE_LAST) Run $(CYAN)make help$(RESET)\n"
	@exit 1