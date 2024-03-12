######################################################################
# Makefile for CPPDungeon
# Author: 	Luca-Philipp Grumbach
######################################################################

EXEC = dungeon

# TOOLS
CXX = g++
LD = g++

# TARGET OS
ifeq ($(OS),Windows_NT)
	OS = windows
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Darwin)
		OS = macOS
	else ifeq ($(UNAME),Linux)
		OS = linux
	else
    	$(error OS not supported by this Makefile)
	endif
endif

# DIRECTORIES
SRC_DIR = src
BIN_DIR_ROOT = bin
ASSETS_DIR = res
LIB_DIR= lib

# OS-SPECIFIC DIRECTORIES
BIN_DIR := $(BIN_DIR_ROOT)/$(OS)
ifeq ($(OS),windows)
	# Windows 32-bit
	ifeq ($(win32),1)
		BIN_DIR := $(BIN_DIR)32
	# Windows 64-bit
	else
		BIN_DIR := $(BIN_DIR)64
	endif
endif


# INCLUDES
INCFLAGS = -I$(LIB_DIR)
INCFLAGS += -I/usr/local/include

# COMPILER FLAGS
CXXFLAGS  = -std=c++20
CXXFLAGS += -O2
CXXFLAGS += -g
CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Wpedantic

# LINKER FLAGS
LDFLAGS = -lm -lstdc++

# LINKER LIBRARIES
ifeq ($(OS),macOS)
	LDFLAGS += -framework OpenGL -framework GLUT -framework Carbon -lpng -L/usr/local/lib
	LDFLAGS += -framework CoreGraphics -framework Foundation
else ifeq ($(OS),linux)
	LDFLAGS += -lX11 -lGL -lpthread -lpng -lstdc++fs
else ifeq ($(OS),windows)
	LDFLAGS += -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++20
endif

# GATHER SOURCES
ifeq ($(OS),macOS)
	SRC = $(shell find src -name "*.cpp")
else ifeq ($(OS),linux)
	SRC = $(shell find src -name "*.cpp")
else ifeq ($(OS),windows)
	find_files = $(foreach n,$1,$(shell C:\\\msys64\\\usr\\\bin\\\find.exe -L $2 -name "$n"))
	SRC = $(call find_files,*.cpp,src)
	CXX += -mconsole
endif

OBJ = $(SRC:%.cpp=$(BIN_DIR)/%.o)
DEP = $(SRC:%.cpp=$(BIN_DIR)/%.d)
-include $(DEP)

# DIRECTORY COPY COMMAND
ifeq ($(OS),windows)
    COPY_DIRS_CMD = cmd " /c robocopy "$(SRC_DIR)" "$(BIN_DIR)/$(SRC_DIR)" /e /xf * /mt"
else ifeq ($(OS),macOS)
	COPY_DIRS_CMD = rsync -a --include '*/' --exclude '*' "$(SRC_DIR)" "$(BIN_DIR)"
else ifeq ($(OS),linux)
	COPY_DIRS_CMD = rsync -a --include '*/' --exclude '*' "$(SRC_DIR)" "$(BIN_DIR)"
endif

# ASSETS COPY COMMAND
ifeq ($(OS),windows)
    COPY_ASSETS_CMD = cmd " /c robocopy "$(ASSETS_DIR)" "$(BIN_DIR)/$(ASSETS_DIR)" /s /purge /mt"
else ifeq ($(OS),macOS)
	COPY_ASSETS_CMD = rsync -a "$(ASSETS_DIR)" "$(BIN_DIR)"
else ifeq ($(OS),linux)
	COPY_ASSETS_CMD = rsync -a "$(ASSETS_DIR)" "$(BIN_DIR)"
endif

# TARGETS
.DEFAULT_GOAL := dungeon

$(BIN_DIR):
	mkdir -p $@

createdirs: $(BIN_DIR)
	$(COPY_DIRS_CMD)

$(OBJ): $(BIN_DIR)/%.o: %.cpp
	$(CXX) -o $@ -MMD -c $< $(CXXFLAGS) $(INCFLAGS)

dungeon: createdirs $(OBJ)
	$(LD) -o $(BIN_DIR)/$(EXEC) $(filter %.o, $^) $(LDFLAGS)
	
copyassets:
	$(COPY_ASSETS_CMD)

run:
	./$(BIN_DIR)/$(EXEC)

.PHONY: clean

clean:
	rm -rf $(BIN_DIR)/$(SRC_DIR)

all:
	@echo "Building for $(OS)"
	@$(MAKE) -s dungeon
	@$(MAKE) -s copyassets
	@$(MAKE) -s clean
	@echo "Build finished"