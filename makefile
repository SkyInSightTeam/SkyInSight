ifeq ($(OS),Windows_NT)
	PRINT=@echo -e
	COMPILER=g++
    COLOR=\033
else
	UNAME_S := $(shell uname -s)
    COLOR=\e
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
	$(PRINT) "\n$(COLOR)[32m--------| $(COLOR)[1;32mCLEANING$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m\n\n"
	rm -f out.bin *.gch SkyInsight
	$(PRINT) "$(COLOR)[32m\tDONE$(COLOR)[0m\n"

compiler:
	$(PRINT) "$(COLOR)[32m--------| $(COLOR)[1;32mCompilation of all your .cpp$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m\n\n"
	$(COMPILER) -std=c++17 -o SkyInsight main.cpp
	$(PRINT) "$(COLOR)[32m\tDONE$(COLOR)[0m\n"

test: compiler
	$(PRINT) "--------| Test of the binary file |--------\n\n"
	./SkyInsight
	$(PRINT) "\tDONE$(COLOR)[0m\n"

.PHONY: all clean test