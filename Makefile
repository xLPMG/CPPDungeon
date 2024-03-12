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
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	SRC = $(shell find src -name "*.cpp")
else ifeq ($(UNAME),Linux)
	SRC = $(shell find src -name "*.cpp")
else ifeq ($(OS),Windows_NT)
	find_files = $(foreach n,$1,$(shell C:\\\msys64\\\usr\\\bin\\\find.exe -L $2 -name "$n"))
	SRC = $(call find_files,*.cpp,src)
	CXX += -mconsole
endif
OBJ = $(SRC:%.cpp=$(BIN)/%.o)
DEP = $(SRC:%.cpp=$(BIN)/%.d)
OUT = $(BIN)/game

-include $(DEP)

ifeq ($(UNAME),Darwin)
	CXX = $(shell brew --prefix llvm)/bin/clang
	LD = $(shell brew --prefix llvm)/bin/clang
	LDFLAGS += -framework OpenGL -framework GLUT -framework Carbon -lpng -L/usr/local/lib
	LDFLAGS += -framework CoreGraphics -framework Foundation
else ifeq ($(UNAME),Linux)
	LDFLAGS += -lX11 -lGL -lpthread -lpng -lstdc++fs
else ifeq ($(OS),Windows_NT)
	LDFLAGS += -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++20
endif

ifeq ($(OS),Windows_NT)
    # Windows
    COPY_CMD = cmd " /c robocopy "src" "bin/src" /e /xf * /mt"
else
    # Unix-like systems
    COPY_CMD = rsync -a --include '*/' --exclude '*' "src" "bin"
endif

$(BIN):
	mkdir -p $@

dirs: $(BIN)
	@echo "wtf $(COPY_CMD)"
	$(COPY_CMD)

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