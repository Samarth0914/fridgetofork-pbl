#include "../include/RecommendationEngine.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <chrono>
#include <fstream>
#include <sstream>

void testExactMatch() {
    RecommendationEngine engine;
    std::vector<Ingredient> ings = { Ingredient("egg"), Ingredient("butter"), Ingredient("milk") };
    engine.addRecipe(Recipe(1, "Scrambled Eggs", "Breakfast", 10, ings));

    std::vector<std::string> userHave = { "egg", "butter", "milk" };
    auto results = engine.recommend(userHave, 5);

    assert(results.size() == 1);
    assert(results[0].isExactMatch == true);
    assert(results[0].isNearMatch == false);
    assert(results[0].missingIngredients.empty());
    assert(std::abs(results[0].matchPercentage - 100.0) < 0.01);
    std::cout << "[PASS] Unit Test 1: Full Match (100% exact match, 0 missing)" << std::endl;
}

void testNearMatch() {
    RecommendationEngine engine;
    std::vector<Ingredient> ings = { Ingredient("bread"), Ingredient("cheese"), Ingredient("butter") };
    engine.addRecipe(Recipe(2, "Grilled Cheese", "Snack", 10, ings));

    // User missing butter (has 2 out of 3 = 66.7% match, missing 1)
    std::vector<std::string> userHave = { "bread", "cheese" };
    auto results = engine.recommend(userHave, 5);

    assert(results.size() == 1);
    assert(results[0].isExactMatch == false);
    assert(results[0].isNearMatch == true);
    assert(results[0].missingIngredients.size() == 1);
    assert(results[0].missingIngredients[0] == "butter");
    assert(results[0].matchPercentage > 66.0 && results[0].matchPercentage < 67.0);
    std::cout << "[PASS] Unit Test 2: Near Match (Missing 1 item, >= 50% threshold passed)" << std::endl;
}

void testRankingAndTieBreaking() {
    RecommendationEngine engine;
    engine.addRecipe(Recipe(1, "Recipe Alpha", "Dinner", 20, 
        { Ingredient("egg"), Ingredient("milk"), Ingredient("bread"), Ingredient("sugar") }));
    engine.addRecipe(Recipe(2, "Recipe Beta", "Snack", 5, 
        { Ingredient("bread"), Ingredient("butter") }));
    engine.addRecipe(Recipe(3, "Recipe Gamma", "Breakfast", 10, 
        { Ingredient("bread"), Ingredient("butter"), Ingredient("egg") }));
    engine.addRecipe(Recipe(4, "Recipe Delta", "Breakfast", 10, 
        { Ingredient("bread"), Ingredient("butter"), Ingredient("egg") }));

    std::vector<std::string> userHave = { "bread", "butter", "egg", "milk" };
    auto results = engine.recommend(userHave, 10);

    assert(results.size() == 4);
    assert(results[0].recipe.getTitle() == "Recipe Beta");
    assert(results[1].recipe.getTitle() == "Recipe Delta");
    assert(results[2].recipe.getTitle() == "Recipe Gamma");
    assert(results[3].recipe.getTitle() == "Recipe Alpha");
    std::cout << "[PASS] Unit Test 3: Ranking & 4-Tier Deterministic Tie-breaking" << std::endl;
}

void testEmptyInput() {
    RecommendationEngine engine;
    engine.addRecipe(Recipe(1, "Toast", "Snack", 5, { Ingredient("bread") }));

    std::vector<std::string> emptyHave;
    auto results = engine.recommend(emptyHave, 10);
    assert(results.empty());
    std::cout << "[PASS] Edge Test 4: Empty user input handled gracefully" << std::endl;
}

void testDuplicateAndDirtyInput() {
    RecommendationEngine engine;
    engine.addRecipe(Recipe(1, "Tomato Omelette", "Breakfast", 10, 
        { Ingredient("egg"), Ingredient("tomato") }));

    std::vector<std::string> dirtyHave = { "  EGG  ", "egg", "tomatoes", "TOMATO", "unknown_space_dust" };
    auto results = engine.recommend(dirtyHave, 10);

    assert(results.size() == 1);
    assert(results[0].isExactMatch == true);
    assert(results[0].matchedIngredients.size() == 2);
    std::cout << "[PASS] Edge Test 5: Duplicate, mixed-case, whitespace, and unknown ingredients handled" << std::endl;
}

