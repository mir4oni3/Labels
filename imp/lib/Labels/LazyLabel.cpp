#include "../../inc/Labels/LazyLabel.hpp"

#include <iostream>

LazyLabel::LazyLabel(const std::shared_ptr<Label>& label) {
    if (!label) {
        throw std::invalid_argument("Label cannot be nullptr");
    }
    this->label = label;
}

namespace {
    void requestValue(std::shared_ptr<Label>& label) {
        if (!label) {
            throw std::invalid_argument("Label cannot be nullptr");
        }
        std::cout << "Enter value for label: " << std::endl;
        std::string value;
        std::cin >> value;
        label->setText(value);
    }
}

std::string LazyLabel::getText() const {
    if (requestCount == -1) {
        requestValue(label);
        requestCount = 0;
    }
    requestCount++;
    if (requestCount == LAZY_LABEL_TIMEOUT) {
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