#include "../../inc/Labels/BaseLabel.hpp"

#include <typeinfo>

void Label::setText(const std::string& text) {
    value = text;
}

bool Label::operator==(const Label& other) {
    return typeid(*this) == typeid(other) && value == other.value;
}

bool Label::operator!=(const Label& other) {
    return !(*this == other);
}