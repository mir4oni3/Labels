#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Transformations/BaseTransformation.hpp"

class BaseTransformationTest1 : public Transformation {
public:
    std::string transform(const std::string&) const override {
        return "";
    }
};

class BaseTransformationTest2 : public Transformation {
public:
    std::string transform(const std::string&) const override {
        return "";
    }
};

TEST_CASE("BaseLabelComparisonOperatorsTest", "[base_transformation]") {
    BaseTransformationTest1 t1;
    BaseTransformationTest1 t2;
    BaseTransformationTest2 t3;

    REQUIRE(t1 == t2);
    REQUIRE(!(t1 != t2));

    REQUIRE(!(t1 == t3));
    REQUIRE(t1 != t3);

    REQUIRE(!(t2 == t3));
    REQUIRE(t2 != t3);
}