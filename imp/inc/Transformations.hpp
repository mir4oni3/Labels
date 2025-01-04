#pragma once

#include <string>
#include <vector>
#include <memory>

class Transformation {
public:
    virtual std::string transform(const std::string&) const = 0;
    virtual ~Transformation() = default;
};

// ------------------------------------------------------ //

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

class CensorTransformation : public TextTransformation {
    std::string censorWord;
public:
    CensorTransformation(const std::string&);
    const std::string& getCensorWord() const;
    void setCensorWord(const std::string&);

    virtual std::string transform(const std::string&) const override;
};

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

// ------------------------------------------------------ //

class CompositeTransformation : public Transformation {
    std::vector<std::shared_ptr<Transformation>> transformations;
public:
    CompositeTransformation(const std::vector<std::shared_ptr<Transformation>>&);

    const std::vector<std::shared_ptr<Transformation>>& getTransformations() const;
    void setTransformations(const std::vector<std::shared_ptr<Transformation>>&);

    virtual std::string transform(const std::string&) const override;

    virtual void pushTransformation(const std::shared_ptr<Transformation>&);
    virtual std::shared_ptr<Transformation> popTransformation();
    virtual std::shared_ptr<Transformation> popAt(unsigned int);
    virtual void insertAt(const std::shared_ptr<Transformation>&, unsigned int);
};