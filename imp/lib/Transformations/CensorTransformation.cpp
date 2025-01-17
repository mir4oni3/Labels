#include "../../inc/Transformations/CensorTransformation.hpp"

#include <iostream>

CensorTransformation::CensorTransformation(const std::string& word) {
    setCensorWord(word);
}

const std::string& CensorTransformation::getCensorWord() const {
    return censorWord;
}

void CensorTransformation::setCensorWord(const std::string& word) {
    if (word == "") {
        throw std::invalid_argument("Censor word cannot be empty");
    }
    censorWord = word;
}

bool CensorTransformation::operator==(const Transformation& other) {
    try {
        const CensorTransformation& otherCensor = dynamic_cast<const CensorTransformation&>(other);
        return censorWord == otherCensor.censorWord;
    } catch (const std::bad_cast& e) {}
    
    return false;
}

std::string CensorTransformation::transform(const std::string& text) const {
    std::string result = text;
    size_t start = 0;
    while ((start = result.find(censorWord, start)) != std::string::npos) {
        result.replace(start, censorWord.length(), censorWord.length(), '*');
        start += censorWord.length();
    }
    return result;
}