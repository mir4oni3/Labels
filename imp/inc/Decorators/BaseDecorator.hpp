#pragma once

#include "../Labels/BaseLabel.hpp"
#include "../Transformations/CompositeTransformation.hpp"

#include <memory>
#include <functional>

class BaseDecorator : public Label {
protected:
    std::shared_ptr<Label> label;
    std::shared_ptr<CompositeTransformation> transformation;

public:
    BaseDecorator(const std::shared_ptr<Label>&, const std::shared_ptr<Transformation>&);

    virtual ~BaseDecorator() = default;

    virtual std::shared_ptr<BaseDecorator> clone() const = 0;
    virtual bool operator==(const BaseDecorator&) const;

    const std::shared_ptr<Label>& getLabel() const;
    void setLabel(const std::shared_ptr<Label>&);

    const std::shared_ptr<Label>& getUnderlyingLabel() const;

    const std::shared_ptr<Transformation>& getTransformation() const;
    void setTransformation(const std::shared_ptr<Transformation>&);

    virtual std::string getText() const override = 0;
};

std::shared_ptr<BaseDecorator> removeLastDecoration(std::shared_ptr<BaseDecorator>&);
std::shared_ptr<BaseDecorator> removeAtIndex(std::shared_ptr<BaseDecorator>&, unsigned int);
std::shared_ptr<BaseDecorator> removeType(std::shared_ptr<BaseDecorator>&, const std::shared_ptr<BaseDecorator>&);
std::shared_ptr<BaseDecorator> removeSpecific(std::shared_ptr<BaseDecorator>&, const std::shared_ptr<BaseDecorator>&);

std::shared_ptr<BaseDecorator> pushDecoration(std::shared_ptr<BaseDecorator>&, const std::shared_ptr<BaseDecorator>&);
std::shared_ptr<BaseDecorator> insertDecoration(std::shared_ptr<BaseDecorator>&, unsigned int, std::shared_ptr<BaseDecorator>&);