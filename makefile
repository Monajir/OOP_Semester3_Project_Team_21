 #g++ main.cpp -o game.exe -std=c++14 -IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm
comp := g++
flag := -std=c++17 -IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm

cppfile := $(wildcard src/*.cpp)

game.exe :
	$(comp) $(cppfile) -o game.exe $(flag) 


clean :
	@rm -f game.exe

