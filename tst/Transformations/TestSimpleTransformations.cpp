#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Transformations/SimpleTransformations.hpp"

TEST_CASE("CapitalizeTransformationTransformTest", "[capitalize_transformation]") {
    CapitalizeTransformation transformation;
    REQUIRE(transformation.transform("hello") == "Hello");
    REQUIRE(transformation.transform("Hello") == "Hello");
    REQUIRE(transformation.transform("HELLO") == "HELLO");
    REQUIRE(transformation.transform("") == "");
    REQUIRE(transformation.transform(" ") == " ");
    REQUIRE(transformation.transform("1hello") == "1hello");
}

TEST_CASE("LeftTrimTransformationTransformTest", "[left_trim_transformation]") {
    LeftTrimTransformation transformation;
    REQUIRE(transformation.transform("hello") == "hello");
    REQUIRE(transformation.transform(" hello") == "hello");
    REQUIRE(transformation.transform("  hello") == "hello");
    REQUIRE(transformation.transform("hello ") == "hello ");
    REQUIRE(transformation.transform("   hello  hello  ") == "hello  hello  ");
    REQUIRE(transformation.transform("") == "");
    REQUIRE(transformation.transform(" ") == "");
}

TEST_CASE("RightTrimTransformationTransformTest", "[right_trim_transformation]") {
    RightTrimTransformation transformation;
    REQUIRE(transformation.transform("hello") == "hello");
    REQUIRE(transformation.transform("hello ") == "hello");
    REQUIRE(transformation.transform("hello  ") == "hello");
    REQUIRE(transformation.transform(" hello") == " hello");
    REQUIRE(transformation.transform("  hello  hello   ") == "  hello  hello");
    REQUIRE(transformation.transform("") == "");
    REQUIRE(transformation.transform(" ") == "");
}

TEST_CASE("NormalizeSpaceTransformationTransformTest", "[normalize_space_transformation]") {
    NormalizeSpaceTransformation transformation;
    REQUIRE(transformation.transform("hello") == "hello");
    REQUIRE(transformation.transform(" hello") == " hello");
    REQUIRE(transformation.transform("  hello") == " hello");
    REQUIRE(transformation.transform("hello ") == "hello ");
    REQUIRE(transformation.transform("hello  ") == "hello ");
    REQUIRE(transformation.transform("    hello    hello   ") == " hello hello ");
    REQUIRE(transformation.transform("") == "");
    REQUIRE(transformation.transform(" ") == " ");
}

TEST_CASE("DecorateTransformationTransformTest", "[decorate_transformation]") {
    DecorateTransformation transformation;
    REQUIRE(transformation.transform("hello") == "-={ hello }=-");
    REQUIRE(transformation.transform(" hello") == "-={  hello }=-");
    REQUIRE(transformation.transform("  hello") == "-={   hello }=-");
    REQUIRE(transformation.transform("hello ") == "-={ hello  }=-");
    REQUIRE(transformation.transform("hello  ") == "-={ hello   }=-");
    REQUIRE(transformation.transform("    hello    hello    ") == "-={     hello    hello     }=-");
    REQUIRE(transformation.transform("") == "-={  }=-");
    REQUIRE(transformation.transform(" ") == "-={   }=-");
}