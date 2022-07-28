#include "gtest/gtest.h"
#include "../../globals/Utils.cpp"
#include <string>

// Test functionality of absolute value method; .abs().
TEST(UtilsTestSuite, TestAbsoluteValue)
{

    ASSERT_EQ(1, Utils::getInstance().abs(-1));
    ASSERT_EQ(9, Utils::getInstance().abs(9));
    ASSERT_EQ(0, Utils::getInstance().abs(0));

}

// Test functionality of split by method; .splitBy()
TEST(UtilsTestSuite, TestSplitBy)
{

    const std::string temp{"hello;world"};
    const char delim = ';';
    auto result = *(Utils::getInstance().splitBy(temp, delim));
    ASSERT_EQ("hello", result.get<0>());
    ASSERT_EQ("world", result.get<1>());

}

// Test functionality of string double validation method; .isStringDouble()
TEST(UtilsTestSuite, TestStringDouble)
{

    const std::string temp{"5.0"};
    const std::string temp2{"23"};
    ASSERT_EQ(false, Utils::getInstance().isStringDouble(temp2));
    ASSERT_EQ(true, Utils::getInstance().isStringDouble(temp));

}

// Test functionality of string integer validation method; .isStringInteger()
TEST(UtilsTestSuite, TestStringInteger)
{

    const std::string temp{"5.2"};
    const std::string temp2{"23"};
    ASSERT_EQ(false, Utils::getInstance().isStringInteger(temp));
    ASSERT_EQ(true, Utils::getInstance().isStringInteger(temp2));

}

// Test functionality of is leap year validation method; .isLeap()
TEST(UtilsTestSuite, TestIsLeapYear)
{

    int leapYear = 1988;
    int notLeapYear = 1987;
    ASSERT_EQ(true, Utils::getInstance().isLeap(leapYear));
    ASSERT_EQ(false, Utils::getInstance().isLeap(notLeapYear));

}

TEST(UtilsTestSuite, TestIsStringDate)
{

    const std::string validStringDate{"21-05-1996"};
    const std::string notValidStringDate{"100-05-1996"};
    const std::string alsoNotValidStringDate{"-100-05-1996"};
    const std::string anotherInvalidDate{"1996-05-21"};
    ASSERT_EQ(true, Utils::getInstance().isStringDate(validStringDate));
    ASSERT_EQ(false, Utils::getInstance().isStringDate(notValidStringDate));
    ASSERT_EQ(false, Utils::getInstance().isStringDate(alsoNotValidStringDate));
    ASSERT_EQ(false, Utils::getInstance().isStringDate(anotherInvalidDate));

}

int main(int argc, char** argv)
{

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}