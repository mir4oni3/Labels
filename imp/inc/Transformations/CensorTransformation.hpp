#pragma once

#include "SimpleTransformations.hpp"

class CensorTransformation : public TextTransformation {
    std::string censorWord;
public:
    CensorTransformation(const std::string&);
    const std::string& getCensorWord() const;
    void setCensorWord(const std::string&);

    virtual std::string transform(const std::string&) const override;
};