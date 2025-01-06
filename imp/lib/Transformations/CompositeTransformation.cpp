#include "../../inc/Transformations/CompositeTransformation.hpp"

#include <iostream>

CompositeTransformation::CompositeTransformation(const std::vector<std::shared_ptr<Transformation>>& transformations) {
    setTransformations(transformations);
}

const std::vector<std::shared_ptr<Transformation>>& CompositeTransformation::getTransformations() const {
    return transformations;
}

void CompositeTransformation::setTransformations(const std::vector<std::shared_ptr<Transformation>>& transformations) {
    this->transformations = transformations;
}

std::string CompositeTransformation::transform(const std::string& text) const {
    std::string result = text;
    for (const auto& transformation : transformations) {
        result = transformation->transform(result);
    }
    return result;
}

void CompositeTransformation::pushTransformation(const std::shared_ptr<Transformation>& transformation) {
    transformations.push_back(transformation);
}

std::shared_ptr<Transformation> CompositeTransformation::popTransformation() {
    if (transformations.empty()) {
        throw std::out_of_range("No transformations to pop");
    }
    std::shared_ptr<Transformation> result = transformations.back();
    transformations.pop_back();
    return result;
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
    transformations.insert(transformations.begin() + index, transformation);
}