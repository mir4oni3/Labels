#pragma once

#include <string>

class Transformation {
public:
    virtual std::string transform(const std::string&) const = 0;
    virtual ~Transformation() = default;

    virtual bool operator==(const Transformation&) const;
    virtual bool operator!=(const Transformation&) const;
};