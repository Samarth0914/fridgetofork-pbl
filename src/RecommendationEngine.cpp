#include "../include/RecommendationEngine.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>
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

// Lightweight JSON helper to parse string fields
static std::string extractJsonString(const std::string& block, const std::string& key) {
    std::string needle = "\"" + key + "\"";
    size_t pos = block.find(needle);
    if (pos == std::string::npos) return "";
    pos = block.find(':', pos);
    if (pos == std::string::npos) return "";
    pos = block.find('"', pos);
    if (pos == std::string::npos) return "";
    size_t endPos = block.find('"', pos + 1);
    if (endPos == std::string::npos) return "";
    return block.substr(pos + 1, endPos - pos - 1);
}

// Lightweight JSON helper to parse int fields
static int extractJsonInt(const std::string& block, const std::string& key, int defaultVal = 0) {
    std::string needle = "\"" + key + "\"";
    size_t pos = block.find(needle);
    if (pos == std::string::npos) return defaultVal;
    pos = block.find(':', pos);
    if (pos == std::string::npos) return defaultVal;
    while (pos < block.size() && (block[pos] == ':' || block[pos] == ' ' || block[pos] == '\t')) pos++;
    size_t endPos = pos;
    while (endPos < block.size() && (std::isdigit(block[endPos]) || block[endPos] == '-')) endPos++;
    try {
        return std::stoi(block.substr(pos, endPos - pos));
    } catch (...) {
        return defaultVal;
    }
}

// Lightweight JSON helper to parse string arrays
static std::vector<std::string> extractJsonStringArray(const std::string& block, const std::string& key) {
    std::vector<std::string> list;
    std::string needle = "\"" + key + "\"";
    size_t pos = block.find(needle);
    if (pos == std::string::npos) return list;
    pos = block.find('[', pos);
    if (pos == std::string::npos) return list;
    size_t endBracket = block.find(']', pos);
    if (endBracket == std::string::npos) return list;

    std::string arrayContent = block.substr(pos + 1, endBracket - pos - 1);
    size_t i = 0;
    while (i < arrayContent.size()) {
        size_t quoteStart = arrayContent.find('"', i);
        if (quoteStart == std::string::npos) break;
        size_t quoteEnd = arrayContent.find('"', quoteStart + 1);
        if (quoteEnd == std::string::npos) break;
        list.push_back(arrayContent.substr(quoteStart + 1, quoteEnd - quoteStart - 1));
        i = quoteEnd + 1;
    }
    return list;
}

bool RecommendationEngine::loadRecipesFromJsonString(const std::string& jsonString) {
    if (jsonString.empty()) return false;
    size_t pos = 0;
    int loaded = 0;

    while ((pos = jsonString.find('{', pos)) != std::string::npos) {
        int openBraces = 0;
        size_t cur = pos;
        size_t endPos = std::string::npos;
        while (cur < jsonString.size()) {
            if (jsonString[cur] == '{') openBraces++;
            else if (jsonString[cur] == '}') {
                openBraces--;
                if (openBraces == 0) {
                    endPos = cur;
                    break;
                }
            }
            cur++;
        }
        if (endPos == std::string::npos) break;

        std::string block = jsonString.substr(pos, endPos - pos + 1);
        int id = extractJsonInt(block, "id", loaded + 1);
        std::string title = extractJsonString(block, "title");
        std::string category = extractJsonString(block, "category");
        if (category.empty()) category = "General";
        int prepTime = extractJsonInt(block, "prep_time", 15);
        std::string instructions = extractJsonString(block, "instructions");

        auto rawIngredients = extractJsonStringArray(block, "ingredients");
        if (!title.empty() && !rawIngredients.empty()) {
            std::vector<Ingredient> ings;
            for (const auto& item : rawIngredients) {
                ings.emplace_back(item);
            }
            addRecipe(Recipe(id, title, category, prepTime, ings, instructions));
            loaded++;
        }

        pos = endPos + 1;
    }

    return loaded > 0;
}

std::string RecommendationEngine::resultsToJson(const std::vector<RecipeMatchResult>& results) {
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];
        ss << "{";
        ss << "\"id\":" << r.recipe.getId() << ",";
        ss << "\"title\":\"" << r.recipe.getTitle() << "\",";
        ss << "\"category\":\"" << r.recipe.getCategory() << "\",";
        ss << "\"prep_time\":" << r.recipe.getPrepTimeMinutes() << ",";
        ss << "\"match_percentage\":" << std::fixed << std::setprecision(1) << r.matchPercentage << ",";
        ss << "\"is_exact\":" << (r.isExactMatch ? "true" : "false") << ",";
        ss << "\"is_near\":" << (r.isNearMatch ? "true" : "false") << ",";
        
        ss << "\"matched_ingredients\":[";
        for (size_t j = 0; j < r.matchedIngredients.size(); ++j) {
            ss << "\"" << r.matchedIngredients[j] << "\"";
            if (j + 1 < r.matchedIngredients.size()) ss << ",";
        }
        ss << "],";

        ss << "\"missing_ingredients\":[";
        for (size_t j = 0; j < r.missingIngredients.size(); ++j) {
            ss << "\"" << r.missingIngredients[j] << "\"";
            if (j + 1 < r.missingIngredients.size()) ss << ",";
        }
        ss << "],";

        std::string inst = r.recipe.getInstructions();
        std::string escapedInst = "";
        for (char c : inst) {
            if (c == '"') escapedInst += "\\\"";
            else if (c == '\\') escapedInst += "\\\\";
            else escapedInst += c;
        }
        ss << "\"instructions\":\"" << escapedInst << "\"";
        ss << "}";
        if (i + 1 < results.size()) ss << ",";
    }
    ss << "]";
    return ss.str();
}
