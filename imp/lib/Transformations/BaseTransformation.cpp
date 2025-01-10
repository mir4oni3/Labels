#include "../../inc/Transformations/BaseTransformation.hpp"

#include <typeinfo>

bool Transformation::operator==(const Transformation& other) const {
    return typeid(*this) == typeid(other);
}

bool Transformation::operator!=(const Transformation& other) const {
    return !(*this == other);
}