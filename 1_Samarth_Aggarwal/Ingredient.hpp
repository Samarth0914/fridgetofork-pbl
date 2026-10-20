#ifndef FRIDGETOFORK_INGREDIENT_HPP
#define FRIDGETOFORK_INGREDIENT_HPP

#include <string>

namespace FridgeToFork {

class Ingredient {
private:
    std::string id_;
    std::string name_;
    std::string category_;

public:
    Ingredient() = default;
    Ingredient(std::string id, std::string name, std::string category);

    const std::string& getId() const noexcept;
    const std::string& getName() const noexcept;
    const std::string& getCategory() const noexcept;

    static std::string normalize(const std::string& input);
};

} // namespace FridgeToFork

#endif // FRIDGETOFORK_INGREDIENT_HPP
