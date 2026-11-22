// M2-M3 Core Matching & 4-Tier Deterministic Ranking - Govind Chhabra
#ifndef RECOMMENDATION_ENGINE_HPP
#define RECOMMENDATION_ENGINE_HPP

#include "Recipe.hpp"
#include <vector>
#include <string>
#include <unordered_set>

struct RecipeMatchResult {
    Recipe recipe;
    double matchPercentage;          // 0.0 to 100.0
    std::vector<std::string> matchedIngredients;
    std::vector<std::string> missingIngredients;
    bool isExactMatch;               // 0 missing ingredients
    bool isNearMatch;                // 1 or 2 missing ingredients AND matchPercentage >= 50.0

    // Deterministic 4-tier ranking comparator:
    // 1. Highest match percentage first
    // 2. Fewest missing ingredients first
    // 3. Fewer total required ingredients first
    // 4. Alphabetical by recipe title
    static bool compareRank(const RecipeMatchResult& a, const RecipeMatchResult& b);
};

class RecommendationEngine {
private:
    std::vector<Recipe> catalog;

public:
    RecommendationEngine();

    void addRecipe(const Recipe& recipe);
    void clearCatalog();
    size_t getCatalogSize() const;

    // Load recipes from JSON formatted string
    bool loadRecipesFromJsonString(const std::string& jsonString);

    // Recommend top recipes based on user's available ingredients
    std::vector<RecipeMatchResult> recommend(
        const std::vector<std::string>& userIngredients,
        size_t topN = 10,
        double minNearMatchThreshold = 50.0
    ) const;

    // Helper: export results to JSON string for web/bridge transmission
    static std::string resultsToJson(const std::vector<RecipeMatchResult>& results);
};

#endif // RECOMMENDATION_ENGINE_HPP
