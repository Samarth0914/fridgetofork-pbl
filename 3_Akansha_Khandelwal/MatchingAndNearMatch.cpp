/**
 * Team Member: Akansha Khandelwal
 */

#include "../2_Govind_Chhabra/RecommendationEngine.hpp"

namespace FridgeToFork {

std::unordered_set<std::string> RecommendationEngine::buildUserIngredientSet(
    const std::vector<std::string>& rawUserIngredients) {
    std::unordered_set<std::string> normalizedSet;
    for (const auto& item : rawUserIngredients) {
        std::string norm = Ingredient::normalize(item);
        if (!norm.empty()) {
            normalizedSet.insert(norm);
        }
    }
    return normalizedSet;
}

MatchResult RecommendationEngine::evaluateRecipe(
    const Recipe& recipe,
    const std::unordered_set<std::string>& userIngredientSet) {

    MatchResult result;
    result.recipe = recipe;
    result.totalRequired = recipe.getTotalIngredients();

    if (result.totalRequired == 0) {
        return result;
    }

    for (const auto& reqIngredient : recipe.getIngredients()) {
        const std::string& reqName = reqIngredient.getName();
        if (userIngredientSet.find(reqName) != userIngredientSet.end()) {
            result.matchedIngredients.push_back(reqName);
        } else {
            result.missingIngredients.push_back(reqName);
        }
    }

    result.matchedCount = result.matchedIngredients.size();
    result.missingCount = result.missingIngredients.size();

    result.matchPercentage =
        (static_cast<double>(result.matchedCount) * 100.0) /
        static_cast<double>(result.totalRequired);

    return result;
}

} // namespace FridgeToFork
