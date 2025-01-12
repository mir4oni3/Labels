#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Labels/SimpleLabels.hpp"
#include "../../imp/inc/Transformations/CensorTransformation.hpp"
#include "../../imp/inc/Decorators/BaseDecorator.hpp"

class BaseDecoratorStub : public BaseDecorator {
public:
    BaseDecoratorStub(const std::shared_ptr<Label>& label) : BaseDecorator(label) {}

    std::string getText() const override {
        return "";
    }
};

TEST_CASE("BaseDecoratorConstructorAndGetterTest", "[base_decorator]") {
    std::shared_ptr<BaseDecoratorStub> decorator;
    std::shared_ptr<Label> label = std::make_shared<SimpleLabel>("label");

    REQUIRE_THROWS_AS(BaseDecoratorStub(nullptr), std::invalid_argument);

    REQUIRE_NOTHROW(decorator = std::make_shared<BaseDecoratorStub>(label));
}
