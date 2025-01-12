#include <catch2/catch_test_macros.hpp>

#include <set>
#include <iostream>

#include "../../imp/inc/Decorators/ConcreteDecorators.hpp"
#include "../../imp/inc/Labels/SimpleLabels.hpp"
#include "../../imp/inc/Transformations/CensorTransformation.hpp"
#include "../../imp/inc/Transformations/ReplaceTransformation.hpp"

class RandomDecoratorTest : public RandomDecorator {
public:
    RandomDecoratorTest(const std::shared_ptr<Label>& label, const std::vector<std::shared_ptr<Transformation>>& transformations) : RandomDecorator(label, transformations) {}
    const int getCurrent() const {
        return current;
    }
    void setCurrent(int value) {
        current = value;
    }
    const std::vector<int>& getOrder() const {
        return order;
    }
    void setOrder(const std::vector<int>& value) {
        order = value;
    }
};

class RandomDecoratorTest2 : public RandomDecorator {
public:
    RandomDecoratorTest2(const std::shared_ptr<Label>& label, const std::vector<std::shared_ptr<Transformation>>& transformations) : RandomDecorator(label, transformations) {}
    const int getCurrent() const {
        return current;
    }
    void setCurrent(int value) {
        current = value;
    }
    const std::vector<int>& getOrder() const {
        return order;
    }
    void setOrder(const std::vector<int>& value) {
        order = value;
    }
};



TEST_CASE("SimpleDecoratorConstructorAndGetterTest", "[simple_decorator]") {
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");
    std::shared_ptr<Transformation> transformation = std::make_shared<CensorTransformation>("transformation");

    REQUIRE_THROWS_AS(SimpleDecorator(label, nullptr), std::invalid_argument);
    REQUIRE_THROWS_AS(SimpleDecorator(nullptr, transformation), std::invalid_argument);

    std::shared_ptr<SimpleDecorator> simpleDecorator;
    REQUIRE_NOTHROW(simpleDecorator = std::make_shared<SimpleDecorator>(label, transformation));

    REQUIRE_NOTHROW(simpleDecorator->getLabel());
    REQUIRE_NOTHROW(simpleDecorator->getTransformation());
    REQUIRE(*simpleDecorator->getLabel() == *label);
    REQUIRE(*simpleDecorator->getTransformation() == *transformation);
}

TEST_CASE("SimpleDecoratorSetTransformationTest", "[simple_decorator]") {
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");
    std::shared_ptr<Transformation> transformation = std::make_shared<CensorTransformation>("transformation");
    SimpleDecorator simpleDecorator(label, transformation);

    REQUIRE_THROWS_AS(simpleDecorator.setTransformation(nullptr), std::invalid_argument);

    std::shared_ptr<Transformation> newTransformation = std::make_shared<ReplaceTransformation>("from", "to");
    REQUIRE_NOTHROW(simpleDecorator.setTransformation(newTransformation));
    REQUIRE(*simpleDecorator.getTransformation() == *newTransformation);
}

TEST_CASE("SimpleDecoratorGetTextTest", "[simple_decorator]") {
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("test");
    std::shared_ptr<Transformation> transformation = std::make_shared<CensorTransformation>("test");
    SimpleDecorator simpleDecorator(label, transformation);
    REQUIRE(simpleDecorator.getText() == "****");
}

TEST_CASE("RandomDecoratorConstructorAndGetterTest", "[random_decorator]") {
    srand(42);

    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");
    std::vector<std::shared_ptr<Transformation>> transformations = {
            std::make_shared<CensorTransformation>("transformation1"),
            std::make_shared<ReplaceTransformation>("from1", "to1"),
            std::make_shared<CensorTransformation>("transformation2")
    };

    REQUIRE_THROWS_AS(RandomDecorator(label, {nullptr}), std::invalid_argument);
    REQUIRE_THROWS_AS(RandomDecorator(nullptr, transformations), std::invalid_argument);

    std::shared_ptr<RandomDecorator> decorator;
    REQUIRE_NOTHROW(decorator = std::make_shared<RandomDecorator>(label, transformations));
    REQUIRE_NOTHROW(decorator->getLabel());
    REQUIRE_NOTHROW(decorator->getTransformation());
    REQUIRE(*decorator->getLabel() == *label);
    REQUIRE(decorator->getTransformation() == transformations);
}

TEST_CASE("RandomDecoratorSetTransformationTest", "[random_decorator]") {
    srand(42);

    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");
    std::vector<std::shared_ptr<Transformation>> trans = {
            std::make_shared<CensorTransformation>("transformation2")
    };

    RandomDecorator decorator(label, trans);

    std::vector<std::shared_ptr<Transformation>> newTrans = {
            std::make_shared<CensorTransformation>("transformation1"),
            std::make_shared<ReplaceTransformation>("from1", "to1"),
            std::make_shared<CensorTransformation>("transformation2")
    };

    REQUIRE_NOTHROW(decorator.setTransformation(newTrans));
    REQUIRE(decorator.getTransformation() == newTrans);

    newTrans = {
            std::make_shared<CensorTransformation>("transformation1"),
            nullptr,
            std::make_shared<CensorTransformation>("transformation2")
    };

    REQUIRE_THROWS_AS(decorator.setTransformation(newTrans), std::invalid_argument);
}

