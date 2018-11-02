CC = clang++
OUT = bin/rpg
SRC = $(wildcard Classes/*.cpp)
DEP = $(wildcard Headers/*.h)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
COMPILER_FLAGS = -Wall -I/usr/include/SDL2
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o $(OUT)
all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS)
obj/%.o: src/%.cpp $(DEP)
	$(CC) $< $(COMPILER_FLAGS) -0 $@

clean:
