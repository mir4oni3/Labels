#pragma once

#include "SimpleTransformations.hpp"

class ReplaceTransformation : public TextTransformation {
    std::string from;
    std::string to;
public:
    ReplaceTransformation(const std::string&, const std::string&);
    const std::string& getFrom() const;
    void setFrom(const std::string&);
    const std::string& getTo() const;
    void setTo(const std::string&);

    virtual std::string transform(const std::string&) const override;
};