#include "../../inc/Labels/SimpleLabels.hpp"

SimpleLabel::SimpleLabel(const std::string& text) {
    value = text;
}

std::string SimpleLabel::getText() const {
    return value;
}

RichLabel::RichLabel(const std::string& text, const std::string& color,
                     unsigned short size, const std::string& name) {
    this->value = text;
    this->color = color;
    this->fontSize = size;
    this->fontName = name;
}

std::string RichLabel::getText() const {
    return "<font color=\"" + color + "\" size=\"" + std::to_string(fontSize) +
           "\" face=\"" + fontName + "\">" + value + "</font>\n";
}	

std::string RichLabel::getColor() const {
    return color;
}

void RichLabel::setColor(const std::string& color) {
    this->color = color;
}

unsigned short RichLabel::getFontSize() const {
    return fontSize;
}

void RichLabel::setFontSize(unsigned short size) {
    this->fontSize = size;
}

std::string RichLabel::getFontName() const {
    return fontName;
}

void RichLabel::setFontName(const std::string& name) {
    this->fontName = name;
}