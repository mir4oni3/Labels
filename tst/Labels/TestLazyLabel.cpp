#include <catch2/catch_test_macros.hpp>

#include "../../imp/inc/Labels/LazyLabel.hpp"
#include "../../imp/inc/Labels/SimpleLabels.hpp"

#include <iostream>
#include <sstream>

class LazyLabelTest : public LazyLabel {
public:
    LazyLabelTest(const std::shared_ptr<Label>& label) : LazyLabel(label) {}
    LazyLabelTest(const std::shared_ptr<Label>& label, unsigned short timeout) : LazyLabel(label, timeout) {}

    std::shared_ptr<Label> getLabel() {
        return label;
    }

    short getRequestCount() {
        return requestCount;
    }

    unsigned short getTimeout() {
        return timeout;
    }

    void setTimeout(int value) {
        timeout = value;
    }
};

static std::string simulateIO(const std::shared_ptr<LazyLabel>& lazyLabel, const std::string& input) {
    std::istringstream stdinInput(input);
    std::ostringstream capturedOutput;

    //save the original streams
    std::streambuf* originalCin = std::cin.rdbuf();
    std::streambuf* originalCout = std::cout.rdbuf();

    //redirect streams to sstreams
    std::cin.rdbuf(stdinInput.rdbuf());
    std::cout.rdbuf(capturedOutput.rdbuf());

    //gets input from stdin, which is currently redirected to sstream and save output
    std::string out = lazyLabel->getText();
    std::cout << out << '\n';

    // Restore streams
    std::cin.rdbuf(originalCin);
    std::cout.rdbuf(originalCout);

    return capturedOutput.str();
}

TEST_CASE("LazyLabelDefaultConstructorTest", "[lazy_label]") {
    std::shared_ptr<LazyLabelTest> lazyLabel;
    REQUIRE_NOTHROW(lazyLabel = std::make_shared<LazyLabelTest>(std::make_shared<SimpleLabel>("test")));
    simulateIO(lazyLabel, "test");
    REQUIRE(lazyLabel->getText() == "test");
    REQUIRE(lazyLabel->getTimeout() == LazyLabelConstants::LAZY_LABEL_TIMEOUT);
}

TEST_CASE("LazyLabelTimeoutConstructorTest", "[lazy_label]") {
    std::shared_ptr<LazyLabelTest> lazyLabel;
    REQUIRE_NOTHROW(lazyLabel = std::make_shared<LazyLabelTest>(std::make_shared<SimpleLabel>("test"), 10));
    simulateIO(lazyLabel, "test");
    REQUIRE(lazyLabel->getText() == "test");
    REQUIRE(lazyLabel->getTimeout() == 10);
}

TEST_CASE("LazyLabelCorrectRequestCountTest", "[lazy_label]") {
    std::shared_ptr<LazyLabelTest> lazyLabel;
    REQUIRE_NOTHROW(lazyLabel = std::make_shared<LazyLabelTest>(std::make_shared<SimpleLabel>("test"), 10));
    REQUIRE(lazyLabel->getRequestCount() == -1); //-1 indicates no value is set yet
    simulateIO(lazyLabel, "test");
    REQUIRE(lazyLabel->getRequestCount() == 1);
    REQUIRE(lazyLabel->getText() == "test");
    REQUIRE(lazyLabel->getRequestCount() == 2);
    REQUIRE(lazyLabel->getText() == "test");
    REQUIRE(lazyLabel->getRequestCount() == 3);
}

TEST_CASE("LazyLabelNoNewTextTest", "[lazy_label]") {
    std::shared_ptr<LazyLabel> lazyLabel =  std::make_shared<LazyLabelTest>(std::make_shared<SimpleLabel>("test"), 3);
    REQUIRE(simulateIO(lazyLabel, "test") == "Enter value for label: \ntest\n");
    REQUIRE(simulateIO(lazyLabel, "") == "test\n");
    REQUIRE(simulateIO(lazyLabel, "n") == "Would you want to enter a new value for label? (y/n)\ntest\n");
}

TEST_CASE("LazyLabelNewTextTest", "[lazy_label]") {
    std::shared_ptr<LazyLabel> lazyLabel =  std::make_shared<LazyLabelTest>(std::make_shared<SimpleLabel>("test"), 3);
    REQUIRE(simulateIO(lazyLabel, "test") == "Enter value for label: \ntest\n");
    REQUIRE(simulateIO(lazyLabel, "") == "test\n");
    REQUIRE(simulateIO(lazyLabel, "y\nhello") == "Would you want to enter a new value for label? (y/n)\n"
                                                 "Enter value for label: \n"
                                                 "hello\n");
    REQUIRE(simulateIO(lazyLabel, "") == "hello\n");
}

TEST_CASE("LazyLabelPolymorphicOperator==Test", "[lazy_label]") {
    LazyLabel lazyLabel1(std::make_shared<SimpleLabel>("test"));
    LazyLabel lazyLabel2(std::make_shared<SimpleLabel>("test"));
    Label& label1ref = lazyLabel1;
    Label& label2ref = lazyLabel2;

    LazyLabel lazyLabel3(std::make_shared<SimpleLabel>("test"), LazyLabelConstants::LAZY_LABEL_TIMEOUT + 1);
    LazyLabel lazyLabel4(std::make_shared<SimpleLabel>("test"), LazyLabelConstants::LAZY_LABEL_TIMEOUT + 1);
    Label& label3ref = lazyLabel3;
    Label& label4ref = lazyLabel4;

    SimpleLabel simpleLabel("test");
    Label& simpleLabelRef = simpleLabel;

    REQUIRE(lazyLabel1 == lazyLabel2);
    REQUIRE(lazyLabel3 == lazyLabel4);
    REQUIRE(!(lazyLabel1 == lazyLabel3));
    REQUIRE(!(lazyLabel1 == simpleLabelRef));
}