void testMoreResultsRequestedThanAvailable() {
    RecommendationEngine engine;
    engine.addRecipe(Recipe(1, "Toast", "Snack", 5, { Ingredient("bread") }));
    engine.addRecipe(Recipe(2, "Butter Toast", "Snack", 5, { Ingredient("bread"), Ingredient("butter") }));

    std::vector<std::string> userHave = { "bread", "butter" };
    auto results = engine.recommend(userHave, 100);

    assert(results.size() == 2);
    std::cout << "[PASS] Edge Test 6: More results requested than available handled safely" << std::endl;
}

void testNearMatchFloorRejection() {
    RecommendationEngine engine;
    engine.addRecipe(Recipe(1, "Truffle Pasta", "Dinner", 30, 
        { Ingredient("pasta"), Ingredient("cream"), Ingredient("truffle") }));

    std::vector<std::string> userHave = { "pasta" };
    auto results = engine.recommend(userHave, 10, 50.0);

    assert(results.empty());
    std::cout << "[PASS] Edge Test 7: Near match floor rejection (<50% match discarded)" << std::endl;
}

void testJsonDatasetAndBridgeSerialization() {
    std::ifstream file("data/recipes.json");
    if (!file.is_open()) {
        file.open("fridgetofork/data/recipes.json");
    }
    assert(file.is_open());
    std::stringstream buffer;
    buffer << file.rdbuf();

    RecommendationEngine engine;
    bool ok = engine.loadRecipesFromJsonString(buffer.str());
    assert(ok);
    assert(engine.getCatalogSize() == 30);

    auto results = engine.recommend({ "egg", "butter", "milk", "bread" }, 5);
    assert(!results.empty());
    assert(results[0].isExactMatch == true);

    std::string jsonOut = RecommendationEngine::resultsToJson(results);
    assert(jsonOut.find("Classic Scrambled Eggs") != std::string::npos);
    assert(jsonOut.find("French Toast") != std::string::npos);
    std::cout << "[PASS] Integration Test 8: JSON Catalog Loader (30 recipes) & Bridge Serializer verified" << std::endl;
}

void testPerformanceLargeCatalog() {
    RecommendationEngine engine;
    std::vector<std::string> masterIngredients = {
        "egg", "bread", "butter", "milk", "cheese", "tomato", "onion",
        "potato", "garlic", "ginger", "rice", "chicken", "paneer", "spinach"
    };

    for (int i = 1; i <= 150; ++i) {
        std::vector<Ingredient> ings;
        ings.emplace_back(masterIngredients[i % masterIngredients.size()]);
        ings.emplace_back(masterIngredients[(i + 3) % masterIngredients.size()]);
        ings.emplace_back(masterIngredients[(i + 7) % masterIngredients.size()]);
        engine.addRecipe(Recipe(i, "Synthetic Recipe #" + std::to_string(i), "Test", 15, ings));
    }

    std::vector<std::string> userHave = { "egg", "bread", "butter", "onion", "garlic" };

    auto start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < 100; ++iter) {
        auto results = engine.recommend(userHave, 10);
    }
    auto end = std::chrono::high_resolution_clock::now();

    double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    double avgMs = totalMs / 100.0;

    std::cout << "[PASS] Performance Benchmark 9: Catalog of 150 recipes evaluated 100x." << std::endl;
    std::cout << "       Average query latency: " << avgMs << " ms (Threshold: < 5.0 ms)" << std::endl;
    assert(avgMs < 5.0);
}

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "     FridgeToFork (T141) Comprehensive Test Suite" << std::endl;
    std::cout << "==================================================" << std::endl;

    testExactMatch();
    testNearMatch();
    testRankingAndTieBreaking();
    testEmptyInput();
    testDuplicateAndDirtyInput();
    testMoreResultsRequestedThanAvailable();
    testNearMatchFloorRejection();
    testJsonDatasetAndBridgeSerialization();
    testPerformanceLargeCatalog();

    std::cout << "==================================================" << std::endl;
    std::cout << "  ALL 9 UNIT, EDGE, INTEGRATION & SPEED TESTS PASS" << std::endl;
    std::cout << "==================================================" << std::endl;
    return 0;
}
