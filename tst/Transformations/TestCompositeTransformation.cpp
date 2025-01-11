#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Transformations/CompositeTransformation.hpp"
#include "../../imp/inc/Transformations/CensorTransformation.hpp"
#include "../../imp/inc/Transformations/ReplaceTransformation.hpp"
#include "../../imp/inc/Transformations/SimpleTransformations.hpp"

TEST_CASE("CompositeTransformationConstructorTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    std::shared_ptr<CompositeTransformation> composite;
    REQUIRE_NOTHROW(composite = std::make_shared<CompositeTransformation>(transformations));

    transformations[0] = nullptr;
    REQUIRE_THROWS_AS(std::make_shared<CompositeTransformation>(transformations), std::invalid_argument);
}

TEST_CASE("CompositeTransformationGetterTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite(transformations);
    REQUIRE_NOTHROW(composite.getTransformations());
    REQUIRE(composite.getTransformations() == transformations);
}

TEST_CASE("CompositeTransformationSetTransformationsTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite;
    REQUIRE_NOTHROW(composite.setTransformations(transformations));
    REQUIRE(composite.getTransformations() == transformations);

    transformations[0] = nullptr;
    REQUIRE_THROWS_AS(composite.setTransformations(transformations), std::invalid_argument);
}

TEST_CASE("CompositeTransformationPolymorphicOperator==Test", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite1(transformations);
    CompositeTransformation composite2(transformations);
    Transformation& composite1Ref = composite1;
    Transformation& composite2Ref = composite2;

    //equal
    REQUIRE(composite1Ref == composite2Ref);
    REQUIRE(!(composite1Ref != composite2Ref));

    std::shared_ptr<Transformation> transformation = std::make_shared<LeftTrimTransformation>();
    composite2.pushTransformation(transformation);

    //different length
    REQUIRE(!(composite1Ref == composite2Ref));
    REQUIRE(composite1Ref != composite2Ref);

    
    CensorTransformation censor("test");
    Transformation& censorRef = censor;
    
    //different type
    REQUIRE(!(composite1Ref == censorRef));
    REQUIRE(composite1Ref != censorRef);

    composite1.pushTransformation(std::make_shared<CensorTransformation>(censor));

    //same length and same type, but different elements
    REQUIRE(!(composite1Ref == composite2Ref));
    REQUIRE(composite1Ref != composite2Ref);
}

TEST_CASE("CompositeTransformationPushTransformationTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite(transformations);
    std::shared_ptr<Transformation> transformation = std::make_shared<LeftTrimTransformation>();
    REQUIRE_NOTHROW(composite.pushTransformation(transformation));
    transformations.push_back(transformation);
    REQUIRE(composite.getTransformations() == transformations);

    REQUIRE_THROWS_AS(composite.pushTransformation(nullptr), std::invalid_argument);
}

TEST_CASE("CompositeTransformationPopTransformationTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite(transformations);
    REQUIRE_NOTHROW(composite.popTransformation());
    transformations.pop_back();
    REQUIRE(composite.getTransformations() == transformations);
}

TEST_CASE("CompositeTransformationPopAtTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite(transformations);
    std::shared_ptr<Transformation> popped;

    REQUIRE_NOTHROW(popped = composite.popAt(0));
    REQUIRE(*popped == *transformations[0]);

    transformations.erase(transformations.begin());
    REQUIRE(composite.getTransformations() == transformations);

    REQUIRE_THROWS_AS(composite.popAt(1000), std::out_of_range);

    REQUIRE_NOTHROW(popped = composite.popAt(0));
    REQUIRE(*popped == *transformations[0]);
    REQUIRE_THROWS_AS(composite.popAt(0), std::out_of_range);
}

TEST_CASE("CompositeTransformationPeekAtTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite(transformations);
    std::shared_ptr<Transformation> peeked;

    REQUIRE_NOTHROW(peeked = composite.peekAt(0));
    REQUIRE(*peeked == *transformations[0]);

    REQUIRE_THROWS_AS(composite.peekAt(1000), std::out_of_range);

    REQUIRE_NOTHROW(peeked = composite.peekAt(1));
    REQUIRE(*peeked == *transformations[1]);
}

TEST_CASE("CompositeTransformationInsertAtTest", "[composite_transformation]") {
    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(std::make_shared<CensorTransformation>("test"));
    transformations.push_back(std::make_shared<ReplaceTransformation>("from", "to"));

    CompositeTransformation composite(transformations);

    std::shared_ptr<Transformation> transformation = std::make_shared<LeftTrimTransformation>();
    REQUIRE_NOTHROW(composite.insertAt(transformation, 1));
    REQUIRE(*composite.peekAt(1) == *transformation);

    transformation = std::make_shared<RightTrimTransformation>();
    REQUIRE_NOTHROW(composite.insertAt(transformation, 3));
    REQUIRE(*composite.peekAt(3) == *transformation);

    REQUIRE_THROWS_AS(composite.insertAt(transformation, 1000), std::out_of_range);
    REQUIRE_THROWS_AS(composite.insertAt(nullptr, 0), std::invalid_argument);
}

TEST_CASE("CompositeTransformationTransformTest", "[composite_transformation]") {
    std::shared_ptr<Transformation> t1 = std::make_shared<CensorTransformation>("test");
    std::shared_ptr<Transformation> t2 = std::make_shared<ReplaceTransformation>("from", "test");

    std::vector<std::shared_ptr<Transformation>> transformations;
    transformations.push_back(t1);
    transformations.push_back(t2);

    std::string text = "from test";

    CompositeTransformation composite(transformations);
    REQUIRE(composite.transform(text) == "test ****");

    transformations[0] = t2;
    transformations[1] = t1;
    composite.setTransformations(transformations);
    REQUIRE(composite.transform(text) == "**** ****"); //order matters
}