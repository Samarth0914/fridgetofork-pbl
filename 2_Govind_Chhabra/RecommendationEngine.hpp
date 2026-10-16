/**
 * Team Member: Govind Chhabra (2510011653) & Samarth Aggarwal (2510010686)
 */

#ifndef FRIDGETOFORK_RECOMMENDATION_ENGINE_HPP
#define FRIDGETOFORK_RECOMMENDATION_ENGINE_HPP

#include "../1_Samarth_Aggarwal/Recipe.hpp"
#include "../3_Akansha_Khandelwal/MatchTypes.hpp"
#include <cstddef>
#include <string>
#include <unordered_set>
#include <vector>

namespace FridgeToFork {

class RecommendationEngine {
private:
    std::vector<Recipe> catalog_;
    std::unordered_set<std::string> knownIngredientVocabulary_;
    static constexpr double kEpsilon = 1e-6;

public:
    RecommendationEngine() = default;

    void addRecipe(const Recipe& recipe);
    bool loadCatalogFromJson(const std::string& filePath);
    std::size_t getCatalogSize() const noexcept;
    std::vector<std::string> getAllCatalogIngredients() const;

    static std::unordered_set<std::string> buildUserIngredientSet(
        const std::vector<std::string>& rawUserIngredients);

    static MatchResult evaluateRecipe(
        const Recipe& recipe,
        const std::unordered_set<std::string>& userIngredientSet);

    static bool compareMatchResults(const MatchResult& a, const MatchResult& b) noexcept;

    std::vector<MatchResult> recommendTopRecipes(
        const std::vector<std::string>& rawUserIngredients,
        std::size_t maxResults = 10) const;
};

} // namespace FridgeToFork

#endif // FRIDGETOFORK_RECOMMENDATION_ENGINE_HPP
