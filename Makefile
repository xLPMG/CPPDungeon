print-%:; @echo $($*)

CXX = g++
LD = g++

# library paths
PATH_LIB = lib
PATH_SDL = $(PATH_LIB)/SDL
PATH_SDL_IMG = $(PATH_LIB)/SDL_image

INCFLAGS = -iquotesrc

CXXFLAGS  = -std=c++20
CXXFLAGS += -O2
CXXFLAGS += -g
CXXFLAGS += -fbracket-depth=1024
CXXFLAGS += -fmacro-backtrace-limit=0
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

	INCFLAGS += -I$(PATH_SDL)/include
	INCFLAGS += -I$(PATH_SDL_IMG)/include
	LDFLAGS += $(shell $(BIN)/sdl/sdl2-config --prefix=$(BIN) --static-libs)
else ifeq ($(UNAME),Linux)
	LDFLAGS += -lSDL2
endif
	LDFLAGS += -lSDL2_image

$(BIN):
	mkdir -p $@

dirs: $(BIN)
	rsync -a --include '*/' --exclude '*' "src" "bin"

lib-sdl:
	mkdir -p $(BIN)/sdl
	cmake -S $(PATH_SDL) -B $(BIN)/sdl
	cd $(BIN)/sdl && make -j 4
	chmod +x $(BIN)/sdl/sdl2-config
	mkdir -p $(BIN)/lib
	cp $(BIN)/sdl/libSDL2.a $(BIN)/lib


lib-sdl_img:
	cd $(PATH_SDL_IMG) && make -j 4 install
	cp $(BIN)/sdl_image/lib/libSDL2_image.a $(BIN)/lib

libs: lib-sdl lib-sdl_img

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