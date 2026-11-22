# FridgeToFork: A C++/Web Recipe Recommender Based on Available Ingredients

**Project-Based Learning (Phase-III Final Deliverable) — Team T141**  
**Department of Computer Science & Engineering, Graphic Era (Deemed to be University), Dehradun**

## Team Members (T141)
- **Samarth Aggarwal** (2510010686) — Team Lead | WebAssembly Integration (`M5`), Front-End UI & Deployment
- **Govind Chhabra** (2510011653) — Core C++ Data Model (`M1`), Matching (`M2`), Deterministic Ranking (`M3`), Autocomplete (`M6`) & Benchmarks
- **Akansha Khandelwal** (2510011474) — Matching (`M2`), Near-Match Threshold Engine (`M4`) & System Documentation
- **Amogh Sharma** (2510012513) — Standardized Recipe Catalog (`data/recipes.json`), Unit/Edge Test Suite & Validation

---

## Architectural Highlights
1. **Core OOP C++17 Engine (`include/` & `src/`)**:
   - `Ingredient`: Trims whitespace, normalizes case, and singularizes common plural suffixes (`tomatoes` -> `tomato`, `eggs` -> `egg`, `berries` -> `berry`).
   - `Recipe`: Stores recipe metadata and an `std::unordered_set<std::string>` of normalized ingredients for $O(1)$ lookup.
   - `RecommendationEngine`: Computes exact matches (`0` missing) and near-matches (`1` or `2` missing ingredients **and** match score $\ge 50\%$).
2. **4-Tier Deterministic Ranking (`M3`)**:
   - **Tier 1**: Higher match percentage first.
   - **Tier 2**: Fewer missing ingredients first.
   - **Tier 3**: Fewer total required ingredients first (simpler recipes prioritized).
   - **Tier 4**: Alphabetical order by recipe title (`std::sort`).
3. **Single-Call WebAssembly Bridge (`M5` - `src/wasm_bridge.cpp`)**:
   - Exposes `initCatalog` and `recommendRecipes` via Emscripten `EMSCRIPTEN_BINDINGS` and C-ABI `EMSCRIPTEN_KEEPALIVE`.
4. **Interactive Front-End with Autocomplete (`M6` - `web/`)**:
   - Live autocomplete dropdown, quick-select staple pills, category/match filters (`All`, `Cook Now 100%`, `Near-Matches`), and missing-ingredient visual callouts.

---

## Quick Start & Build Instructions

### 1. Run Unit, Edge-Case, and Performance Benchmark Tests
```bash
make run-test
# Or directly with g++:
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src/Ingredient.cpp src/Recipe.cpp src/RecommendationEngine.cpp tests/test_suite.cpp -o test_runner
./test_runner
```

### 2. Run Native Command-Line Interface (CLI)
```bash
make run-cli
# Or pass custom ingredients:
./fridgetofork_cli data/recipes.json egg bread butter milk onion
```

### 3. Build WebAssembly Module (Requires Emscripten SDK)
```bash
make wasm
```

### 4. Launch Web Interface
You can either double-click `web/index.html` directly in your file browser, or serve the project directory over HTTP:
```bash
python3 -m http.server 8000
# Open http://localhost:8000/web/ in your browser
```
