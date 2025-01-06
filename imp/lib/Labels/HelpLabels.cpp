#include "../../inc/Labels/HelpLabels.hpp"

#include <iostream>

HelpLabel::HelpLabel(const std::shared_ptr<Label>& label) {
    if (!label) {
        throw std::invalid_argument("Label cannot be nullptr");
    }
    this->label = label;
}

std::string HelpLabel::getText() const {
    return label->getText();
}

HelpLengthLabel::HelpLengthLabel(const std::shared_ptr<Label>& label) : HelpLabel(label) {}

std::string HelpLengthLabel::getHelp() const {
    return "Length of the text is " + std::to_string(label->getText().size());
}

HelpWordCountLabel::HelpWordCountLabel(const std::shared_ptr<Label>& label) : HelpLabel(label) {}

std::string HelpWordCountLabel::getHelp() const {
    std::string text = label->getText();
    int count = 0;
    bool inWord = false;
    for (char c : text) {
        if (c == ' ') {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            count++;
        }
    }
    return "Word count is " + std::to_string(count);
}