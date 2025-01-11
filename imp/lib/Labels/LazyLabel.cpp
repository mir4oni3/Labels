#include "../../inc/Labels/LazyLabel.hpp"

#include <iostream>

static void requestValue(std::shared_ptr<Label>& label) {
    if (!label) {
        throw std::invalid_argument("Label cannot be nullptr");
    }
    std::cout << "Enter value for label: " << std::endl;
    std::string value;
    std::cin >> value;
    label->setText(value);
}

LazyLabel::LazyLabel(const std::shared_ptr<Label>& label) {
    if (!label) {
        throw std::invalid_argument("Label cannot be nullptr");
    }
    this->label = label;
}

LazyLabel::LazyLabel(const std::shared_ptr<Label>& label, unsigned short timeout) : LazyLabel(label) {
    if (timeout == 0) {
        throw std::invalid_argument("Timeout cannot be 0");
    }
    this->timeout = timeout;
}

bool LazyLabel::operator==(const Label& other) {
    try {
        const LazyLabel& otherLazy = dynamic_cast<const LazyLabel&>(other);
        return *label == *otherLazy.label && timeout == otherLazy.timeout && requestCount == otherLazy.requestCount;
    } catch (const std::bad_cast& e) {}
    
    return false;
}

std::string LazyLabel::getText() const {
    if (requestCount == -1) {
        requestValue(label);
        requestCount = 0;
    }
    requestCount++;
    if (requestCount == timeout) {
        std::cout << "Would you want to enter a new value for label? (y/n)" << std::endl;
        char answer;
        std::cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            requestValue(label);
        }
        requestCount = 0;
    }
    return label->getText();
}