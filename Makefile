# compile -> make
# compile and run -> make run

#define which compiler to use
CXX := g++
OUTPUT := tiledtest

LIB_DIR := /usr/lib
INC_DIR := /usr/include

CXX_FLAGS := -o3 -g -std=c++20
INCLUDES := -I./src -I$(INC_DIR) -I$(LIB_DIR)
LDFLAGS := -o3 -L $(LIB_DIR) -L $(INC_DIR) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -limgui -lImGui-SFML

SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

all:$(OUTPUT)

$(OUTPUT):$(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./build/$@

.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJ_SFMLGAME) ./build/tiledtest

run: $(OUTPUT)
	cd build && ./tiledtest && cd ..