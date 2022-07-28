#include "gtest/gtest.h"
#include "../../data/Medicine.cpp"

// Initialize two instances of an Medicine object and confirm that the properties match
// given the compiler-generated copy constructor.
TEST(MedicineModelTest, CopiedValues)
{

    Medicine product1{0, "Product 1", "Company 1", "07-27-22", "07-27-22", 5.2, 7};
    Medicine product2{product1};
    ASSERT_EQ(product1._id, product2._id);
    ASSERT_EQ(product1._name, product2._name);
    ASSERT_EQ(product1._ownedBy, product2._ownedBy);
    ASSERT_EQ(product1._arrivalDate, product2._arrivalDate);
    ASSERT_EQ(product1._expirationDate, product2._expirationDate);
    ASSERT_EQ(product1._price, product2._price);
    ASSERT_EQ(product1._qty, product2._qty);

}

// Initialize a single instance of a Medicine object and confirm that the default values
// are assigned as expected.
TEST(MedicineModelTest, DefaultInitialization)
{

    Medicine product1;
    ASSERT_EQ(0, product1._id);
    ASSERT_EQ("", product1._name);
    ASSERT_EQ("", product1._ownedBy);
    ASSERT_EQ("", product1._arrivalDate);
    ASSERT_EQ("", product1._expirationDate);
    ASSERT_EQ(0.0, product1._price);
    ASSERT_EQ(0, product1._qty);

}

int main(int argc, char** argv)
{

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}