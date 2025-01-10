#include "../../inc/Transformations/ReplaceTransformation.hpp"

#include <iostream>

ReplaceTransformation::ReplaceTransformation(const std::string& from, const std::string& to) {
    setFrom(from);
    setTo(to);
}

const std::string& ReplaceTransformation::getFrom() const {
    return from;
}

void ReplaceTransformation::setFrom(const std::string& from) {
    if (from == "") {
        throw std::invalid_argument("From string cannot be empty");
    }
    this->from = from;
}

const std::string& ReplaceTransformation::getTo() const {
    return to;
}

void ReplaceTransformation::setTo(const std::string& to) {
    this->to = to;
}

bool ReplaceTransformation::operator==(const Transformation& other) const {
    try {
        const ReplaceTransformation& otherReplace = dynamic_cast<const ReplaceTransformation&>(other);
        return from == otherReplace.from && to == otherReplace.to;
    } catch (const std::bad_cast& e) {}
    
    return false;
}

std::string ReplaceTransformation::transform(const std::string& text) const {
    std::string result = text;
    size_t start = 0;
    while ((start = result.find(from, start)) != std::string::npos) {
        result.replace(start, from.length(), to);
        start += to.length();
    }
    return result;
}