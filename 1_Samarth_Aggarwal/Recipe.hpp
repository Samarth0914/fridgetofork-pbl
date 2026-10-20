#ifndef FRIDGETOFORK_RECIPE_HPP
#define FRIDGETOFORK_RECIPE_HPP

#include "Ingredient.hpp"
#include <string>
#include <vector>

namespace FridgeToFork {

class Recipe {
private:
    std::string id_;
    std::string name_;
    std::vector<Ingredient> ingredients_;
    std::string instructions_;

public:
    Recipe() = default;
    Recipe(std::string id, std::string name, std::vector<Ingredient> ingredients, std::string instructions);

    const std::string& getId() const noexcept;
    const std::string& getName() const noexcept;
    const std::vector<Ingredient>& getIngredients() const noexcept;
    const std::string& getInstructions() const noexcept;
    std::size_t getTotalIngredients() const noexcept;
};

} // namespace FridgeToFork

#endif // FRIDGETOFORK_RECIPE_HPP
