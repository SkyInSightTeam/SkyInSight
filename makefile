ifeq ($(OS),Windows_NT)
	PRINT=@echo
	COMPILER=g++
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		PRINT=@printf
		COMPILER=clang++
	endif
	ifeq ($(UNAME_S),Linux)
		PRINT=@printf
		COMPILER=clang++
	endif
endif

ifeq ($(PRINT),)
	PRINT=@echo
endif

ifeq ($(COMPILER),)
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