#include "../../inc/Decorators/ConcreteDecorators.hpp"

#include <iostream>
#include <algorithm>

static void shuffle (std::vector<int>& order) {
    //seeding is done in main()
    for (int i = 0; i < order.size() - 1; i++) {
        int j = rand() % (order.size() - i) + i;
        std::swap(order[i], order[j]);
    }
}

SimpleDecorator::SimpleDecorator(const std::shared_ptr<Label>& label, const std::shared_ptr<Transformation>& transformation) : BaseDecorator(label) {
    if (!transformation) {
        throw std::invalid_argument("Transformation cannot be nullptr");
    }
    this->transformation = transformation;
}

std::string SimpleDecorator::getText() const {
    return transformation->transform(label->getText());
}

const std::shared_ptr<Transformation>& SimpleDecorator::getTransformation() const {
    return transformation;
}

void SimpleDecorator::setTransformation(const std::shared_ptr<Transformation>& transformation) {
    if (!transformation) {
        throw std::invalid_argument("Transformation cannot be nullptr");
    }
    this->transformation = transformation;
}

RandomDecorator::RandomDecorator(const std::shared_ptr<Label>& label, const std::vector<std::shared_ptr<Transformation>>& transformations) : BaseDecorator(label) {
    setTransformation(transformations);
}

std::string RandomDecorator::getText() const {
    if (transformation.empty()) {
        return label->getText();
    }

    unsigned int index = order[current];

    if (++current == order.size()) {
        shuffle(order);
        current = 0;
    }

    return transformation[index]->transform(label->getText());
}

bool RandomDecorator::operator==(const BaseDecorator& other) const {
    try {

        const RandomDecorator& otherRandom = dynamic_cast<const RandomDecorator&>(other);
        return BaseDecorator::operator==(other) &&
               transformation == otherRandom.transformation &&
               order == otherRandom.order &&
               current == otherRandom.current;

    } catch (const std::bad_cast& e) {}

    return false;
}

const std::vector<std::shared_ptr<Transformation>>& RandomDecorator::getTransformation() const {
    return transformation;
}

void RandomDecorator::setTransformation(const std::vector<std::shared_ptr<Transformation>>& transformations) {
    for (int i = 0; i < transformations.size(); i++) {
        if (!transformations[i]) {
            throw std::invalid_argument("Transformation cannot be nullptr");
        }
    }
    current = 0;
    order = std::vector<int>(transformations.size());
    for (int i = 0; i < transformations.size(); i++) {
        order[i] = i;
    }
    shuffle(order);

    this->transformation = transformations;
}

RepeatingDecorator::RepeatingDecorator(const std::shared_ptr<Label>& label, const std::vector<std::shared_ptr<Transformation>>& transformations) : BaseDecorator(label) {
    setTransformation(transformations);
}

const std::vector<std::shared_ptr<Transformation>>& RepeatingDecorator::getTransformation() const {
    return transformation;
}

void RepeatingDecorator::setTransformation(const std::vector<std::shared_ptr<Transformation>>& transformations) {
    for (int i = 0; i < transformations.size(); i++) {
        if (!transformations[i]) {
            throw std::invalid_argument("Transformation cannot be nullptr");
        }
    }
    this->transformation = transformations;
}

std::string RepeatingDecorator::getText() const {
    if (transformation.empty()) {
        return label->getText();
    }

    unsigned int index = current;
    current = (current + 1) % transformation.size();

    return transformation[index]->transform(label->getText());
}