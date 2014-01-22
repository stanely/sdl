CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -L/home/stanely/sdl/SDL2-2.0.1/build_out/lib -lSDL2_ttf -lSDL2_image -lSDL2 -Wl,-rpath=/home/stanely/sdl/SDL2-2.0.1/build_out/lib
SDL_INCLUDE = -I/home/stanely/sdl/SDL2-2.0.1/build_out/include

#SDL_LIB += -L/home/stanely/sdl/SDL2_image-2.0.0/build_out/lib -lSDL2_image
#SDL_INCLUDE += -I/home/stanely/sdl/SDL2_image-2.0.0/build_out/include

# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = SDL_Test2

all: $(EXE)

$(EXE): main.o
	$(CXX) $< $(LDFLAGS) -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
