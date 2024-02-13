OS := $(shell uname)

ifeq ($(OS),Darwin)  # macOS
	PRINT=@printf
	COMPILER=clang++
else ifeq ($(OS),Linux)
	PRINT=@printf
	COMPILER=clang++
else ifeq ($(OS),Windows)
	PRINT=@echo
	COMPILER=g++
endif

all: compiler

clean:
	$(PRINT) "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f out.bin *.gch SkyInsight
	$(PRINT) "\e[32m\tDONE\e[0m\n"

compiler:
	$(PRINT) "\e[32m--------| \e[1;32mCompilation of all your .cpp\e[0m\e[32m |--------\e[0m\n\n"
	$(COMPILER) -std=c++17 -o SkyInsight main.cpp
	$(PRINT) "\e[32m\tDONE\e[0m\n"

test: compiler
	$(PRINT) "\e[32m--------| \e[1mTest of the binary file\e[0m\e[32m |--------\e[0m\n\n"
	./SkyInsight
	$(PRINT) "\e[32m\tDONE\e[0m\n"

.PHONY: all clean test
