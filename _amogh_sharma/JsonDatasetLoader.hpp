#ifndef FRIDGETOFORK_JSON_DATASET_LOADER_HPP
#define FRIDGETOFORK_JSON_DATASET_LOADER_HPP

#include "../1_Samarth_Aggarwal_M1_CoreModel_and_CLI_Main/Recipe.hpp"
#include <string>
#include <vector>

namespace FridgeToFork {

class JsonDatasetLoader {
public:
    static bool loadRecipesFromFile(const std::string& filePath,
                                    std::vector<Recipe>& outRecipes);

private:
    static std::string extractStringField(const std::string& block, const std::string& key);
    static int extractIntField(const std::string& block, const std::string& key, int defaultVal = 15);
    static Recipe parseSingleRecipeObject(const std::string& block);
};

}

#endif
