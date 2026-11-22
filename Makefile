CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
EMCC = emcc
EMFLAGS = -std=c++17 -O3 -Iinclude --bind -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1

SRC_CORE = src/Ingredient.cpp src/Recipe.cpp src/RecommendationEngine.cpp

.PHONY: all cli test wasm run-test run-cli clean

all: cli test

cli: $(SRC_CORE) src/cli_main.cpp
	$(CXX) $(CXXFLAGS) $(SRC_CORE) src/cli_main.cpp -o fridgetofork_cli

test: $(SRC_CORE) tests/test_suite.cpp
	$(CXX) $(CXXFLAGS) $(SRC_CORE) tests/test_suite.cpp -o test_runner

wasm: $(SRC_CORE) src/wasm_bridge.cpp
	$(EMCC) $(EMFLAGS) $(SRC_CORE) src/wasm_bridge.cpp -o web/fridgetofork.js

run-test: test
	./test_runner

run-cli: cli
	./fridgetofork_cli data/recipes.json egg bread butter milk

clean:
	rm -f fridgetofork_cli test_runner web/fridgetofork.js web/fridgetofork.wasm
