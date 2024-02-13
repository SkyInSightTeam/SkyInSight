OS := $(shell uname)

ifeq ($(OS),Darwin)  # macOS
	COMPILER=clang++
else ifeq ($(OS),Linux)
	COMPILER=clang++
else ifeq ($(OS),Windows)
	COMPILER=g++
endif

all: compiler

clean:
	@printf "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f out.bin *.gch SkyInsight
	@printf "\e[32m\tDONE\e[0m\n"

compiler: 
	@printf "\e[32m--------| \e[1;32mCompilation of all your .cpp\e[0m\e[32m |--------\e[0m\n\n"
	$(COMPILER) -std=c++17 -o SkyInsight main.cpp
	@printf "\e[32m\tDONE\e[0m\n"

test: compiler
	@printf "\e[32m--------| \e[1mTest of the binary file\e[0m\e[32m |--------\e[0m\n\n"
	./SkyInsight
	@printf "\e[32m\tDONE\e[0m\n"

.PHONY: all clean test

