#include "../include/Recipe.hpp"

Recipe::Recipe() : id(0), title(""), category(""), prepTimeMinutes(0), instructions("") {}

Recipe::Recipe(int id, const std::string& title, const std::string& category,
               int prepTimeMinutes, const std::vector<Ingredient>& ingredients,
               const std::string& instructions)
    : id(id), title(title), category(category),
      prepTimeMinutes(prepTimeMinutes), requiredIngredients(ingredients),
      instructions(instructions) {
    for (const auto& ing : requiredIngredients) {
        normalizedIngredientSet.insert(ing.getName());
    }
}

int Recipe::getId() const {
    return id;
}

const std::string& Recipe::getTitle() const {
    return title;
}

const std::string& Recipe::getCategory() const {
    return category;
}

int Recipe::getPrepTimeMinutes() const {
    return prepTimeMinutes;
}

const std::vector<Ingredient>& Recipe::getRequiredIngredients() const {
    return requiredIngredients;
}

const std::string& Recipe::getInstructions() const {
    return instructions;
}

bool Recipe::requiresIngredient(const std::string& normalizedIngredient) const {
    return normalizedIngredientSet.find(normalizedIngredient) != normalizedIngredientSet.end();
}

size_t Recipe::getIngredientCount() const {
    return requiredIngredients.size();
}
