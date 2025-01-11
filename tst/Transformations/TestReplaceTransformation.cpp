#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Transformations/ReplaceTransformation.hpp"

class TransformationTest : public Transformation { 
    std::string from;
    std::string to;
public:
    TransformationTest(const std::string& from, const std::string& to) {
        this->from = from;
        this->to = to;
    }
    std::string transform(const std::string& word) const override {
        return "";
    }
};

TEST_CASE("ReplaceTransformationConstructorAndGettersTest", "[replace_transformation]") {
    std::shared_ptr<ReplaceTransformation> transformation;
    REQUIRE_NOTHROW(transformation = std::make_shared<ReplaceTransformation>("from", "to"));
    REQUIRE(transformation->getFrom() == "from");
    REQUIRE(transformation->getTo() == "to");
}

TEST_CASE("ReplaceTransformationSettersTest", "[replace_transformation]") {
    ReplaceTransformation transformation("from", "to");
    REQUIRE_NOTHROW(transformation.setFrom("new from"));
    REQUIRE(transformation.getFrom() == "new from");
    REQUIRE_NOTHROW(transformation.setTo("new to"));
    REQUIRE(transformation.getTo() == "new to");
    REQUIRE_THROWS_AS(transformation.setFrom(""), std::invalid_argument);
}

TEST_CASE("ReplaceTransformationOperator==Test", "[replace_transformation]") {
    ReplaceTransformation transformation1("from", "to");
    ReplaceTransformation transformation2("from", "to");
    ReplaceTransformation transformation3("new from", "to");
    ReplaceTransformation transformation4("from", "new to");
    TransformationTest transformation5("from", "to");

    REQUIRE(transformation1 == transformation2);
    REQUIRE(!(transformation1 != transformation2));

    REQUIRE(!(transformation1 == transformation3));
    REQUIRE(transformation1 != transformation3);

    REQUIRE(!(transformation1 == transformation4));
    REQUIRE(transformation1 != transformation4);

    REQUIRE(!(transformation1 == transformation5));
    REQUIRE(transformation1 != transformation5);
}

TEST_CASE("ReplaceTransformationTransformTest", "[replace_transformation]") {
    ReplaceTransformation transformation("from", "to");
    REQUIRE(transformation.transform("from") == "to");
    REQUIRE(transformation.transform("from from") == "to to");
    REQUIRE(transformation.transform("from from from") == "to to to");
    REQUIRE(transformation.transform("to") == "to");
    REQUIRE(transformation.transform("hello") == "hello");
    REQUIRE(transformation.transform("") == "");
    REQUIRE(transformation.transform(" ") == " ");
    REQUIRE(transformation.transform("fromfromfrom") == "tototo");
    REQUIRE(transformation.transform("tohellofromhelloto") == "tohellotohelloto");
    REQUIRE(transformation.transform("f rom") == "f rom");

}