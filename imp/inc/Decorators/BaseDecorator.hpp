#pragma once

#include "BaseLabel.hpp"
#include "CompositeTransformation.hpp"

#include <memory>
#include <functional>

class BaseDecorator : public Label {
    int removeRecursively(std::shared_ptr<BaseDecorator>&, unsigned int, std::shared_ptr<Label>&) const;
    void removeDecorator(const std::shared_ptr<BaseDecorator>&, const std::function<bool(const BaseDecorator&, const BaseDecorator&)>&);

protected:
    std::shared_ptr<Label> label;
    std::shared_ptr<CompositeTransformation> transformation;

public:
    BaseDecorator(const std::shared_ptr<Label>&, const std::shared_ptr<Transformation>&);
    virtual ~BaseDecorator() = default;

    virtual bool operator==(const BaseDecorator&) const;

    const std::shared_ptr<Label>& getLabel() const;
    const std::shared_ptr<Label>& getUnderlyingLabel() const;

    const std::shared_ptr<Transformation>& getTransformation() const;
    void setTransformation(const std::shared_ptr<Transformation>&);

    virtual std::string getText() const override = 0;

    void removeLastDecoration();
    void removeAtIndex(unsigned int);
    void removeType(const std::shared_ptr<BaseDecorator>&);
    void removeSpecific(const std::shared_ptr<BaseDecorator>&);

    void pushDecoration(const std::shared_ptr<BaseDecorator>&);
    void insertDecoration(unsigned int, const std::shared_ptr<BaseDecorator>&);
};