/**
 * Team Member: Akansha Khandelwal
 */

#ifndef FRIDGETOFORK_MATCH_TYPES_HPP
#define FRIDGETOFORK_MATCH_TYPES_HPP

#include "../1_Samarth_Aggarwal/Recipe.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace FridgeToFork {

enum class MatchCategory {
    COOK_NOW,
    NEAR_MATCH,
    LOW_MATCH
};

struct MatchResult {
    Recipe recipe;
    std::size_t matchedCount = 0;
    std::size_t missingCount = 0;
    std::size_t totalRequired = 0;
    double matchPercentage = 0.0;
    MatchCategory category = MatchCategory::LOW_MATCH;
    std::vector<std::string> matchedIngredients;
    std::vector<std::string> missingIngredients;

    bool isCookNow() const noexcept {
        return category == MatchCategory::COOK_NOW;
    }

    bool isNearMatch() const noexcept {
        return category == MatchCategory::NEAR_MATCH;
    }

    std::string getCategoryLabel() const {
        switch (category) {
            case MatchCategory::COOK_NOW:
                return "Cook Now (100% Match)";
            case MatchCategory::NEAR_MATCH:
                return "Near-Match (Missing " + std::to_string(missingCount) + ")";
            default:
                return "Low Match";
        }
    }
};

} // namespace FridgeToFork

#endif // FRIDGETOFORK_MATCH_TYPES_HPP
