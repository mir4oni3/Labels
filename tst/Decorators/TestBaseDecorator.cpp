#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Labels/SimpleLabels.hpp"
#include "../../imp/inc/Decorators/BaseDecorator.hpp"

#include <iostream>

class BaseDecoratorStub : public BaseDecorator {
public:
    BaseDecoratorStub(const std::shared_ptr<Label>& label) : BaseDecorator(label) {}

    std::string getText() const override {
        return "";
    }
};

class BaseDecoratorStub2 : public BaseDecorator {
public:
    BaseDecoratorStub2(const std::shared_ptr<Label>& label) : BaseDecorator(label) {}

    std::string getText() const override {
        return "";
    }
};

class BaseDecoratorStub3 : public BaseDecorator {
public:
    BaseDecoratorStub3(const std::shared_ptr<Label>& label) : BaseDecorator(label) {}

    std::string getText() const override {
        return "";
    }
};

TEST_CASE("BaseDecoratorConstructorAndGetLabelTest", "[base_decorator]") {
    std::shared_ptr<BaseDecoratorStub> decorator;
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");

    REQUIRE_THROWS_AS(BaseDecoratorStub(nullptr), std::invalid_argument);

    REQUIRE_NOTHROW(decorator = std::make_shared<BaseDecoratorStub>(label));
    REQUIRE(*decorator->getLabel() == *label);
}

TEST_CASE("BaseDecoratorOperator==Test", "[base_decorator]") {
    std::shared_ptr<Label> label1 = std::make_shared<SimpleLabel>("label");
    std::shared_ptr<Label> label2 = std::make_shared<SimpleLabel>("label");
    std::shared_ptr<BaseDecoratorStub> decorator1 = std::make_shared<BaseDecoratorStub>(label1);
    std::shared_ptr<BaseDecoratorStub> decorator2 = std::make_shared<BaseDecoratorStub>(label2);

    //equal
    REQUIRE(*decorator1 == *decorator2);

    //same type but different labels
    decorator2->setLabel(std::make_shared<SimpleLabel>("label2"));
    REQUIRE(!(*decorator1 == *decorator2));
    REQUIRE(*decorator1 != *decorator2);
    
    //different types
    std::shared_ptr<BaseDecoratorStub2> decorator3 = std::make_shared<BaseDecoratorStub2>(label1);
    REQUIRE(!(*decorator1 == *decorator3));
    REQUIRE(*decorator1 != *decorator3);
}

TEST_CASE("BaseDecoratorSetLabelTest", "[base_decorator]") {
    BaseDecoratorStub decorator(std::make_shared<SimpleLabel>("label"));
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label2");

    REQUIRE_NOTHROW(decorator.setLabel(label));
    REQUIRE(*decorator.getLabel() == *label);
}

TEST_CASE("BaseDecoratorGetUnderlyingLabelTest", "[base_decorator]") {
    std::shared_ptr<BaseDecorator> decorator = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);

    std::shared_ptr<Label> expectedUnderlying = std::make_shared<SimpleLabel>("label");
    REQUIRE(*decorator->getUnderlyingLabel() == *expectedUnderlying);
}

TEST_CASE("BaseDecoratorRemoveLastDecorationTest", "[base_decorator]") {
    std::shared_ptr<BaseDecorator> decorator = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));

    REQUIRE_THROWS_AS(removeLastDecoration(decorator), std::runtime_error);

    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);

    std::shared_ptr<BaseDecorator> expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    REQUIRE_NOTHROW(decorator = removeLastDecoration(decorator));
    REQUIRE(typeid(*decorator) == typeid(BaseDecoratorStub));
    REQUIRE_NOTHROW(decorator = removeLastDecoration(decorator));
    REQUIRE(typeid(*decorator) == typeid(BaseDecoratorStub2));
}

TEST_CASE("BaseDecoratorRemoveAtIndexTest" , "[base_decorator]") {
    std::shared_ptr<BaseDecorator> decorator = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));

    REQUIRE_THROWS_AS(removeAtIndex(decorator, 0), std::runtime_error);

    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);

    REQUIRE_THROWS_AS(removeAtIndex(decorator, 7), std::out_of_range);

    REQUIRE_NOTHROW(decorator = removeAtIndex(decorator, 0));
    std::shared_ptr<BaseDecorator> expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    REQUIRE(*decorator == *expected);

    REQUIRE_NOTHROW(decorator = removeAtIndex(decorator, 2));
    expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    REQUIRE(*decorator == *expected);

    REQUIRE_NOTHROW(decorator = removeAtIndex(decorator, 4));
    expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    REQUIRE(*decorator == *expected);

    REQUIRE_NOTHROW(decorator = removeAtIndex(decorator, 2));
    expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    REQUIRE(*decorator == *expected);
}

