NAME = opengl-test
CFLAGS = -std=c++17 -O2
MFLAGS = -m64
# unix ######
#
LDFLAGS = -lGLEW -lglfw -lGL -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi # unix
IFLAGS = -I /usr/include -I ./src
LFLAGS = -L /usr/lib
#############
# windoof ###
#
#MFLAGS = -mwindows
#LDFLAGS = -lglew32 -lglew32s -lglfw3 -lglfw3dll -lpthread -lgdi32 -lopengl32
#IFLAGS = -I H:\Code\Libs\glfw-3.3.4.bin.WIN64\include
#LFLAGS = -L H:\Code\Libs\glfw-3.3.4.bin.WIN64\lib-mingw-w64 -L /usr/local/lib
#############
#############SOURCES = main.cpp file1.cpp
SOURCES = ./src/VertexArray.cpp ./src/VertexBufferLayout.cpp ./src/IndexBuffer.cpp  ./src/VertexBuffer.cpp ./src/Renderer.cpp ./src/main.cpp
DFLAGS = -D_DEBUG -ggdb


all: build

build:
	g++ $(MFLAGS) $(CFLAGS) -o $(NAME) $(LDFLAGS) $(LFLAGS) $(IFLAGS) $(SOURCES)

build_debug:
	g++ $(MFLAGS) $(CFLAGS) -o $(NAME) $(LDFLAGS) $(LFLAGS) $(IFLAGS) $(DFLAGS) $(SOURCES)

debug: build_debug
	gdb -q -batch -ex run -ex backtrace ./$(NAME)

.PHONY: test clean

test: build
	./$(NAME)

clean:
	rm -f $(NAME)
