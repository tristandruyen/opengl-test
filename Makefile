CFLAGS = -std=c++17 -O2
MFLAGS = -m64
# unix ######
#
LDFLAGS = -lglfw -lGL -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi # unix
#############

# windoof ###
#
#MFLAGS = -mwindows
#LDFLAGS = -lglfw3 -lglfw3dll -lpthread -lgdi32 -lopengl32
#IFLAGS = -I H:\Code\Libs\glfw-3.3.4.bin.WIN64\include
#LFLAGS = -L H:\Code\Libs\glfw-3.3.4.bin.WIN64\lib-mingw-w64 -L /usr/local/lib
#############


NAME = opengl-test
build: main.cpp
        g++ $(MFLAGS) $(CFLAGS) -o $(NAME) main.cpp $(LDFLAGS) $(LFLAGS) $(IFLAGS)

.PHONY: test clean

test: build
        ./VulkanTest

clean:
        rm -f $(NAME)

