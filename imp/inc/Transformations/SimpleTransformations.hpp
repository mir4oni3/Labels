#pragma once

#include "BaseTransformation.hpp"

#include <string>
#include <vector>
#include <memory>

class TextTransformation : public Transformation {
public:
    virtual std::string transform(const std::string&) const override = 0;
};

class CapitalizeTransformation : public TextTransformation {
public:
    virtual std::string transform(const std::string&) const override;
};

class LeftTrimTransformation : public TextTransformation {
public:
    virtual std::string transform(const std::string&) const override;
};

class RightTrimTransformation : public TextTransformation {
public:
    virtual std::string transform(const std::string&) const override;
};

class NormalizeSpaceTransformation : public TextTransformation {
public:
    virtual std::string transform(const std::string&) const override;
};

class DecorateTransformation : public TextTransformation {
public:
    virtual std::string transform(const std::string&) const override;
};