#ifndef INGREDIENT_HPP
#define INGREDIENT_HPP

#include <string>
#include <algorithm>
#include <cctype>

class Ingredient {
private:
    std::string name;
    std::string category;

public:
    Ingredient();
    explicit Ingredient(const std::string& name, const std::string& category = "general");

    const std::string& getName() const;
    const std::string& getCategory() const;

    void setName(const std::string& name);
    void setCategory(const std::string& category);

    // Normalizes input to trimmed, lowercase singular format
    static std::string normalize(const std::string& rawName);

    bool operator==(const Ingredient& other) const;
    bool operator<(const Ingredient& other) const;
};

#endif // INGREDIENT_HPP
