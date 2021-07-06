CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lGL -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi # unix
# LDFLAGS = -lglfw -lopengl32 -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi # windoof
NAME = opengl-test

build: main.cpp
	g++ $(CFLAGS) -o $(NAME) main.cpp $(LDFLAGS)

.PHONY: test clean

test: build
	./VulkanTest

clean:
	rm -f $(NAME)
