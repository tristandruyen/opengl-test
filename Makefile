NAME = opengl-test
CFLAGS = -std=c++17 -O2
MFLAGS = -m64
# unix ######
#
LDFLAGS = -lGLEW -lglfw -lGL -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi # unix
IFLAGS = -I /usr/include
LFLAGS = -L /usr/lib
#############
# windoof ###
#
#MFLAGS = -mwindows
#LDFLAGS = -lglew32 -lglew32s -lglfw3 -lglfw3dll -lpthread -lgdi32 -lopengl32
#IFLAGS = -I H:\Code\Libs\glfw-3.3.4.bin.WIN64\include
#LFLAGS = -L H:\Code\Libs\glfw-3.3.4.bin.WIN64\lib-mingw-w64 -L /usr/local/lib
#############

build: main.cpp
	g++ $(MFLAGS) $(CFLAGS) -o $(NAME) main.cpp $(LDFLAGS) $(LFLAGS) $(IFLAGS)

build_debug:
	g++ $(MFLAGS) $(CFLAGS) -o $(NAME) main.cpp $(LDFLAGS) $(LFLAGS) $(IFLAGS) -D_DEBUG -ggdb

debug: build_debug
	gdb -q -batch -ex run -ex backtrace ./$(NAME)

.PHONY: test clean

test: build
	./$(NAME)

clean:
	rm -f $(NAME)
