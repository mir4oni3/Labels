#include "../../inc/Decorators/ConcreteDecorators.hpp"

#include <iostream>
#include <algorithm>
#include <random>

SimpleDecorator::SimpleDecorator(const std::shared_ptr<Label>& label, const std::shared_ptr<Transformation>& transformation) : BaseDecorator(label, transformation) {}

std::string SimpleDecorator::getText() const {
    return transformation->transform(label->getText());
}

std::shared_ptr<BaseDecorator> SimpleDecorator::clone() const {
    return std::make_shared<SimpleDecorator>(*this);
}

RandomDecorator::RandomDecorator(const std::shared_ptr<Label>& label, const std::vector<std::shared_ptr<Transformation>>& transformations) : BaseDecorator(label, std::make_shared<CompositeTransformation>(transformations)), order(transformations.size()) {
    for (int i = 0; i < transformations.size(); i++) {
        order[i] = i;
    }
    std::shuffle(order.begin(), order.end(), std::random_device());
}

std::string RandomDecorator::getText() const {
    const std::vector<std::shared_ptr<Transformation>>& transformations = transformation->getTransformations();
    if (transformations.empty()) {
        return label->getText();
    }

    unsigned int index = order[current];

    if (++current == order.size()) {
        std::shuffle(order.begin(), order.end(), std::random_device());
        current = 0;
    }

    return transformations[index]->transform(label->getText());
}

std::shared_ptr<BaseDecorator> RandomDecorator::clone() const {
    return std::make_shared<RandomDecorator>(*this);
}

bool RandomDecorator::operator==(const BaseDecorator& other) const {
    try {
        const RandomDecorator& otherRandom = dynamic_cast<const RandomDecorator&>(other);
        return BaseDecorator::operator==(other) && order == otherRandom.order && current == otherRandom.current;
    } catch (const std::bad_cast& e) {}

    return false;
}

RepeatingDecorator::RepeatingDecorator(const std::shared_ptr<Label>& label, const std::vector<std::shared_ptr<Transformation>>& transformations) : BaseDecorator(label, std::make_shared<CompositeTransformation>(transformations)) {}

std::string RepeatingDecorator::getText() const {
    const std::vector<std::shared_ptr<Transformation>>& transformations = transformation->getTransformations();
    if (transformations.empty()) {
        return label->getText();
    }

    unsigned int index = current;
    current = (current + 1) % transformations.size();

    return transformations[index]->transform(label->getText());
}

std::shared_ptr<BaseDecorator> RepeatingDecorator::clone() const {
    return std::make_shared<RepeatingDecorator>(*this);
}