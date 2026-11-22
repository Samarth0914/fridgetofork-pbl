#!/usr/bin/env bash
set -e

echo "===================================================="
echo "  Building FridgeToFork (Team T141) - Native & Test "
echo "===================================================="

g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src/Ingredient.cpp src/Recipe.cpp src/RecommendationEngine.cpp tests/test_suite.cpp -o test_runner
echo "[1/2] Built test_runner successfully."

g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src/Ingredient.cpp src/Recipe.cpp src/RecommendationEngine.cpp src/cli_main.cpp -o fridgetofork_cli
echo "[2/2] Built fridgetofork_cli successfully."

if command -v emcc >/dev/null 2>&1; then
    echo "[+] Emscripten detected! Compiling WebAssembly module..."
    emcc -std=c++17 -O3 -Iinclude --bind -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1 src/Ingredient.cpp src/Recipe.cpp src/RecommendationEngine.cpp src/wasm_bridge.cpp -o web/fridgetofork.js
    echo "[✓] WebAssembly build complete: web/fridgetofork.js & web/fridgetofork.wasm"
else
    echo "[i] Emscripten (emcc) not installed in current PATH; web/index.html will run using built-in client engine mirror."
fi

echo ""
echo "Running Automated Test Suite..."
./test_runner
