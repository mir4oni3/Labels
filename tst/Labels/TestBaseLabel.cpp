#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Labels/BaseLabel.hpp"

class BaseLabelStub : public Label {
public:
    virtual std::string getText() const override;
};

std::string BaseLabelStub::getText() const {
    return value;
}

class BaseLabelStub2 : public Label {
public:
    virtual std::string getText() const override;
};

std::string BaseLabelStub2::getText() const {
    return value;
}

TEST_CASE("BaseLabelAccessorTest", "[base_label]") {
    BaseLabelStub label;
    label.setText("test");
    REQUIRE(label.getText() == "test");
}

TEST_CASE("BaseLabelOperator==Test", "[base_label]") {
    BaseLabelStub label1;
    BaseLabelStub label2;
    SECTION("Equal") {
        label1.setText("test");
        label2.setText("test");
        REQUIRE(label1 == label2);
    }
    SECTION("Not Equal text") {
        label1.setText("test");
        label2.setText("hello");
        REQUIRE(!(label1 == label2));
    }
    SECTION("Not Equal type") {
        label1.setText("test");

        BaseLabelStub2 labelOtherType;
        labelOtherType.setText("test");

        REQUIRE(!(label1 == labelOtherType));
    }
}

TEST_CASE("BaseLabelOperator!=Test", "[base_label]") {
    BaseLabelStub label1;
    BaseLabelStub label2;
    SECTION("Equal") {
        label1.setText("test");
        label2.setText("test");
        REQUIRE(!(label1 != label2));
    }
    SECTION("Not Equal text") {
        label1.setText("test");
        label2.setText("hello");
        REQUIRE(label1 != label2);
    }
    SECTION("Not Equal type") {
        label1.setText("test");

        BaseLabelStub2 labelOtherType;
        labelOtherType.setText("test");

        REQUIRE(label1 != labelOtherType);
    }
}