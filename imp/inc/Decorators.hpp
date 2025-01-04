#pragma once

#include "Labels.hpp"
#include "Transformations.hpp"

#include <memory>

class BaseDecorator : public Label {
protected:
    std::shared_ptr<Label> label;
    std::shared_ptr<CompositeTransformation> transformation;
public:
    BaseDecorator(const std::shared_ptr<Label>&, const std::shared_ptr<Transformation>&);
    virtual ~BaseDecorator() = default;

    const std::shared_ptr<Transformation>& getTransformation() const;
    void setTransformation(const std::shared_ptr<Transformation>&);

    virtual std::string getText() const override = 0;

    std::shared_ptr<BaseDecorator> removeLastDecoration();
    std::shared_ptr<BaseDecorator> removeAtIndex(unsigned int);

    void pushDecoration(const std::shared_ptr<BaseDecorator>&);
    void insertDecoration(unsigned int, const std::shared_ptr<BaseDecorator>&);
};

//will only have one item in the composite transformation
class SimpleDecorator : public BaseDecorator {
public:
    SimpleDecorator(const std::shared_ptr<Label>&, const std::shared_ptr<Transformation>&);
    std::string getText() const override;
};

class RandomDecorator : public BaseDecorator {
public:
    RandomDecorator(const std::shared_ptr<Label>&, const std::vector<std::shared_ptr<Transformation>>&);
    std::string getText() const override;
};

class RepeatingDecorator : public BaseDecorator {
    unsigned int current = 0;
public:
    RepeatingDecorator(const std::shared_ptr<Label>&, const std::vector<std::shared_ptr<Transformation>>&);
    std::string getText() const override;
};