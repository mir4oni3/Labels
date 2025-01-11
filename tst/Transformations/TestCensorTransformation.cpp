#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Transformations/CensorTransformation.hpp"

class TransformationTest : public Transformation { 
    std::string censorWord;
public:
    TransformationTest(const std::string& censorWord) {
        this->censorWord = censorWord;
    }
    std::string transform(const std::string& word) const override {
        return "";
    }
};

TEST_CASE("CensorTransformationConstructorAndGetTest", "[censor_transformation]") {
    std::shared_ptr<CensorTransformation> transformation;
    REQUIRE_NOTHROW(transformation = std::make_shared<CensorTransformation>("hello"));
    REQUIRE(transformation->getCensorWord() == "hello");
    REQUIRE_THROWS_AS(CensorTransformation(""), std::invalid_argument);
}

TEST_CASE("CensorTransformationSetCensorWordTest", "[censor_transformation]") {
    CensorTransformation transformation("hello");
    REQUIRE(transformation.getCensorWord() == "hello");
    REQUIRE_NOTHROW(transformation.setCensorWord("bye"));
    REQUIRE(transformation.getCensorWord() == "bye");
    REQUIRE_THROWS_AS(transformation.setCensorWord(""), std::invalid_argument);
}

TEST_CASE("CensorTransformationOperator==Test", "[censor_transformation]") {
    CensorTransformation transformation1("hello");
    CensorTransformation transformation2("hello");
    CensorTransformation transformation3("bye");
    TransformationTest transformation4("hello");

    REQUIRE(transformation1 == transformation2);
    REQUIRE(!(transformation1 != transformation2));

    REQUIRE(!(transformation1 == transformation3));
    REQUIRE(transformation1 != transformation3);

    REQUIRE(!(transformation1 == transformation4));
    REQUIRE(transformation1 != transformation4);
}

TEST_CASE("CensorTransformationTransformTest", "[censor_transformation]") {
    CensorTransformation transformation("hello");
    REQUIRE(transformation.transform("hello") == "*****");
    REQUIRE(transformation.transform("hello hello") == "***** *****");
    REQUIRE(transformation.transform("asdfghello") == "asdfg*****");
    REQUIRE(transformation.transform("hello12345") == "*****12345");
    REQUIRE(transformation.transform("hellowowhello") == "*****wow*****");
    REQUIRE(transformation.transform("hello bye hello") == "***** bye *****");
    REQUIRE(transformation.transform("byehellobye") == "bye*****bye");
    REQUIRE(transformation.transform("hellohellohello") == "***************");
    REQUIRE(transformation.transform("hello hellohello hello") == "***** ********** *****");
    REQUIRE(transformation.transform("hello hello hellohello") == "***** ***** **********");
    REQUIRE(transformation.transform("hellobyehello123hellohelloyesno") == "*****bye*****123**********yesno");
}