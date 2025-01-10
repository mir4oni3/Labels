#pragma once

#include "BaseLabel.hpp"

#include <memory>

class SimpleLabel : public Label {
public:
    SimpleLabel(const std::string&);
    virtual std::string getText() const override;
};

class RichLabel : public Label {
protected:
    std::string color;
    unsigned short fontSize;
    std::string fontName;
public:
    RichLabel(const std::string&, const std::string&,
              unsigned short, const std::string&);

    virtual bool operator==(const Label&) override;
    
    virtual std::string getText() const override;

    virtual std::string getColor() const;
    virtual void setColor(const std::string&);

    virtual unsigned short getFontSize() const;
    virtual void setFontSize(unsigned short);

    virtual std::string getFontName() const;
    virtual void setFontName(const std::string&);
};