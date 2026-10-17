#ifndef FRIDGETOFORK_STANDARDIZER_HPP
#define FRIDGETOFORK_STANDARDIZER_HPP

#include <cctype>
#include <string>

namespace FridgeToFork {
namespace Standardization {

inline std::string trimAndCollapseSpaces(const std::string& raw) {
    std::string result;
    bool inSpace = true;
    for (unsigned char ch : raw) {
        if (std::isspace(ch)) {
            if (!inSpace) { result.push_back(' '); inSpace = true; }
        } else {
            result.push_back(static_cast<char>(ch));
            inSpace = false;
        }
    }
    if (!result.empty() && result.back() == ' ') result.pop_back();
    return result;
}

inline std::string normalizeIngredientName(const std::string& rawName) {
    std::string cleaned = trimAndCollapseSpaces(rawName);
    for (char& ch : cleaned) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return cleaned;
}

}
}

#endif
