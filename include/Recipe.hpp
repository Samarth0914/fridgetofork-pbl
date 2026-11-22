#ifndef RECIPE_HPP
#define RECIPE_HPP

#include "Ingredient.hpp"
#include <string>
#include <vector>
#include <unordered_set>

class Recipe {
private:
    int id;
    std::string title;
    std::string category;
    int prepTimeMinutes;
    std::vector<Ingredient> requiredIngredients;
    std::unordered_set<std::string> normalizedIngredientSet;
    std::string instructions;

public:
    Recipe();
    Recipe(int id, const std::string& title, const std::string& category,
           int prepTimeMinutes, const std::vector<Ingredient>& ingredients,
           const std::string& instructions = "");

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getCategory() const;
    int getPrepTimeMinutes() const;
    const std::vector<Ingredient>& getRequiredIngredients() const;
    const std::string& getInstructions() const;

    bool requiresIngredient(const std::string& normalizedIngredient) const;
    size_t getIngredientCount() const;
};

#endif // RECIPE_HPP
