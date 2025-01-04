#pragma once

#include <string>

class Label {
public:
    virtual std::string getText() const = 0;

    virtual ~Label() = default;
};

class SimpleLabel : public Label {
    std::string value;

public:
    SimpleLabel(const std::string&);

    virtual std::string getText() const override;
    virtual void setText(const std::string&);
};

class RichLabel : public Label {
    std::string value;
    std::string color;
    unsigned short fontSize;
    std::string fontName;

public:
    RichLabel(const std::string&, const std::string&,
              unsigned short, const std::string&);

    virtual std::string getText() const override;
    virtual void setText(const std::string&);

    virtual std::string getColor() const;
    virtual void setColor(const std::string&);

    virtual unsigned short getFontSize() const;
    virtual void setFontSize(unsigned short);

    virtual std::string getFontName() const;
    virtual void setFontName(const std::string&);
};