#include "../include/RecommendationEngine.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

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

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "     FridgeToFork (T141) Core Unit Test Suite     " << std::endl;
    std::cout << "==================================================" << std::endl;

    testExactMatch();
    testNearMatch();
    testRankingAndTieBreaking();

    std::cout << "==================================================" << std::endl;
    std::cout << "          ALL 3 CORE UNIT TESTS PASSED!           " << std::endl;
    std::cout << "==================================================" << std::endl;
    return 0;
}
