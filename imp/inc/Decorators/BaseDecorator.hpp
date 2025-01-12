#pragma once

#include "../Labels/BaseLabel.hpp"
#include "../Transformations/CompositeTransformation.hpp"

#include <memory>
#include <functional>

class BaseDecorator : public Label {
protected:
    std::shared_ptr<Label> label;

public:
    BaseDecorator(const std::shared_ptr<Label>&);

    virtual ~BaseDecorator() = default;

    virtual bool operator==(const BaseDecorator&) const;

    const std::shared_ptr<Label>& getLabel() const;
    void setLabel(const std::shared_ptr<Label>&);

    const std::shared_ptr<Label>& getUnderlyingLabel() const;

    virtual std::string getText() const override = 0;
};

std::shared_ptr<BaseDecorator> removeLastDecoration(std::shared_ptr<BaseDecorator>&);
std::shared_ptr<BaseDecorator> removeAtIndex(std::shared_ptr<BaseDecorator>&, unsigned int);
std::shared_ptr<BaseDecorator> removeType(std::shared_ptr<BaseDecorator>&, const std::shared_ptr<BaseDecorator>&);
std::shared_ptr<BaseDecorator> removeSpecific(std::shared_ptr<BaseDecorator>&, const std::shared_ptr<BaseDecorator>&);

std::shared_ptr<BaseDecorator> pushDecoration(std::shared_ptr<BaseDecorator>&, const std::shared_ptr<BaseDecorator>&);
std::shared_ptr<BaseDecorator> insertDecoration(std::shared_ptr<BaseDecorator>&, unsigned int, std::shared_ptr<BaseDecorator>&);