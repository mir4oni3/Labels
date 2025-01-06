#include "../../inc/Transformations/SimpleTransformations.hpp"

std::string CapitalizeTransformation::transform(const std::string& text) const {
    std::string result = text;
    if (!result.empty()) {
        result[0] = std::toupper(result[0]);
    }
    return result;
}

std::string LeftTrimTransformation::transform(const std::string& text) const {
    size_t start = text.find_first_not_of(" \t\n");
    return (start == std::string::npos) ? "" : text.substr(start);
}

std::string RightTrimTransformation::transform(const std::string& text) const {
    size_t end = text.find_last_not_of(" \t\n");
    return (end == std::string::npos) ? "" : text.substr(0, end + 1);
}

std::string NormalizeSpaceTransformation::transform(const std::string& text) const {
    std::string result;
    bool inSpace = false;
    for (char c : text) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (!inSpace) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
        }
    }
    return result;
}

std::string DecorateTransformation::transform(const std::string& text) const {
    return "-={ " + text + " }=-";
}