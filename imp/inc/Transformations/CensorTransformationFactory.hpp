#include "CensorTransformation.hpp"

#include <unordered_map>

namespace CensorTransformationFactoryConstants {
    const unsigned int DEFAULT_MAX_LENGTH_TO_STORE = 4;
}

class CensorTransformationFactory {
    std::unordered_map<std::string, std::shared_ptr<CensorTransformation>> flyweight;
    unsigned int maxLengthToStore = CensorTransformationFactoryConstants::DEFAULT_MAX_LENGTH_TO_STORE;
public:
    CensorTransformationFactory() = default;
    CensorTransformationFactory(unsigned int maxLengthToStore);

    std::shared_ptr<CensorTransformation> getCensorTransformation(const std::string&);

    void setMaxLengthToStore(unsigned int);
    unsigned int getMaxLengthToStore() const;
};