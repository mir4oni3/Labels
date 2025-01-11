#include "../../inc/Decorators/BaseDecorator.hpp"
#include "../../inc/Transformations/CompositeTransformation.hpp"

#include <iostream>
#include <memory>
#include <typeinfo>

static int insertRecursively(std::shared_ptr<BaseDecorator>& decorator, unsigned int indexInsert, std::shared_ptr<BaseDecorator>& toInsert) {
    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(decorator->getLabel());
    int currentIndex;

    if (!under) {
        currentIndex = 0;
    }
    else {
        currentIndex = insertRecursively(under, indexInsert, toInsert) + 1;
    }

    if (currentIndex == indexInsert) {
        toInsert->setLabel(decorator->getLabel());
        decorator->setLabel(toInsert);
    }

    return currentIndex;
}

std::shared_ptr<BaseDecorator> pushDecoration(std::shared_ptr<BaseDecorator>& decorator, std::shared_ptr<BaseDecorator>& toPush) {
    if (!decorator || !toPush) {	
        throw std::invalid_argument("Decoration cannot be nullptr");
    }
    toPush->setLabel(decorator);
    return toPush;
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

std::shared_ptr<BaseDecorator> removeLastDecoration(std::shared_ptr<BaseDecorator>& decorator) {
    std::shared_ptr<BaseDecorator> result = std::dynamic_pointer_cast<BaseDecorator>(decorator->getLabel());

    if (!result) {
        //To preserve invariants, decorators always need to have at least one decoration.
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }

    return result;
}

std::shared_ptr<BaseDecorator> removeAtIndex(std::shared_ptr<BaseDecorator>& decorator, unsigned int index) {
    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(decorator->getLabel());

    if (!under) {
        //To preserve invariants, decorators always need to have at least one decoration.
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }

    std::shared_ptr<Label> temp = nullptr;
    int currentIndex = removeRecursively(under, index, temp) + 1;

    if (index > currentIndex) {
        throw std::out_of_range("Index out of range");
    }

    if (index == currentIndex) {
        decorator = removeLastDecoration(decorator);
    }
}

bool BaseDecorator::operator==(const BaseDecorator& other) const {
    return typeid(*this) == typeid(other) && *label == *other.label && *transformation == *other.transformation;
}

static std::shared_ptr<BaseDecorator> removeDecorator(std::shared_ptr<BaseDecorator>& removeFrom, const std::shared_ptr<BaseDecorator>& toRemove, const std::function<bool(const BaseDecorator&, const BaseDecorator&)>& comp) {
    if (!toRemove || !removeFrom) {
        throw std::invalid_argument("Decorator cannot be nullptr");
    }

    std::shared_ptr<BaseDecorator> under = std::dynamic_pointer_cast<BaseDecorator>(removeFrom->getLabel());
    
    if (!under && comp(*toRemove, *removeFrom)) {
        //To preserve invariants, decorators always need to have at least one decoration.
        throw std::runtime_error(
            "Tried removal of the single remaining decoration. "
            "If you want to extract the underlying label, use getUnderlyingLabel() instead."
        );
    }

    std::shared_ptr<BaseDecorator> current = nullptr;

    while (under) {
        if (comp(*under, *toRemove)) {
            if (current) {
                current->setLabel(under->getLabel());
            } else {
                removeFrom = removeLastDecoration(removeFrom);
            }
            return removeFrom;
        }
        current = under;
        under = std::dynamic_pointer_cast<BaseDecorator>(under->getLabel());
    }

    throw std::logic_error("No matching decoration to remove.");
    return nullptr;
}

std::shared_ptr<BaseDecorator> removeType(std::shared_ptr<BaseDecorator>& removeFrom, const std::shared_ptr<BaseDecorator>& toRemove) {
    return removeDecorator(removeFrom, toRemove, [](const BaseDecorator& a, const BaseDecorator& b) {
        return typeid(a) == typeid(b);
    });
}

std::shared_ptr<BaseDecorator> removeSpecific(std::shared_ptr<BaseDecorator>& removeFrom, const std::shared_ptr<BaseDecorator>& toRemove) {
    return removeDecorator(removeFrom, toRemove, [](const BaseDecorator& a, const BaseDecorator& b) {
        return a == b;
    });
}

std::shared_ptr<BaseDecorator> insertDecoration(std::shared_ptr<BaseDecorator>& decorator, unsigned int index, std::shared_ptr<BaseDecorator>& toInsert) {
    if (!decorator || !toInsert) {
        throw std::invalid_argument("Decoration cannot be nullptr");
    }

    int currentIndex = insertRecursively(decorator, index, toInsert);

    if (index > currentIndex) {
        throw std::out_of_range("Index out of range");
    }

    return decorator;
}