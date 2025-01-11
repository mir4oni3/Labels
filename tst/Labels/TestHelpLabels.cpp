#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Labels/HelpLabels.hpp"
#include "../../imp/inc/Labels/SimpleLabels.hpp"

class HelpLabelStub : public HelpLabel {
public:
    HelpLabelStub(const std::shared_ptr<Label>& label) : HelpLabel(label) {}
    virtual std::string getHelp() const override;
};

std::string HelpLabelStub::getHelp() const {
    return "test";
}

TEST_CASE("HelpLabelConstructorTest", "[help_label]") {
    std::shared_ptr<HelpLabelStub> helpLabel;
    REQUIRE_NOTHROW(helpLabel = std::make_shared<HelpLabelStub>(std::make_shared<SimpleLabel>("test")));
    REQUIRE(helpLabel->getText() == "test");
}

TEST_CASE("HelpLabelPolymorphicOperator==Test", "[help_label]") {
    HelpLabelStub helpLabel1(std::make_shared<SimpleLabel>("test"));
    HelpLabelStub helpLabel2(std::make_shared<SimpleLabel>("test"));
    Label& helpLabel1ref = helpLabel1;
    Label& helpLabel2ref = helpLabel2;

    SECTION("Equal") {
        REQUIRE(helpLabel1ref == helpLabel2ref);
    }
    SECTION("Not Equal text") {
        helpLabel2.setText("hello");
        REQUIRE(!(helpLabel1ref == helpLabel2ref));
    }
    SECTION("Not Equal type") {
        SimpleLabel labelOtherType("test");
        Label& labelOtherTypeRef = labelOtherType;
        REQUIRE(!(helpLabel1ref == labelOtherTypeRef));
    }
}

TEST_CASE("HelpLabelGetTextTest", "[help_label]") {
    HelpLabelStub helpLabel(std::make_shared<SimpleLabel>("test"));
    REQUIRE(helpLabel.getText() == "test");
}

TEST_CASE("HelpLabelSetTextTest", "[help_label]") {
    HelpLabelStub helpLabel(std::make_shared<SimpleLabel>("test"));
    helpLabel.setText("hello");
    REQUIRE(helpLabel.getText() == "hello");
}

TEST_CASE("HelpLengthLabelConstructorTest", "[help_length_label]") {
    std::shared_ptr<HelpLengthLabel> helpLengthLabel;
    REQUIRE_NOTHROW(helpLengthLabel = std::make_shared<HelpLengthLabel>(std::make_shared<SimpleLabel>("test")));
    REQUIRE(helpLengthLabel->getText() == "test");
}

TEST_CASE("HelpLengthLabelGetHelpTest1", "[help_length_label]") {
    HelpLengthLabel helpLengthLabel(std::make_shared<SimpleLabel>("test"));
    REQUIRE(helpLengthLabel.getHelp() == "Length of the text is 4");
}

TEST_CASE("HelpLengthLabelGetHelpTest2", "[help_length_label]") {
    HelpLengthLabel helpLengthLabel(std::make_shared<SimpleLabel>(""));
    REQUIRE(helpLengthLabel.getHelp() == "Length of the text is 0");
}

TEST_CASE("HelpWordCountLabelConstructorTest", "[help_word_count_label]") {
    std::shared_ptr<HelpWordCountLabel> helpWordCountLabel;
    REQUIRE_NOTHROW(helpWordCountLabel = std::make_shared<HelpWordCountLabel>(std::make_shared<SimpleLabel>("test")));
    REQUIRE(helpWordCountLabel->getText() == "test");
}

TEST_CASE("HelpWordCountLabelGetHelpTest1", "[help_word_count_label]") {
    HelpWordCountLabel helpWordCountLabel(std::make_shared<SimpleLabel>("test"));
    REQUIRE(helpWordCountLabel.getHelp() == "Word count is 1");
}

TEST_CASE("HelpWordCountLabelGetHelpTest2", "[help_word_count_label]") {
    HelpWordCountLabel helpWordCountLabel(std::make_shared<SimpleLabel>("test test"));
    REQUIRE(helpWordCountLabel.getHelp() == "Word count is 2");
}

TEST_CASE("HelpWordCountLabelGetHelpTest3", "[help_word_count_label]") {
    HelpWordCountLabel helpWordCountLabel(std::make_shared<SimpleLabel>(""));
    REQUIRE(helpWordCountLabel.getHelp() == "Word count is 0");
}

TEST_CASE("HelpWordCountLabelGetHelpTest4", "[help_word_count_label]") {
    HelpWordCountLabel helpWordCountLabel(std::make_shared<SimpleLabel>(" "));
    REQUIRE(helpWordCountLabel.getHelp() == "Word count is 0");
}

TEST_CASE("HelpWordCountLabelGetHelpTest5", "[help_word_count_label]") {
    HelpWordCountLabel helpWordCountLabel(std::make_shared<SimpleLabel>("  test   test   "));
    REQUIRE(helpWordCountLabel.getHelp() == "Word count is 2");
}