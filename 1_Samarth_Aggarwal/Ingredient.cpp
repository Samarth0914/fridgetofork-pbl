#include "Ingredient.hpp"
#include <algorithm>
#include <cctype>

namespace FridgeToFork {

Ingredient::Ingredient(std::string id, std::string name, std::string category)
    : id_(std::move(id)), name_(std::move(name)), category_(std::move(category)) {}

const std::string& Ingredient::getId() const noexcept { return id_; }
const std::string& Ingredient::getName() const noexcept { return name_; }
const std::string& Ingredient::getCategory() const noexcept { return category_; }

std::string Ingredient::normalize(const std::string& input) {
    std::string result = input;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), result.end());

    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return result;
}

} // namespace FridgeToFork
