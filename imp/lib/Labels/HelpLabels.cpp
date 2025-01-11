#include "../../inc/Labels/HelpLabels.hpp"

#include <iostream>
#include <memory>

HelpLabel::HelpLabel(const std::shared_ptr<Label>& label) {
    if (!label) {
        throw std::invalid_argument("Label cannot be nullptr");
    }
    this->label = label;
}

std::string HelpLabel::getText() const {
    return label->getText();
}

void HelpLabel::setText(const std::string& text) {
    label->setText(text);
}

HelpLengthLabel::HelpLengthLabel(const std::shared_ptr<Label>& label) : HelpLabel(label) {}

std::string HelpLengthLabel::getHelp() const {
    return "Length of the text is " + std::to_string(label->getText().size());
}

HelpWordCountLabel::HelpWordCountLabel(const std::shared_ptr<Label>& label) : HelpLabel(label) {}

bool HelpLabel::operator==(const Label& other) {
    try {
        const HelpLabel& otherHelp = dynamic_cast<const HelpLabel&>(other);
        return *label == *otherHelp.label;
    } catch (const std::bad_cast& e) {}
    
    return false;
}

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