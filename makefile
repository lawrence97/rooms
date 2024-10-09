all: ./bin/main

./bin/main: ./bin/main.o
	gcc -o $@ $^ -L./lib/ -lglad -lglfw3 -lgdi32

./bin/%.o: ./src/%.c
	gcc -c -o $@ $^ -I./src/ -I./lib/include/ -g -Wall

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
