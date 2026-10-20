#include "Recipe.hpp"

namespace FridgeToFork {

Recipe::Recipe(std::string id, std::string name, std::vector<Ingredient> ingredients, std::string instructions)
    : id_(std::move(id)), name_(std::move(name)), ingredients_(std::move(ingredients)), instructions_(std::move(instructions)) {}

const std::string& Recipe::getId() const noexcept { return id_; }
const std::string& Recipe::getName() const noexcept { return name_; }
const std::vector<Ingredient>& Recipe::getIngredients() const noexcept { return ingredients_; }
const std::string& Recipe::getInstructions() const noexcept { return instructions_; }
std::size_t Recipe::getTotalIngredients() const noexcept { return ingredients_.size(); }

} // namespace FridgeToFork
