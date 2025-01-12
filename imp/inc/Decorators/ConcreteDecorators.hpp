#pragma once

#include "BaseDecorator.hpp"

#include <random>

//will only have one item in the composite transformation
class SimpleDecorator : public BaseDecorator {
    std::shared_ptr<Transformation> transformation;
public:
    SimpleDecorator(const std::shared_ptr<Label>&, const std::shared_ptr<Transformation>&);
    std::string getText() const override;
    const std::shared_ptr<Transformation>& getTransformation() const;
    void setTransformation(const std::shared_ptr<Transformation>&);
};

class RandomDecorator : public BaseDecorator {
    std::vector<std::shared_ptr<Transformation>> transformation;
    mutable unsigned int current = 0;
    mutable std::vector<int> order;

public:
    RandomDecorator(const std::shared_ptr<Label>&, const std::vector<std::shared_ptr<Transformation>>&);
    std::string getText() const override;
    bool operator==(const BaseDecorator&) const override;
    const std::vector<std::shared_ptr<Transformation>>& getTransformation() const;
    void setTransformation(const std::vector<std::shared_ptr<Transformation>>&);
};

class RepeatingDecorator : public BaseDecorator {
    std::vector<std::shared_ptr<Transformation>> transformation;
    mutable unsigned int current = 0;
public:
    RepeatingDecorator(const std::shared_ptr<Label>&, const std::vector<std::shared_ptr<Transformation>>&);
    std::string getText() const override;
    const std::vector<std::shared_ptr<Transformation>>& getTransformation() const;
    void setTransformation(const std::vector<std::shared_ptr<Transformation>>&);
};