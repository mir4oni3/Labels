#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Transformations/CensorTransformationFactory.hpp"

class FactoryStub : public CensorTransformationFactory {
public:
    FactoryStub() : CensorTransformationFactory() {}
    FactoryStub(unsigned int maxLengthToStore) : CensorTransformationFactory(maxLengthToStore) {}

    std::unordered_map<std::string, std::shared_ptr<CensorTransformation>> getFlyweight() {
        return flyweight;
    }

    void setFlyweight(std::unordered_map<std::string, std::shared_ptr<CensorTransformation>> flyweight) {
        this->flyweight = flyweight;
    }
};

TEST_CASE("CensorTransformationFactoryConstructorAndGetTest", "[censor_factory]") {
    std::shared_ptr<CensorTransformationFactory> factory;
    REQUIRE_NOTHROW(factory = std::make_shared<CensorTransformationFactory>());

    int maxLength;
    REQUIRE_NOTHROW(maxLength = factory->getMaxLengthToStore());
    REQUIRE(maxLength == CensorTransformationFactoryConstants::DEFAULT_MAX_LENGTH_TO_STORE);

    REQUIRE_NOTHROW(factory = std::make_shared<CensorTransformationFactory>(5));
    REQUIRE_NOTHROW(maxLength = factory->getMaxLengthToStore());
    REQUIRE(maxLength == 5);
}

TEST_CASE("CensorTransformationFactorySetMaxLengthTest", "[censor_factory]") {
    FactoryStub factory;

    REQUIRE_NOTHROW(factory.setMaxLengthToStore(42));
    REQUIRE(factory.getMaxLengthToStore() == 42);

    REQUIRE_NOTHROW(factory.setMaxLengthToStore(6));
    REQUIRE(factory.getMaxLengthToStore() == 6);

    REQUIRE_THROWS_AS(factory.getCensorTransformation(""), std::invalid_argument);

    factory.getCensorTransformation("1");
    factory.getCensorTransformation("12");
    factory.getCensorTransformation("123");
    factory.getCensorTransformation("1234");
    factory.getCensorTransformation("12345");
    factory.getCensorTransformation("123456");
    factory.getCensorTransformation("1234567");

    std::unordered_map<std::string, std::shared_ptr<CensorTransformation>> Expectedflyweight;
    
    //j represents the maximum length to store
    for (int j = 6; j >= 1; j--) {

        //clear expected flyweight from previous iteration
        Expectedflyweight.clear();

        for (int i = 1; i <= j; i++) {
            //fill expected flyweight with 1, 12, 123, ..., 123...j
            std::string key;
            for (int k = 1; k <= i; k++) {
                key += std::to_string(k);
            }

            Expectedflyweight[key] = std::make_shared<CensorTransformation>(key);
        }

        //decrementing maxLengthToStore should remove longest(last) element from flyweight
        factory.setMaxLengthToStore(j);

        for (auto& [key, value] : factory.getFlyweight()) {
            //assert that the flyweight contains only the expected values
            //the expected values are from 1 to j
            REQUIRE(Expectedflyweight.count(key) > 0);
            REQUIRE(*Expectedflyweight[key] == *value);
        }

    }
}

TEST_CASE("CensorTransformationFactoryGetCensorTransformationTest", "[censor_factory]") {
    FactoryStub factory(3);
    std::shared_ptr<CensorTransformation> transformation;
    
    REQUIRE_NOTHROW(transformation = factory.getCensorTransformation("1"));
    REQUIRE(*transformation == CensorTransformation("1"));

    REQUIRE_NOTHROW(transformation = factory.getCensorTransformation("12"));
    REQUIRE(*transformation == CensorTransformation("12"));

    REQUIRE_NOTHROW(transformation = factory.getCensorTransformation("123"));
    REQUIRE(*transformation == CensorTransformation("123"));

    REQUIRE_NOTHROW(transformation = factory.getCensorTransformation("1234"));
    REQUIRE(*transformation == CensorTransformation("1234"));

    REQUIRE_NOTHROW(transformation = factory.getCensorTransformation("12345"));
    REQUIRE(*transformation == CensorTransformation("12345"));

    REQUIRE_NOTHROW(transformation = factory.getCensorTransformation("123456"));
    REQUIRE(*transformation == CensorTransformation("123456"));

    REQUIRE_NOTHROW(transformation = factory.getCensorTransformation("1234567"));
    REQUIRE(*transformation == CensorTransformation("1234567"));

    std::unordered_map<std::string, std::shared_ptr<CensorTransformation>> Expectedflyweight;
    Expectedflyweight["1"] = std::make_shared<CensorTransformation>("1");
    Expectedflyweight["12"] = std::make_shared<CensorTransformation>("12");
    Expectedflyweight["123"] = std::make_shared<CensorTransformation>("123");
    for (auto& [key, value] : factory.getFlyweight()) {
        REQUIRE(Expectedflyweight.count(key) > 0);
        REQUIRE(*Expectedflyweight[key] == *value);
    }

}