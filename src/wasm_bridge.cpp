#include "../include/RecommendationEngine.hpp"
#include <string>
#include <vector>
#include <sstream>

// Global persistent engine instance for the WebAssembly session
static RecommendationEngine g_engine;
static std::string g_lastResultJson = "[]";

// C++ Helper to parse simple JSON string array: ["egg", "milk", "butter"]
static std::vector<std::string> parseUserIngredientsJson(const std::string& jsonArr) {
    std::vector<std::string> list;
    size_t i = 0;
    while (i < jsonArr.size()) {
        size_t quoteStart = jsonArr.find('"', i);
        if (quoteStart == std::string::npos) break;
        size_t quoteEnd = jsonArr.find('"', quoteStart + 1);
        if (quoteEnd == std::string::npos) break;
        list.push_back(jsonArr.substr(quoteStart + 1, quoteEnd - quoteStart - 1));
        i = quoteEnd + 1;
    }
    return list;
}

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    int initCatalogFromJSON(const char* jsonCatalogStr) {
        if (!jsonCatalogStr) return 0;
        g_engine.clearCatalog();
        bool success = g_engine.loadRecipesFromJsonString(std::string(jsonCatalogStr));
        return success ? static_cast<int>(g_engine.getCatalogSize()) : 0;
    }

    EMSCRIPTEN_KEEPALIVE
    const char* recommendRecipesJSON(const char* userIngredientsJsonStr, int topN) {
        if (!userIngredientsJsonStr) {
            g_lastResultJson = "[]";
            return g_lastResultJson.c_str();
        }
        std::vector<std::string> userIngredients = parseUserIngredientsJson(std::string(userIngredientsJsonStr));
        auto results = g_engine.recommend(userIngredients, static_cast<size_t>(topN > 0 ? topN : 10));
        g_lastResultJson = RecommendationEngine::resultsToJson(results);
        return g_lastResultJson.c_str();
    }
}

// Embind bindings for direct JavaScript object mapping
std::string jsRecommend(const std::string& userIngredientsJson, int topN) {
    std::vector<std::string> userIngredients = parseUserIngredientsJson(userIngredientsJson);
    auto results = g_engine.recommend(userIngredients, static_cast<size_t>(topN > 0 ? topN : 10));
    return RecommendationEngine::resultsToJson(results);
}

bool jsInitCatalog(const std::string& jsonCatalog) {
    g_engine.clearCatalog();
    return g_engine.loadRecipesFromJsonString(jsonCatalog);
}

int jsGetCatalogSize() {
    return static_cast<int>(g_engine.getCatalogSize());
}

EMSCRIPTEN_BINDINGS(FridgeToForkModule) {
    emscripten::function("initCatalog", &jsInitCatalog);
    emscripten::function("recommendRecipes", &jsRecommend);
    emscripten::function("getCatalogSize", &jsGetCatalogSize);
}

#else
// Non-Emscripten stub for native development / mock bridge tests
bool nativeBridgeInitCatalog(const std::string& json) {
    return g_engine.loadRecipesFromJsonString(json);
}

std::string nativeBridgeRecommend(const std::string& userJson, int topN) {
    auto list = parseUserIngredientsJson(userJson);
    auto res = g_engine.recommend(list, topN);
    return RecommendationEngine::resultsToJson(res);
}
#endif
