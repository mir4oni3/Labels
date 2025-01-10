#pragma once

#include "BaseLabel.hpp"

#include <memory>

namespace LazyLabelConstants {
    const unsigned short LAZY_LABEL_TIMEOUT = 5;
}

class LazyLabel : public Label {
protected:
    mutable std::shared_ptr<Label> label;
    mutable short requestCount = -1;
    unsigned short timeout = LazyLabelConstants::LAZY_LABEL_TIMEOUT;

public:
    LazyLabel(const std::shared_ptr<Label>&);
    LazyLabel(const std::shared_ptr<Label>&, unsigned short);

    bool operator==(const Label&) override;

    virtual std::string getText() const override;
};
