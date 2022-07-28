#include "gtest/gtest.h"
#include "../../data/Medicine.cpp"
#include "../../data/MedicineManager.cpp"

// This test method will test the functionality of the following methods with MedicineManager.cpp:
//
//  .add()
//  .contains()
//  .getData()
//  .clear()
//  .getById()
//  .removeById()
TEST(MedicineManagerModelTest, TestFunc)
{

    // Loop till 9 inclusive and invoke the .add() method to fill the MedicineManager object(s) data collection.
    for(int i = 0; i < 10; i++)
    {

        Medicine temp{static_cast<unsigned int>(i), "", "", "", "", 2.5, i + 2};
        MedicineManager::getInstance().add(temp);
        
    }

    // Confirm size of collection is 10 following sequence of .add() operations.
    ASSERT_EQ(10, MedicineManager::getInstance().getData().size());

    // Collection should contain sequence of ids from 0 - 9. So in theory, 5 should
    // be a valid identifier. Confirm this idea.
    ASSERT_EQ(true, MedicineManager::getInstance().contains(5));

    // Conversely, 99 should not be a valid identifier. Confirm this through the .contains() method.
    ASSERT_EQ(false, MedicineManager::getInstance().contains(99));

    // Remove the Medicine object with an id of 2.
    MedicineManager::getInstance().removeById(2);

    // The previous removal should reduce the size from 10 to 9. Check this through an assertion.
    ASSERT_EQ(9, MedicineManager::getInstance().getData().size());

    // Obtain a pointer to Medicine product of id 3 with the .getById() method.
    Medicine* lookup = &MedicineManager::getInstance().getById(3);

    // Assert that it was found and the id returned correctly.
    ASSERT_EQ(3, lookup->_id);

    // Reassign the pointer to an id we know is invalid (i.e.: 55).
    lookup = &MedicineManager::getInstance().getById(55);

    // If .getById() fails it return a automatic storage set object with zero-initialized values. Check if the
    // price is 0.0 to confirm this for sure.
    ASSERT_EQ(0.0, lookup->_price);

    // Clear the entire collection.
    MedicineManager::getInstance().clear();

    // Check if the collection size is now zero. If it is, .clear() worked correctly.
    ASSERT_EQ(0, MedicineManager::getInstance().getData().size());

}

// This test method confirms the uniqueness of a Medicine object name by filling the collection
// with several Medicine objects and invoking the .isUniqueName() method.
TEST(MedicineManagerModelTest, TestUniqueName)
{

    Medicine product1{0, "Product 1", "Company 1", "", "", 0.0, 0};
    Medicine product2{product1};
    Medicine product3{1, "Unique Product", "Unique Company", "", "", 0.0, 0};
    MedicineManager::getInstance().add(product1);
    MedicineManager::getInstance().add(product2);
    ASSERT_EQ(true, MedicineManager::getInstance().isUniqueName("Unique Product"));
    ASSERT_EQ(false, MedicineManager::getInstance().isUniqueName("Product 1"));
    MedicineManager::getInstance().clear();

}

// This test method validates that the .heapSortById() correctly sorts the data collection.
TEST(MedicineManagerModelTest, TestHeapSort)
{

    Medicine product1{22, "Product 1", "Company 1", "", "", 0.0, 0};
    Medicine product2{11, "Product 1", "Company 1", "", "", 0.0, 0};
    Medicine product3{13, "Unique Product", "Unique Company", "", "", 0.0, 0};
    MedicineManager::getInstance().add(product1);
    MedicineManager::getInstance().add(product2);
    MedicineManager::getInstance().add(product3);

    // This collection is clearly not sorted as .add() appends each item to the collection.
    // If we were to take the ids, we could get a psuedo-list such as the following: [22, 11, 13].
    // We would like our sorting algorithm to transform this into [11, 13, 22].
    MedicineManager::getInstance().heapSortById();

    // Given two sequential indexes i and j and a function f where f(x) returns the id of a particular index,
    // confirm that f(i) <= f(j).
    for(int i = 1; i < MedicineManager::getInstance().getData().size(); i++)
    {

        ASSERT_EQ(true, MedicineManager::getInstance().getData()[i]._id >= MedicineManager::getInstance().getData()[i - 1]._id);

    }
}

int main(int argc, char** argv)
{

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}