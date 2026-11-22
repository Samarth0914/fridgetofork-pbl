#include "../include/RecommendationEngine.hpp"
#include <algorithm>
#include <cmath>

bool RecipeMatchResult::compareRank(const RecipeMatchResult& a, const RecipeMatchResult& b) {
    // 1. Highest match percentage first
    if (std::abs(a.matchPercentage - b.matchPercentage) > 0.001) {
        return a.matchPercentage > b.matchPercentage;
    }
    // 2. Fewest missing ingredients first (tie-breaker 1)
    if (a.missingIngredients.size() != b.missingIngredients.size()) {
        return a.missingIngredients.size() < b.missingIngredients.size();
    }
    // 3. Simpler recipes with fewer total required ingredients first (tie-breaker 2)
    if (a.recipe.getIngredientCount() != b.recipe.getIngredientCount()) {
        return a.recipe.getIngredientCount() < b.recipe.getIngredientCount();
    }
    // 4. Stable alphabetical sort by recipe title (tie-breaker 3)
    return a.recipe.getTitle() < b.recipe.getTitle();
}

RecommendationEngine::RecommendationEngine() {}

void RecommendationEngine::addRecipe(const Recipe& recipe) {
    catalog.push_back(recipe);
}

void RecommendationEngine::clearCatalog() {
    catalog.clear();
}

size_t RecommendationEngine::getCatalogSize() const {
    return catalog.size();
}

std::vector<RecipeMatchResult> RecommendationEngine::recommend(
    const std::vector<std::string>& userIngredients,
    size_t topN,
    double minNearMatchThreshold
) const {
    std::vector<RecipeMatchResult> results;
    if (userIngredients.empty() || catalog.empty()) {
        return results;
    }

    // Deduplicate and normalize user input
    std::unordered_set<std::string> userSet;
    for (const auto& raw : userIngredients) {
        std::string norm = Ingredient::normalize(raw);
        if (!norm.empty()) {
            userSet.insert(norm);
        }
    }

    if (userSet.empty()) {
        return results;
    }

    // Evaluate each recipe in catalog
    for (const auto& recipe : catalog) {
        const auto& req = recipe.getRequiredIngredients();
        if (req.empty()) continue;

        std::vector<std::string> matched;
        std::vector<std::string> missing;

        for (const auto& ing : req) {
            if (userSet.find(ing.getName()) != userSet.end()) {
                matched.push_back(ing.getName());
            } else {
                missing.push_back(ing.getName());
            }
        }

        double matchPct = (static_cast<double>(matched.size()) / static_cast<double>(req.size())) * 100.0;
        bool isExact = missing.empty();
        bool isNear = (!isExact && missing.size() <= 2 && matchPct >= minNearMatchThreshold);

        // Retain only valid exact matches and qualifying near-matches (M4)
        if (isExact || isNear) {
            RecipeMatchResult res;
            res.recipe = recipe;
            res.matchPercentage = matchPct;
            res.matchedIngredients = matched;
            res.missingIngredients = missing;
            res.isExactMatch = isExact;
            res.isNearMatch = isNear;
            results.push_back(res);
        }
    }

    // Deterministic 4-level sorting
    std::sort(results.begin(), results.end(), RecipeMatchResult::compareRank);

    // Limit to topN results
    if (results.size() > topN) {
        results.resize(topN);
    }

    return results;
}
