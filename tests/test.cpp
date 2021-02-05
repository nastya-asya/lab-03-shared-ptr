// Copyright 2020 Anastasiya Smirnova nastya.asya08@yandex.ru

#include <gtest/gtest.h>
#include "shared_ptr.hpp"

TEST(Example, EmptyTest) { EXPECT_TRUE(true); }

TEST(SharedPtr, func1) {
  int k = 1000;
  SharedPtr<int> test0(&k);
  ASSERT_EQ(*test0, k);
}

TEST(SharedPtr, funcOperator) {
  int k = 1000;
  SharedPtr<int> test0(&k);
  SharedPtr<int> test1 = test0;
  ASSERT_EQ(*test1, k);
}

TEST(SharedPtr, funcCountOfLinks) {
  int k = 1000;
  SharedPtr<int> test0(&k);
  SharedPtr<int> test1(test0);
  SharedPtr<int> test2 = test1;
  ASSERT_EQ(test0.useCount(), 3);
}

TEST(SharedPtr, funcSwap) {
  int a = 1;
  int b = 2;
  SharedPtr<int> A(&a);
  SharedPtr<int> B(&b);
  B.swap(A);
  ASSERT_EQ(*B, 1);
}

TEST(SharedPtr, funcRvalueOperator) {
  int a = 1;
  SharedPtr<int> A(&a);
  SharedPtr<int> B(std::move(A));
  ASSERT_EQ(B.useCount(), 2);
}

TEST(SharedPtr, funcBool) {
  SharedPtr<int> A;
  ASSERT_EQ(A, false);
}

TEST(SharedPtr, funcThrow) {
  SharedPtr<int> A;
  ASSERT_THROW(*A, std::out_of_range);
}