TEST_CASE("RandomDecoratorOperator==Test", "[random_decorator]") {
    srand(42);

    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");
    std::vector<std::shared_ptr<Transformation>> transformation = {
            std::make_shared<CensorTransformation>("transformation1"),
            std::make_shared<ReplaceTransformation>("from1", "to1"),
            std::make_shared<CensorTransformation>("transformation2")
    };

    RandomDecoratorTest decorator1(label, transformation);
    RandomDecoratorTest decorator2(label, transformation);

    decorator1.setOrder({1, 2, 0});
    decorator2.setOrder({1, 2, 0});

    BaseDecorator& decorator1ref = decorator1;
    BaseDecorator& decorator2ref = decorator2;

    //equal
    REQUIRE(decorator1ref == decorator2ref);
    REQUIRE(!(decorator1ref != decorator2ref));

    decorator2.getText();

    //not equal because current counter is different
    REQUIRE(!(decorator1ref == decorator2ref));
    REQUIRE(decorator1ref != decorator2ref);

    decorator1.setCurrent(decorator2.getCurrent()); //current is now the same

    decorator1.setOrder({1, 0, 2});

    //not equal because order is different
    REQUIRE(!(decorator1ref == decorator2ref));
    REQUIRE(decorator1ref != decorator2ref);

    RandomDecoratorTest2 decorator3(label, transformation);
    decorator3.setOrder(decorator1.getOrder());
    decorator3.setCurrent(decorator1.getCurrent());
    BaseDecorator& decorator3ref = decorator3;

    //not equal because different types
    REQUIRE(!(decorator1ref == decorator3ref));
    REQUIRE(decorator1ref != decorator3ref);
}

TEST_CASE("RandomDecoratorGetTextTest", "[random_decorator]") {
    srand(42);
    
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("hello bye");
    std::vector<std::shared_ptr<Transformation>> transformations;

    RandomDecorator decorator(label, transformations);
    REQUIRE(decorator.getText() == "hello bye");

    transformations = {
            std::make_shared<CensorTransformation>("hello"),
            std::make_shared<ReplaceTransformation>("hello", "bye"),
            std::make_shared<CensorTransformation>("bye")
    };

    decorator = RandomDecorator(label, transformations);

    std::set<std::string> expected;
    std::string text;

    std::set<std::vector<std::string>> orders;
    std::vector<std::string> currentOrder;

    
    //very small chance to pick 3 different transformations 20 times in a row
    //in the case of a bug where transformations are repicked before reshuffling
    for (int i = 0; i < 20; i++) {
        expected = {"***** bye", "bye bye", "hello ***"};

        text = decorator.getText();
        currentOrder.push_back(text);
        REQUIRE(expected.find(text) != expected.end());
        expected.erase(text);

        text = decorator.getText();
        currentOrder.push_back(text);
        REQUIRE(expected.find(text) != expected.end());
        expected.erase(text);

        text = decorator.getText();
        currentOrder.push_back(text);
        REQUIRE(expected.find(text) != expected.end());
        expected.erase(text);

        orders.emplace(currentOrder);
        currentOrder.clear();
    }

    //very small chance to pick the same order 20 times in a row
    //this guarantees the randomness of the selected transformation
    REQUIRE(orders.size() > 1);
}

TEST_CASE("RepeatingDecoratorConstructorAndGetterTest", "[repeating_decorator]") {
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");
    std::vector<std::shared_ptr<Transformation>> transformations = {
            std::make_shared<CensorTransformation>("transformation1"),
            std::make_shared<ReplaceTransformation>("from1", "to1"),
            std::make_shared<CensorTransformation>("transformation2")
    };

    REQUIRE_THROWS_AS(RepeatingDecorator(label, {nullptr}), std::invalid_argument);
    REQUIRE_THROWS_AS(RepeatingDecorator(nullptr, transformations), std::invalid_argument);

    std::shared_ptr<RepeatingDecorator> decorator;
    REQUIRE_NOTHROW(decorator = std::make_shared<RepeatingDecorator>(label, transformations));
    REQUIRE_NOTHROW(decorator->getLabel());
    REQUIRE_NOTHROW(decorator->getTransformation());
    REQUIRE(*decorator->getLabel() == *label);
    REQUIRE(decorator->getTransformation() == transformations);
}

TEST_CASE("RepeatingDecoratorSetTransformationTest", "[repeating_decorator]") {
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");
    std::vector<std::shared_ptr<Transformation>> trans = {
            std::make_shared<CensorTransformation>("transformation2")
    };

    RepeatingDecorator decorator(label, trans);

    std::vector<std::shared_ptr<Transformation>> newTrans = {
            std::make_shared<CensorTransformation>("transformation1"),
            std::make_shared<ReplaceTransformation>("from1", "to1"),
            std::make_shared<CensorTransformation>("transformation2")
    };

    REQUIRE_NOTHROW(decorator.setTransformation(newTrans));
    REQUIRE(decorator.getTransformation() == newTrans);

    newTrans = {
            std::make_shared<CensorTransformation>("transformation1"),
            nullptr,
            std::make_shared<CensorTransformation>("transformation2")
    };

    REQUIRE_THROWS_AS(decorator.setTransformation(newTrans), std::invalid_argument);
}

TEST_CASE("RepeatingDecoratorGetTextTest", "[repeating_decorator]") {
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("hello bye");
    std::vector<std::shared_ptr<Transformation>> transformations;

    RepeatingDecorator decorator(label, transformations);
    REQUIRE(decorator.getText() == "hello bye");

    transformations = {
            std::make_shared<CensorTransformation>("hello"),
            std::make_shared<ReplaceTransformation>("hello", "bye"),
            std::make_shared<CensorTransformation>("bye")
    };

    decorator = RepeatingDecorator(label, transformations);

    for (int i = 0; i < 20; i++) {
        REQUIRE(decorator.getText() == "***** bye");
        REQUIRE(decorator.getText() == "bye bye");
        REQUIRE(decorator.getText() == "hello ***");
    }
}