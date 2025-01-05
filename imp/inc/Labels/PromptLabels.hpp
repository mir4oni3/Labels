#pragma once

#include "BaseLabel.hpp"

#include <memory>

class PromptLabel : public Label {
protected:
    std::shared_ptr<Label> label;
public:
    PromptLabel(const std::shared_ptr<Label>&);

    virtual std::string getText() const override;
    virtual std::string getPrompt() const = 0;
};

class HelpLabel : public PromptLabel {
public:
    HelpLabel(const std::shared_ptr<Label>&);

    virtual std::string getPrompt() const override;
};

class NoHelpLabel : public PromptLabel {
public:
    NoHelpLabel(const std::shared_ptr<Label>&);

    virtual std::string getPrompt() const override;
};