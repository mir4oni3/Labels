#include "../../inc/Transformations/CompositeTransformation.hpp"

#include <iostream>

CompositeTransformation::CompositeTransformation(const std::vector<std::shared_ptr<Transformation>>& transformations) {
    setTransformations(transformations);
}

const std::vector<std::shared_ptr<Transformation>>& CompositeTransformation::getTransformations() const {
    return transformations;
}

void CompositeTransformation::setTransformations(const std::vector<std::shared_ptr<Transformation>>& transformations) {
    for (const auto& transformation : transformations) {
        pushTransformation(transformation);
    }
}

std::string CompositeTransformation::transform(const std::string& text) const {
    std::string result = text;
    for (const auto& transformation : transformations) {
        result = transformation->transform(result);
    }
    return result;
}

void CompositeTransformation::pushTransformation(const std::shared_ptr<Transformation>& transformation) {
    insertAt(transformation, transformations.size());
}

std::shared_ptr<Transformation> CompositeTransformation::popTransformation() {
    return popAt(transformations.size() - 1);
}

std::shared_ptr<Transformation> CompositeTransformation::popAt(unsigned int index) {
    if (index >= transformations.size()) {
        throw std::out_of_range("Index out of range");
    }
    std::shared_ptr<Transformation> result = transformations[index];
    transformations.erase(transformations.begin() + index);
    return result;
}

void CompositeTransformation::insertAt(const std::shared_ptr<Transformation>& transformation, unsigned int index) {
    if (index > transformations.size()) {
        throw std::out_of_range("Index out of range");
    }
    if (!transformation) {
        throw std::invalid_argument("Transformation cannot be nullptr");
    }
    transformations.insert(transformations.begin() + index, transformation);
}