#pragma once

#include <string>

class Label {
protected:
    std::string value;
public:
    virtual std::string getText() const = 0;
    virtual void setText(const std::string&);

    virtual bool operator==(const Label&);
    virtual bool operator!=(const Label&);

    virtual ~Label() = default;
};