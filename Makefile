CC = g++
CCFLAGS =
LDFLAGS =

all: compile run

compile: build src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

ifeq ($(OS),Windows_NT)

build:
	mkdir build
run: build/main.o
	build/main.o -in input.png -par
clean:
	rmdir /s /q build

else

build:
	@mkdir -p build
run: build/main.o
	@./build/main.o
clean:
	rm -rf build

endif
