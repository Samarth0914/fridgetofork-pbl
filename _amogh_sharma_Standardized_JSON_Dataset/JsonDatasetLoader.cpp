#include "JsonDatasetLoader.hpp"
#include <fstream>
#include <sstream>

namespace FridgeToFork {

bool JsonDatasetLoader::loadRecipesFromFile(const std::string& filePath,
                                            std::vector<Recipe>& outRecipes) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        return false;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    const std::string jsonText = ss.str();

    std::size_t recipesKeyPos = jsonText.find("\"recipes\"");
    if (recipesKeyPos == std::string::npos) return false;
    std::size_t arrayStart = jsonText.find('[', recipesKeyPos);
    if (arrayStart == std::string::npos) return false;

    std::size_t pos = arrayStart + 1;
    while (pos < jsonText.size()) {
        std::size_t objStart = jsonText.find('{', pos);
        if (objStart == std::string::npos) break;

        int braceDepth = 0;
        bool inQuotes = false;
        std::size_t objEnd = std::string::npos;
        for (std::size_t i = objStart; i < jsonText.size(); ++i) {
            char c = jsonText[i];
            if (c == '"' && (i == 0 || jsonText[i - 1] != '\\')) {
                inQuotes = !inQuotes;
            } else if (!inQuotes) {
                if (c == '{') braceDepth++;
                else if (c == '}') {
                    braceDepth--;
                    if (braceDepth == 0) {
                        objEnd = i;
                        break;
                    }
                }
            }
        }
        if (objEnd == std::string::npos) break;

        std::string recipeBlock = jsonText.substr(objStart, objEnd - objStart + 1);
        Recipe parsed = parseSingleRecipeObject(recipeBlock);
        if (!parsed.getId().empty() && !parsed.getName().empty() && parsed.getTotalIngredients() > 0) {
            outRecipes.push_back(parsed);
        }
        pos = objEnd + 1;
    }
    return !outRecipes.empty();
}

std::string JsonDatasetLoader::extractStringField(const std::string& block, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    std::size_t keyPos = block.find(pattern);
    if (keyPos == std::string::npos) return "";
    std::size_t colonPos = block.find(':', keyPos + pattern.size());
    if (colonPos == std::string::npos) return "";
    std::size_t firstQuote = block.find('"', colonPos + 1);
    if (firstQuote == std::string::npos) return "";
    std::size_t secondQuote = block.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) return "";
    return block.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

int JsonDatasetLoader::extractIntField(const std::string& block, const std::string& key, int defaultVal) {
    std::string pattern = "\"" + key + "\"";
    std::size_t keyPos = block.find(pattern);
    if (keyPos == std::string::npos) return defaultVal;
    std::size_t colonPos = block.find(':', keyPos + pattern.size());
    if (colonPos == std::string::npos) return defaultVal;
    std::size_t numStart = block.find_first_of("0123456789", colonPos + 1);
    if (numStart == std::string::npos) return defaultVal;
    std::size_t numEnd = block.find_first_not_of("0123456789", numStart);
    return std::stoi(block.substr(numStart, numEnd - numStart));
}

Recipe JsonDatasetLoader::parseSingleRecipeObject(const std::string& block) {
    std::size_t ingKeyPos = block.find("\"ingredients\"");
    std::string headerPart = (ingKeyPos != std::string::npos) ? block.substr(0, ingKeyPos) : block;

    std::string id = extractStringField(headerPart, "id");
    std::string name = extractStringField(headerPart, "name");
    std::string category = extractStringField(headerPart, "category");
    int prepTime = extractIntField(headerPart, "prep_time_minutes", 15);

    Recipe recipe(id, name, category.empty() ? "General" : category, prepTime);

    if (ingKeyPos != std::string::npos) {
        std::size_t arrStart = block.find('[', ingKeyPos);
        std::size_t arrEnd = block.find(']', arrStart);
        if (arrStart != std::string::npos && arrEnd != std::string::npos) {
            std::string ingArrayStr = block.substr(arrStart + 1, arrEnd - arrStart - 1);
            std::size_t p = 0;
            while (true) {
                std::size_t oStart = ingArrayStr.find('{', p);
                if (oStart == std::string::npos) break;
                std::size_t oEnd = ingArrayStr.find('}', oStart);
                if (oEnd == std::string::npos) break;
                std::string ingObj = ingArrayStr.substr(oStart, oEnd - oStart + 1);
                std::string rawIngName = extractStringField(ingObj, "name");
                std::string ingCat = extractStringField(ingObj, "category");
                if (!rawIngName.empty()) {
                    recipe.addIngredient(Ingredient(rawIngName, ingCat.empty() ? "general" : ingCat));
                }
                p = oEnd + 1;
            }
        }
    }
    return recipe;
}

}
