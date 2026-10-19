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

} // namespace FridgeToFork
