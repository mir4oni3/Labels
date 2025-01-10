#pragma once

#include "BaseDecorator.hpp"

//will only have one item in the composite transformation
class SimpleDecorator : public BaseDecorator {
public:
    SimpleDecorator(const std::shared_ptr<Label>&, const std::shared_ptr<Transformation>&);
    std::string getText() const override;
};

class RandomDecorator : public BaseDecorator {
    mutable unsigned int current = 0;
    std::vector<int> order;
public:
    RandomDecorator(const std::shared_ptr<Label>&, const std::vector<std::shared_ptr<Transformation>>&);
    std::string getText() const override;
    bool operator==(const BaseDecorator&) const override;
};

class RepeatingDecorator : public BaseDecorator {
    mutable unsigned int current = 0;
public:
    RepeatingDecorator(const std::shared_ptr<Label>&, const std::vector<std::shared_ptr<Transformation>>&);
    std::string getText() const override;
};