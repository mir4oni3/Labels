#include "../../inc/Decorators/BaseDecorator.hpp"
#include "../../inc/Transformations/CompositeTransformation.hpp"

#include <iostream>
#include <memory>
#include <typeinfo>

BaseDecorator::BaseDecorator(const std::shared_ptr<Label>& label, const std::shared_ptr<Transformation>& trans) {
    if (!label) {
        throw std::invalid_argument("Label cannot be nullptr");
    }
    this->label = label;
    setTransformation(trans);
}

//NOTE: The returned label could be decorated.
const std::shared_ptr<Label>& BaseDecorator::getLabel() const {
    return label;
}

//Returns the base undecorated label.
const std::shared_ptr<Label>& BaseDecorator::getUnderlyingLabel() const {
    std::shared_ptr<BaseDecorator> current = std::dynamic_pointer_cast<BaseDecorator>(label);
    if (!current) {
        return label;
    }

    while (std::dynamic_pointer_cast<BaseDecorator>(current->label)) {
        current = std::dynamic_pointer_cast<BaseDecorator>(current->label);
    }

    return current->label;
}

const std::shared_ptr<Transformation>& BaseDecorator::getTransformation() const {
    return transformation;
}

void BaseDecorator::setTransformation(const std::shared_ptr<Transformation>& trans) {
    if (!trans) {
        throw std::invalid_argument("Transformation cannot be nullptr");
    }
    this->transformation = std::make_shared<CompositeTransformation>();
    this->transformation->pushTransformation(trans);
}

//To preserve invariants, decorators always need to have at least one decoration.
void BaseDecorator::removeLastDecoration() {
    std::shared_ptr<BaseDecorator> result = std::dynamic_pointer_cast<BaseDecorator>(label);

    if (!result) {
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }
    
    this->label = result->label;
    this->transformation = result->transformation;
}

int BaseDecorator::removeRecursively(std::shared_ptr<BaseDecorator>& decorator, unsigned int indexRemove, std::shared_ptr<Label>& prev) const {
    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(decorator->label);

    if (!under) {
        if (indexRemove == 0) {
            prev = decorator->label;
        }
        return 0;
    }

    int currentIndex = removeRecursively(under, indexRemove, prev) + 1;

    if (currentIndex == indexRemove - 1) {
        prev = decorator;
    }
    if (currentIndex == indexRemove + 1) {
        decorator->label = prev;
    }

    return currentIndex;
}

void BaseDecorator::removeAtIndex(unsigned int index) {
    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(label);

    if (!under) {
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }

    int currentIndex = removeRecursively(under, index, std::shared_ptr<Label>()) + 1;

    if (index > currentIndex) {
        throw std::out_of_range("Index out of range");
    }

    if (index == currentIndex) {
        removeLastDecoration();
    }
}

void BaseDecorator::removeType(const std::shared_ptr<BaseDecorator>& objTypeToRemove) {
    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(label);

    if (!under && typeid(*objTypeToRemove) == typeid(*this)) {
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }

    if (!under) {
        throw std::logic_error("No matching decoration to remove.");
    }

    std::shared_ptr<BaseDecorator> current = nullptr;

    while (under) {
        if (typeid(*under) == typeid(*objTypeToRemove)) {
            if (current) {
                current->label = under->label;
            } else {
                removeLastDecoration();
            }
            return;
        }
        current = under;
        under = std::dynamic_pointer_cast<BaseDecorator>(under->label);
    }

    throw std::logic_error("No matching decoration to remove.");
}