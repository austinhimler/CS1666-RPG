CC = clang++
SRC = $(wildcard Classes/*.cpp) $(wildcard Classes/*/*.cpp)
DEP = $(wildcard Headers/*/*.h) $(wildcard Headers/*.h)
OBJ = $(patsubst $(SRC), obj/%.o, $(SRC))
COMPILER_FLAGS = -Wall -I/usr/include/SDL2 -I/usr/include/SOIL -I/usr/include/glm -I/usr/include/GL -I/include/
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lGLEW -lGL -lSOIL

all: cs1666rpg.o
cs1666rpg.o: $(OBJ)
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@
obj/%.o: $(SRC) $(DEP)
	$(CC) $< -c $(COMPILER_FLAGS) -o $@
clean:
	rm obj/*.o
	rm cs1666rpg.o