TEST_CASE("BaseDecoratorRemoveTypeTest", "[base_decorator]") {
    std::shared_ptr<BaseDecorator> decorator = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));

    //tried removing single remaining decorator
    std::shared_ptr<BaseDecorator> toRemove = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    REQUIRE_THROWS_AS(removeType(decorator, toRemove), std::runtime_error);

    //no matching decoration to remove
    toRemove = std::make_shared<BaseDecoratorStub2>(std::make_shared<SimpleLabel>("hello"));
    REQUIRE_THROWS_AS(removeType(decorator, toRemove), std::logic_error);

    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    decorator = std::make_shared<BaseDecoratorStub>(decorator);

    toRemove = std::make_shared<BaseDecoratorStub>(std::make_shared<SimpleLabel>("hi"));
    std::shared_ptr<BaseDecorator> expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    REQUIRE_NOTHROW(decorator = removeType(decorator, toRemove));
    REQUIRE(*decorator == *expected);

    toRemove = std::make_shared<BaseDecoratorStub>(std::make_shared<SimpleLabel>("bye"));
    expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    REQUIRE_NOTHROW(decorator = removeType(decorator, toRemove));
    REQUIRE(*decorator == *expected);

    toRemove = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("wow"));
    expected = std::make_shared<BaseDecoratorStub2>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    expected = std::make_shared<BaseDecoratorStub2>(expected);
    REQUIRE_NOTHROW(decorator = removeType(decorator, toRemove));
    REQUIRE(*decorator == *expected);
}

TEST_CASE("BaseDecoratorRemoveSpecificTest", "[base_decorator]") {
    std::shared_ptr<BaseDecorator> decorator = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));

    //tried removing single remaining decorator
    std::shared_ptr<BaseDecorator> toRemove = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    REQUIRE_THROWS_AS(removeSpecific(decorator, toRemove), std::runtime_error);

    //no matching decoration to remove(same type but different label text)
    toRemove = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("hello"));
    REQUIRE_THROWS_AS(removeSpecific(decorator, toRemove), std::logic_error);

    decorator = std::make_shared<BaseDecoratorStub>(decorator);
    decorator = std::make_shared<BaseDecoratorStub2>(decorator);
    
    //the label should be BaseDecoratorStub, not SimpleLabel
    toRemove = std::make_shared<BaseDecoratorStub2>(std::make_shared<SimpleLabel>("label"));
    REQUIRE_THROWS_AS(removeSpecific(decorator, toRemove), std::logic_error);

    //The label is correct, so it should be removed
    toRemove = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    toRemove = std::make_shared<BaseDecoratorStub>(toRemove);
    toRemove = std::make_shared<BaseDecoratorStub2>(toRemove);
    REQUIRE_NOTHROW(decorator = removeSpecific(decorator, toRemove));
    std::shared_ptr<BaseDecorator> expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    REQUIRE(*decorator == *expected);
}

TEST_CASE("BaseDecoratorPushDecorationTest", "[base_decorator]") {
    std::shared_ptr<BaseDecorator> decorator = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    std::shared_ptr<BaseDecorator> toPush = std::make_shared<BaseDecoratorStub>(std::make_shared<SimpleLabel>("label2"));

    REQUIRE_THROWS_AS(pushDecoration(decorator, nullptr), std::invalid_argument);

    std::shared_ptr<BaseDecorator> nullDecorator = nullptr;
    REQUIRE_THROWS_AS(pushDecoration(nullDecorator, toPush), std::invalid_argument);
    
    std::shared_ptr<BaseDecorator> expected;
    expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);

    decorator = pushDecoration(decorator, toPush);
    REQUIRE(*decorator == *expected);
}

TEST_CASE("BaseDecoratorInsertDecorationTest", "[base_decorator]") {
    std::shared_ptr<BaseDecorator> decorator = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    std::shared_ptr<BaseDecorator> toInsert = std::make_shared<BaseDecoratorStub>(std::make_shared<SimpleLabel>("label2"));
    std::shared_ptr<BaseDecorator> expected;

    //Invalid args
    std::shared_ptr<BaseDecorator> nullDecorator = nullptr;
    REQUIRE_THROWS_AS(insertDecoration(decorator, 0, nullDecorator), std::invalid_argument);
    REQUIRE_THROWS_AS(insertDecoration(nullDecorator, 0, toInsert), std::invalid_argument);
    REQUIRE_THROWS_AS(insertDecoration(decorator, 1000, toInsert), std::out_of_range);

    //insert at the end
    toInsert = std::make_shared<BaseDecoratorStub>(std::make_shared<SimpleLabel>("label2"));
    expected = std::make_shared<BaseDecoratorStub3>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    REQUIRE_NOTHROW(decorator = insertDecoration(decorator, 1, toInsert));
    REQUIRE(*decorator == *expected);

    //insert at the start
    toInsert = std::make_shared<BaseDecoratorStub2>(std::make_shared<SimpleLabel>("label2"));
    expected = std::make_shared<BaseDecoratorStub2>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub3>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    REQUIRE_NOTHROW(decorator = insertDecoration(decorator, 0, toInsert));
    REQUIRE(*decorator == *expected);

    //insert in the middle
    toInsert = std::make_shared<BaseDecoratorStub>(std::make_shared<SimpleLabel>("label3"));
    expected = std::make_shared<BaseDecoratorStub2>(std::make_shared<SimpleLabel>("label"));
    expected = std::make_shared<BaseDecoratorStub>(expected);
    expected = std::make_shared<BaseDecoratorStub3>(expected);
    expected = std::make_shared<BaseDecoratorStub>(expected);
    REQUIRE_NOTHROW(decorator = insertDecoration(decorator, 1, toInsert));
    REQUIRE(*decorator == *expected);
}