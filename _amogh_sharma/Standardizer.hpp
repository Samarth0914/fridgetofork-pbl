#ifndef FRIDGETOFORK_STANDARDIZER_HPP
#define FRIDGETOFORK_STANDARDIZER_HPP

#include <cctype>
#include <string>
#include <unordered_map>

namespace FridgeToFork {
namespace Standardization {

inline std::string trimAndCollapseSpaces(const std::string& raw) {
    std::string result;
    result.reserve(raw.size());
    bool inSpace = true;
    for (unsigned char ch : raw) {
        if (std::isspace(ch)) {
            if (!inSpace) {
                result.push_back(' ');
                inSpace = true;
            }
        } else {
            result.push_back(static_cast<char>(ch));
            inSpace = false;
        }
    }
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    return result;
}

inline std::string normalizeIngredientName(const std::string& rawName) {
    std::string cleaned = trimAndCollapseSpaces(rawName);
    for (char& ch : cleaned) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    if (cleaned.empty()) {
        return cleaned;
    }

    static const std::unordered_map<std::string, std::string> kPluralExceptions = {
        {"tomatoes", "tomato"},
        {"potatoes", "potato"},
        {"chilies", "chili"},
        {"chillies", "chili"},
        {"green chilies", "green chili"},
        {"green chillies", "green chili"},
        {"leaves", "leaf"},
        {"basil leaves", "basil"},
        {"loaves", "loaf"},
        {"halves", "half"}
    };
    auto it = kPluralExceptions.find(cleaned);
    if (it != kPluralExceptions.end()) {
        return it->second;
    }

    if (cleaned.size() > 3 && cleaned.back() == 's' && cleaned[cleaned.size() - 2] != 's') {
        if (cleaned.size() > 4 && cleaned.substr(cleaned.size() - 3) == "ies") {
            return cleaned.substr(0, cleaned.size() - 3) + "y";
        }
        return cleaned.substr(0, cleaned.size() - 1);
    }
    return cleaned;
}

}
}

#endif
