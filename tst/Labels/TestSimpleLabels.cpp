#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Labels/SimpleLabels.hpp"

TEST_CASE("SimpleLabelConstructorAndGetterTest", "[simple_label]") {
    std::shared_ptr<SimpleLabel> label;
    REQUIRE_NOTHROW(label = std::make_shared<SimpleLabel>("test"));
    REQUIRE(label->getText() == "test");
}

TEST_CASE("RichLabelConstructorAndGetterTest", "[rich_label]") {
    std::shared_ptr<RichLabel> label;
    REQUIRE_NOTHROW(label = std::make_shared<RichLabel>("test", "red", 12, "Arial"));
    REQUIRE(label->getText() == "<font color=\"red\" size=\"12\" face=\"Arial\">test</font>\n");
    REQUIRE(label->getColor() == "red");
    REQUIRE(label->getFontSize() == 12);
    REQUIRE(label->getFontName() == "Arial");
}

TEST_CASE("RichLabelSetterTest", "[rich_label]") {
    RichLabel label("test", "red", 12, "Arial");
    label.setColor("blue");
    label.setFontSize(14);
    label.setFontName("Times New Roman");
    REQUIRE(label.getText() == "<font color=\"blue\" size=\"14\" face=\"Times New Roman\">test</font>\n");
    REQUIRE(label.getColor() == "blue");
    REQUIRE(label.getFontSize() == 14);
    REQUIRE(label.getFontName() == "Times New Roman");
}

TEST_CASE("RichLabelPolymorphicOperator==Test", "[rich_label]") {
    RichLabel label1("test", "red", 12, "Arial");
    RichLabel label2("test", "red", 12, "Arial");
    Label& label1ref = label1;
    Label& label2ref = label2;

    SECTION("Not Equal text") {
        label2.setText("hello");
        REQUIRE(!(label1ref == label2ref));
        label2.setText("test");
    }
    SECTION("Not Equal color") {
        label2.setColor("blue");
        REQUIRE(!(label1ref == label2ref));
        label2.setColor("red");
    }
    SECTION("Not Equal font size") {
        label2.setFontSize(14);
        REQUIRE(!(label1ref == label2ref));
        label2.setFontSize(12);
    }
    SECTION("Not Equal font name") {
        label2.setFontName("Times New Roman");
        REQUIRE(!(label1ref == label2ref));
        label2.setFontName("Arial");
    }
    SECTION("Not Equal type") {
        SimpleLabel labelOtherType("test");
        Label& labelOtherTypeRef = labelOtherType;
        REQUIRE(!(label1ref == labelOtherTypeRef));
    }
    SECTION("Equal") {
        REQUIRE(label1ref == label2ref);
    }
}