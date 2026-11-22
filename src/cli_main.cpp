#include "../include/RecommendationEngine.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void printBanner() {
    std::cout << "============================================================" << std::endl;
    std::cout << "   FridgeToFork: C++ Recipe Recommender (Team T141)        " << std::endl;
    std::cout << "   Graphic Era (Deemed to be University) - CS&E Phase-III   " << std::endl;
    std::cout << "============================================================" << std::endl;
}

std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    printBanner();

    RecommendationEngine engine;

    // Attempt to load from data/recipes.json
    std::string jsonPath = "data/recipes.json";
    if (argc > 1 && std::string(argv[1]).find(".json") != std::string::npos) {
        jsonPath = argv[1];
    }

    std::string jsonData = readFile(jsonPath);
    if (!jsonData.empty() && engine.loadRecipesFromJsonString(jsonData)) {
        std::cout << "[✓] Loaded " << engine.getCatalogSize() << " recipes from " << jsonPath << std::endl;
    } else {
        std::cout << "[!] Could not load external JSON. Loading built-in default recipes..." << std::endl;
        engine.addRecipe(Recipe(1, "Scrambled Eggs", "Breakfast", 10,
            { Ingredient("egg"), Ingredient("butter"), Ingredient("milk") },
            "Whisk eggs with milk, cook on low heat with butter."));
        engine.addRecipe(Recipe(2, "French Toast", "Breakfast", 15,
            { Ingredient("bread"), Ingredient("egg"), Ingredient("milk"), Ingredient("butter") },
            "Dip bread in egg-milk batter, toast in butter."));
        engine.addRecipe(Recipe(3, "Grilled Cheese", "Snack", 10,
            { Ingredient("bread"), Ingredient("cheese"), Ingredient("butter") },
            "Place cheese between bread slices, butter outside, pan fry."));
        engine.addRecipe(Recipe(4, "Garlic Butter Pasta", "Dinner", 20,
            { Ingredient("pasta"), Ingredient("garlic"), Ingredient("butter"), Ingredient("cheese") },
            "Boil pasta, sauté garlic in butter, toss together with cheese."));
    }

    std::vector<std::string> userIngredients;

    // Check if arguments were passed via command line
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find(".json") == std::string::npos) {
            userIngredients.push_back(arg);
        }
    }

    // Interactive input mode if no ingredient arguments given
    if (userIngredients.empty()) {
        std::cout << "\nEnter ingredients you have (separated by spaces or commas):" << std::endl;
        std::cout << "Example: egg butter bread milk" << std::endl;
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);

        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            if (token.empty()) continue;
            // Trim comma if present
            if (token.back() == ',') token.pop_back();
            if (!token.empty()) userIngredients.push_back(token);
        }
    }

    std::cout << "\nSelected ingredients (" << userIngredients.size() << "): ";
    for (size_t i = 0; i < userIngredients.size(); ++i) {
        std::cout << userIngredients[i] << (i + 1 < userIngredients.size() ? ", " : "");
    }
    std::cout << "\nSearching recipe catalog...\n" << std::endl;

    auto results = engine.recommend(userIngredients, 5);

    if (results.empty()) {
        std::cout << "[-] No exact or qualifying near-match (>=50%) recipes found." << std::endl;
        std::cout << "    Tip: Try adding common kitchen staples like butter, bread, or egg.\n" << std::endl;
        return 0;
    }

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "                      TOP RECOMMENDATIONS                   " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];
        std::cout << (i + 1) << ". " << r.recipe.getTitle() 
                  << " [" << r.recipe.getCategory() << " | " << r.recipe.getPrepTimeMinutes() << " mins]" << std::endl;
        std::cout << "   Match Score : " << r.matchPercentage << "% " 
                  << (r.isExactMatch ? "(★ Cook Now - 100%)" : "(✦ Near Match)") << std::endl;
        
        std::cout << "   Matched     : ";
        for (size_t j = 0; j < r.matchedIngredients.size(); ++j) {
            std::cout << r.matchedIngredients[j] << (j + 1 < r.matchedIngredients.size() ? ", " : "");
        }
        std::cout << std::endl;

        if (!r.missingIngredients.empty()) {
            std::cout << "   Missing     : [ ";
            for (size_t j = 0; j < r.missingIngredients.size(); ++j) {
                std::cout << r.missingIngredients[j] << (j + 1 < r.missingIngredients.size() ? ", " : "");
            }
            std::cout << " ]" << std::endl;
        }

        if (!r.recipe.getInstructions().empty()) {
            std::cout << "   Instructions: " << r.recipe.getInstructions() << std::endl;
        }
        std::cout << "------------------------------------------------------------" << std::endl;
    }

    return 0;
}
