#include "../../inc/Transformations/CensorTransformationFactory.hpp"

CensorTransformationFactory::CensorTransformationFactory(unsigned int maxLengthToStore) : maxLengthToStore(maxLengthToStore) {}

std::shared_ptr<CensorTransformation> CensorTransformationFactory::getCensorTransformation(const std::string& censoredWord) {
    if (censoredWord.length() > maxLengthToStore) {
        return std::make_shared<CensorTransformation>(censoredWord);
    }
    if (flyweight.count(censoredWord) == 0) {
        flyweight[censoredWord] = std::make_shared<CensorTransformation>(censoredWord);
    }
    return flyweight[censoredWord];
}

//NOTE: Decreasing the maxLengthToStore will remove stored transformations that exceed the new length.
void CensorTransformationFactory::setMaxLengthToStore(unsigned int maxLengthToStore) {
    this->maxLengthToStore = maxLengthToStore;

    for (auto it = flyweight.begin(); it != flyweight.end();) {
        if (it->first.length() > maxLengthToStore) {
            it = flyweight.erase(it);
        } else {
            ++it;
        }
    }
}

unsigned int CensorTransformationFactory::getMaxLengthToStore() const {
    return maxLengthToStore;
}