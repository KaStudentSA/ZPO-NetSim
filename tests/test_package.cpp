//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "package.hpp"
#include "types.hpp"



TEST(PackageTest, IsAssignedIdLowest) {
    // przydzielanie ID o jeden większych -- utworzenie dwóch obiektów pod rząd
    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused) {
    // przydzielanie ID po zwolnionym obiekcie

    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, ReuseMiddleId) {
    // Reset stanu (jeśli pola są publiczne)
    // Package::assigned_IDs.clear(); Package::freed_IDs.clear();

    Package* p1 = new Package(); // ID: 1
    Package* p2 = new Package(); // ID: 2
    Package* p3 = new Package(); // ID: 3

    delete p2; // ID 2 trafia do freed_IDs

    Package p4; // Powinna przejąć ID 2
    EXPECT_EQ(p4.get_id(), 2);

    delete p1;
    delete p3;
}