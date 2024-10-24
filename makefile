CC=gcc
CFLAGS=-g -Wall -Wformat

all: ./bin/main

./bin/main: ./bin/main.o ./bin/shader.o ./bin/texture.o ./bin/pipeline.o ./bin/construct.o ./bin/scene.o
	$(CC) -o $@ $^ -L./lib/ -lglad -lglfw3 -lgdi32

./bin/%.o: ./src/%.c
	$(CC) -c -o $@ $^ -I./src/ -I./lib/include/ $(CFLAGS)

run:
	@./bin/main

clean:
	@del /q bin


# structure
# ./lib/
#        include/
#                glad/
#                GLFW/
#                KHR/
#        libglad.a
#        libglfw3.a
#        (glad.c)
