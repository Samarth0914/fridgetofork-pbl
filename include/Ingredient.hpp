/**
 * Team Member: Samarth Aggarwal (2510010686) & Govind Chhabra (2510011653)
 */

#ifndef FRIDGETOFORK_INGREDIENT_HPP
#define FRIDGETOFORK_INGREDIENT_HPP

#include <string>

namespace FridgeToFork {

class Ingredient {
private:
    std::string name_;
    std::string category_;

public:
    Ingredient();
    explicit Ingredient(const std::string& rawName,
                        const std::string& category = "general");

    const std::string& getName() const noexcept;
    const std::string& getCategory() const noexcept;

    bool operator==(const Ingredient& other) const noexcept;
    bool operator<(const Ingredient& other) const noexcept;
};

}

#endif // FRIDGETOFORK_INGREDIENT_HPP
