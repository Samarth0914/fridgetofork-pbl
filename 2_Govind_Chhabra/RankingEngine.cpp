/**
 * Team Member: Govind Chhabra (2510011653)
 */

#include "RecommendationEngine.hpp"
#include "../4_Amogh_Sharma/JsonDatasetLoader.hpp"
#include <algorithm>
#include <cmath>

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

bool RecommendationEngine::compareMatchResults(const MatchResult& a, const MatchResult& b) noexcept {
    if (std::fabs(a.matchPercentage - b.matchPercentage) > kEpsilon) {
        return a.matchPercentage > b.matchPercentage;
    }
    if (a.recipe.getName() != b.recipe.getName()) {
        return a.recipe.getName() < b.recipe.getName();
    }
    return a.recipe.getId() < b.recipe.getId();
}

std::vector<MatchResult> RecommendationEngine::recommendTopRecipes(
    const std::vector<std::string>& rawUserIngredients,
    std::size_t maxResults) const {

    std::unordered_set<std::string> userSet = buildUserIngredientSet(rawUserIngredients);
    if (userSet.empty() || catalog_.empty() || maxResults == 0) {
        return {};
    }

    std::vector<MatchResult> candidates;
    candidates.reserve(catalog_.size());

    for (const auto& recipe : catalog_) {
        MatchResult res = evaluateRecipe(recipe, userSet);

        if (res.matchedCount > 0 && (res.isCookNow() || res.isNearMatch())) {
            candidates.push_back(std::move(res));
        }
    }

    const std::size_t k = std::min(maxResults, candidates.size());
    std::partial_sort(candidates.begin(),
                      candidates.begin() + static_cast<std::ptrdiff_t>(k),
                      candidates.end(),
                      compareMatchResults);

    candidates.resize(k);
    return candidates;
}

} // namespace FridgeToFork
