#pragma once

#include "SimpleTransformations.hpp"

class CompositeTransformation : public Transformation {
    std::vector<std::shared_ptr<Transformation>> transformations;
public:
    CompositeTransformation() = default;
    CompositeTransformation(const std::vector<std::shared_ptr<Transformation>>&);

    const std::vector<std::shared_ptr<Transformation>>& getTransformations() const;
    void setTransformations(const std::vector<std::shared_ptr<Transformation>>&);

    virtual std::string transform(const std::string&) const override;

    bool operator==(const Transformation&) const override;
    
    virtual void pushTransformation(const std::shared_ptr<Transformation>&);
    virtual std::shared_ptr<Transformation> popTransformation();
    virtual std::shared_ptr<Transformation> popAt(unsigned int);
    virtual void insertAt(const std::shared_ptr<Transformation>&, unsigned int);
};