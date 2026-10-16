/**
 * Team Member: Govind Chhabra (2510011653)
 */

#include "RecommendationEngine.hpp"
#include "../4_Amogh_Sharma/JsonDatasetLoader.hpp"
#include <algorithm>

namespace FridgeToFork {

void RecommendationEngine::addRecipe(const Recipe& recipe) {
    if (recipe.getId().empty() || recipe.getName().empty() || recipe.getTotalIngredients() == 0) {
        return;
    }
    catalog_.push_back(recipe);
    for (const auto& ing : recipe.getIngredients()) {
        knownIngredientVocabulary_.insert(ing.getName());
    }
}

bool RecommendationEngine::loadCatalogFromJson(const std::string& filePath) {
    std::vector<Recipe> loaded;
    if (!JsonDatasetLoader::loadRecipesFromFile(filePath, loaded)) {
        return false;
    }
    for (const auto& r : loaded) {
        addRecipe(r);
    }
    return !catalog_.empty();
}

std::size_t RecommendationEngine::getCatalogSize() const noexcept {
    return catalog_.size();
}

std::vector<std::string> RecommendationEngine::getAllCatalogIngredients() const {
    std::vector<std::string> sortedList(knownIngredientVocabulary_.begin(),
                                        knownIngredientVocabulary_.end());
    std::sort(sortedList.begin(), sortedList.end());
    return sortedList;
}

} // namespace FridgeToFork
