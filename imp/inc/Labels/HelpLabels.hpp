#pragma once

#include "BaseLabel.hpp"

#include <memory>

class HelpLabel : public Label {
protected:
    std::shared_ptr<Label> label;
public:
    HelpLabel(const std::shared_ptr<Label>&);

    bool operator==(const Label&) override;

    virtual std::string getText() const override;
    virtual void setText(const std::string&) override;

    virtual std::string getHelp() const = 0;
};

class HelpLengthLabel : public HelpLabel {
public:
    HelpLengthLabel(const std::shared_ptr<Label>&);

    virtual std::string getHelp() const override;
};

class HelpWordCountLabel : public HelpLabel {
public:
    HelpWordCountLabel(const std::shared_ptr<Label>&);

    virtual std::string getHelp() const override;
};