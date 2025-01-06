#include "../../inc/Decorators/BaseDecorator.hpp"
#include "../../inc/Transformations/CompositeTransformation.hpp"

#include <iostream>
#include <memory>

void BaseDecorator::updateLayer(const std::shared_ptr<Label>& label) {
 std::shared_ptr<BaseDecorator> labelDecorator = std::dynamic_pointer_cast<BaseDecorator>(label);
    if (labelDecorator) {
        this->layer = labelDecorator->layer + 1;
    } else {
        this->layer = 0;
    }
}

BaseDecorator::BaseDecorator(const std::shared_ptr<Label>& label, const std::shared_ptr<Transformation>& trans) {
    if (!label) {
        throw std::invalid_argument("Label cannot be nullptr");
    }
    this->label = label;
    setTransformation(trans);
    updateLayer(label);
}

//NOTE: The returned label could be decorated.
const std::shared_ptr<Label>& BaseDecorator::getLabel() const {
    return label;
}

//Returns the base undecorated label.
const std::shared_ptr<Label>& BaseDecorator::getUnderlyingLabel() const {
    if (this->layer == 0) {
        return label;
    }

    std::shared_ptr<BaseDecorator> current = std::dynamic_pointer_cast<BaseDecorator>(label);
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
    if (this->layer == 0) {
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }
    std::shared_ptr<BaseDecorator> result = std::dynamic_pointer_cast<BaseDecorator>(label);
    this->label = result->label;
    this->transformation = result->transformation;
    this->layer = result->layer;
}

void BaseDecorator::removeAtIndex(unsigned int index) {
    if (this->layer == 0) {
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }

    if (index == this->layer) {
        removeLastDecoration();
        return;
    }

    if (index > this->layer) {
        throw std::runtime_error(
            "Tried removal of a decoration at an index"
            " greater than the current decorations."
        );
    }

    this->layer--;

    //Find the decoration right after the one we want to remove.
    std::shared_ptr<BaseDecorator> current = std::dynamic_pointer_cast<BaseDecorator>(label);
    current->layer--;
    while (current->layer != index) {
       current = std::dynamic_pointer_cast<BaseDecorator>(current->label);
       current->layer--;
    }

    /////////////////////////////////////
    if (index == 0) {
        current->label = current->getUnderlyingLabel();
    }

    for (unsigned int i = 1; i < index; i++) {
        std::shared_ptr<BaseDecorator> next = std::dynamic_pointer_cast<BaseDecorator>(current->label);
        if (!next) {
            std::cout << "WARNING: Tried removal of a decoration at an index, but there are fewer decorations. No action performed. Returning nullptr..." << std::endl;
            return nullptr;
        }
        current = next;
    }

    std::shared_ptr<BaseDecorator> result = std::dynamic_pointer_cast<BaseDecorator>(current->label);
    current->label = result->label;
    return result;
}