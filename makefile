all: compiler

clean:
	@echo "Cleaning..."
	rm -f out.bin *.gch SkyInsight
	@echo "Cleaned."

compiler:
	@echo "Compiling..."
	g++ -std=c++17 -o SkyInsight main.cpp
	@echo "Compilation finished."

test: compiler
	@echo "Running tests..."
	./SkyInsight
	@echo "Tests finished."

.PHONY: all clean test
