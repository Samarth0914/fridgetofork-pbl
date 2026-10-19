/**
 * Team Members: Samarth Aggarwal (2510010686) & Govind Chhabra (2510011653)
 */

#include "Ingredient.hpp"
#include "../4_Amogh_Sharma/Standardizer.hpp"

namespace FridgeToFork {

Ingredient::Ingredient()
    : name_(""), category_("general") {}

Ingredient::Ingredient(const std::string& rawName, const std::string& category)
    : name_(Standardization::normalizeIngredientName(rawName)),
      category_(Standardization::trimAndCollapseSpaces(category)) {}

const std::string& Ingredient::getName() const noexcept {
    return name_;
}

const std::string& Ingredient::getCategory() const noexcept {
    return category_;
}

bool Ingredient::operator==(const Ingredient& other) const noexcept {
    return name_ == other.name_;
}

bool Ingredient::operator<(const Ingredient& other) const noexcept {
    return name_ < other.name_;
}

}
