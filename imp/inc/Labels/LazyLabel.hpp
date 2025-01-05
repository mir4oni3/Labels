#pragma once

#include "BaseLabel.hpp"

#include <memory>

const short LAZY_LABEL_TIMEOUT = 5;

class LazyLabel : public Label {
protected:
    std::shared_ptr<Label> label;
    short requestCount = -1;
public:
    LazyLabel(const std::shared_ptr<Label>&);

    virtual std::string getText() const override;
};
