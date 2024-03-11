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
		COMPILER=g++
	endif
endif

ifeq ($(PRINT),)
	PRINT=@echo
endif

ifeq ($(COMPILER),)
	COMPILER=g++
endif

all: compiler

install:
	$(PRINT) "\n$(COLOR)--------| $(COLOR)[1;32mINSTALLING FTXUI$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m\n\n"
	git clone https://github.com/ArthurSonzogni/FTXUI.git
	cd FTXUI && mkdir build && cd build && cmake .. && make && sudo make install
	$(PRINT) "$(COLOR)[32m\tDONE$(COLOR)[0m\n"

clean:
	$(PRINT) "\n$(COLOR)--------| $(COLOR)[1;32mCLEANING$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m\n\n"
	rm -f out.bin *.gch SkyInsight
	$(PRINT) "$(COLOR)[32m\tDONE$(COLOR)[0m\n"

compiler:
	$(PRINT) "$(COLOR)[32m--------| $(COLOR)[1;32mCompilation of all your .cpp$(COLOR)[0m$(COLOR)[32m |--------$(COLOR)[0m\n\n"
	$(COMPILER) -std=c++17 -o SkyInsight main.cpp src/Date/Date.cpp src/WeatherApiCaller/*.cpp src/City/City.cpp -Iinclude/ -lcurlpp -lcurl -L/usr/local/lib -lftxui-component -lftxui-dom -lftxui-screen
	$(PRINT) "$(COLOR)[32m\tDONE$(COLOR)[0m\n"

test: compiler
	$(PRINT) "$(COLOR)[32m--------| Test of the binary file |--------$(COLOR)[0m\n\n"
	./SkyInsight
	$(PRINT) "\tDONE$(COLOR)[0m\n"

.PHONY: all clean test