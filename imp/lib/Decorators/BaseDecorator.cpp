#include "../../inc/Decorators/BaseDecorator.hpp"
#include "../../inc/Transformations/CompositeTransformation.hpp"

#include <iostream>
#include <memory>
#include <typeinfo>

static int insertRecursively(std::shared_ptr<BaseDecorator>& decorator, unsigned int indexInsert, const std::shared_ptr<BaseDecorator>& toInsert) {
    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(decorator->getLabel());
    int currentIndex;

    if (!under && indexInsert == 0) {
        std::shared_ptr<BaseDecorator> newDec = std::make_shared<BaseDecorator>(decorator->getLabel(), toInsert->getTransformation());
        decorator->setLabel(newDec);
    }

    if (!under) {
        currentIndex = 0;
    }
    else {
        currentIndex = insertRecursively(under, indexInsert, toInsert) + 1;
    }


    if (currentIndex == indexInsert - 1) {
        decorator->pushDecoration(toInsert);
    }

    return currentIndex;
}

static int removeRecursively(std::shared_ptr<BaseDecorator>& decorator, unsigned int indexRemove, std::shared_ptr<Label>& prev) {
    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(decorator->getLabel());

    if (!under) {
        if (indexRemove == 0) {
            prev = decorator->getLabel();
        }
        return 0;
    }

    int currentIndex = removeRecursively(under, indexRemove, prev) + 1;

    if (currentIndex == indexRemove - 1) {
        prev = decorator;
    }
    if (currentIndex == indexRemove + 1) {
        decorator->setLabel(prev);
    }

    return currentIndex;
}

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

void BaseDecorator::setLabel(const std::shared_ptr<Label>& other) {
    this->label = other;
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

bool BaseDecorator::operator==(const BaseDecorator& other) const {
    return typeid(*this) == typeid(other) && *label == *other.label && *transformation == *other.transformation;
}

void BaseDecorator::removeDecorator(const std::shared_ptr<BaseDecorator>& objToRemove, const std::function<bool(const BaseDecorator&, const BaseDecorator&)>& comp) {
    if (!objToRemove) {
        throw std::invalid_argument("Type to remove cannot be nullptr");
    }

    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(label);
    
    if (!under && typeid(*objToRemove) == typeid(*this)) {
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
        if (comp(*under, *objToRemove)) {
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

void BaseDecorator::removeType(const std::shared_ptr<BaseDecorator>& objTypeToRemove) {
    removeDecorator(objTypeToRemove, [](const BaseDecorator& a, const BaseDecorator& b) {
        return typeid(a) == typeid(b);
    });
}

void BaseDecorator::removeSpecific(const std::shared_ptr<BaseDecorator>& objToRemove) {
    removeDecorator(objToRemove, [](const BaseDecorator& a, const BaseDecorator& b) {
        return a == b;
    });
}

void BaseDecorator::pushDecoration(const std::shared_ptr<BaseDecorator>& decorator) {
    if (!decorator) {
        throw std::invalid_argument("Decoration cannot be nullptr");
    }

    std::shared_ptr<BaseDecorator> newUnder = std::make_shared<BaseDecorator>(label, transformation);
    label = newUnder;
    transformation = decorator->transformation;
}

void BaseDecorator::insertDecoration(unsigned int index, const std::shared_ptr<BaseDecorator>& decorator) {
    if (!decorator) {
        throw std::invalid_argument("Decoration cannot be nullptr");
    }

    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(label);

    if (!under) {
        if (index == 0) {
            std::shared_ptr<BaseDecorator> newUnder = std::make_shared<BaseDecorator>(label, decorator->getTransformation());
            label = newUnder;
            return;
        } else {
            throw std::out_of_range("Index out of range");
        }
    }

    int currentIndex = insertRecursively(under, index, decorator) + 1;

    if (index == currentIndex) {
        under->pushDecoration(decorator);
    }
    if (index > currentIndex) {
        throw std::out_of_range("Index out of range");
    }
}