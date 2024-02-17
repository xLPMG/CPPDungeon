print-%:; @echo $($*)

CXX = g++
LD = g++

# library paths
PATH_LIB = lib
INCFLAGS = -I$(PATH_LIB)
INCFLAGS += -I/usr/local/include

CXXFLAGS  = -std=c++20
CXXFLAGS += -O2
CXXFLAGS += -g
CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -Wpedantic
CXXFLAGS += -Wfloat-equal
CXXFLAGS += -Wstrict-aliasing
CXXFLAGS += -Wswitch-default
CXXFLAGS += -Wformat=2
CXXFLAGS += -Wno-newline-eof
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -Wno-strict-prototypes
CXXFLAGS += -Wno-fixed-enum-extension
CXXFLAGS += -Wno-int-to-void-pointer-cast
CXXFLAGS += -Wno-gnu-statement-expression
CXXFLAGS += -Wno-gnu-compound-literal-initializer
CXXFLAGS += -Wno-gnu-zero-variadic-macro-arguments
CXXFLAGS += -Wno-gnu-empty-struct
CXXFLAGS += -Wno-gnu-auto-type
CXXFLAGS += -Wno-gnu-empty-initializer
CXXFLAGS += -Wno-gnu-pointer-arith

LDFLAGS = -lm
LDFLAGS += -lstdc++

BIN = bin
SRC = $(shell find src -name "*.cpp")
OBJ = $(SRC:%.cpp=$(BIN)/%.o)
DEP = $(SRC:%.cpp=$(BIN)/%.d)
OUT = $(BIN)/game

-include $(DEP)

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	CXX = $(shell brew --prefix llvm)/bin/clang
	LD = $(shell brew --prefix llvm)/bin/clang
	LDFLAGS += -framework OpenGL -framework GLUT -framework Carbon -lpng -L/usr/local/lib
	LDFLAGS += -framework CoreGraphics -framework Foundation
else ifeq ($(UNAME),Linux)
	LDFLAGS += -lX11 -lGL -lpthread -lpng -lstdc++fs
else ifeq ($(UNAME),Windows)
	LDFLAGS += -user32 -gdi32 -opengl32 -gdiplus -Shlwapi -dwmapi -stdc++fs
endif

$(BIN):
	mkdir -p $@

dirs: $(BIN)
	rsync -a --include '*/' --exclude '*' "src" "bin"


$(OBJ): $(BIN)/%.o: %.cpp
	$(CXX) -o $@ -MMD -c $< $(CXXFLAGS) $(INCFLAGS)

dungeon: dirs $(OBJ)
	$(LD) -o $(BIN)/dungeon $(filter %.o, $^) $(LDFLAGS)

run:
	./bin/dungeon

all: dirs dungeon

.PHONY: clean

clean:
	rm -rf bin/src