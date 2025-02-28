#include <gtest/gtest.h>
#include "../lib/bitarray.hpp"

TEST(BitArray_test, default_constructor)
{
    BitArray arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_TRUE(arr.empty());
    EXPECT_THROW(arr[0], std::invalid_argument);
    EXPECT_THROW(arr.set(0), std::invalid_argument);
}

TEST(BitArray_test, destructor)
{
    {
        BitArray arr;
        BitArray arr1(32, ~0UL);
        BitArray arr2(1024);
        BitArray arr4(32, 0b101010);
        arr4.clear();
    }

    SUCCEED();
}

TEST(BitArray_test, parameterized_constructor)
{
    BitArray arr(64, ~0UL);
    EXPECT_EQ(arr.size(), 64);
    EXPECT_TRUE(arr[0]);
    EXPECT_FALSE(arr[32]);

    BitArray arr1(0);
    EXPECT_EQ(arr1.size(), 0);
    EXPECT_TRUE(arr1.empty());
    EXPECT_THROW(arr1[0], std::invalid_argument);
    EXPECT_THROW(arr1.set(0), std::invalid_argument);

    EXPECT_THROW(BitArray arr2(-1), std::invalid_argument);
}

TEST(BitArray_test, copy_constructor)
{
    BitArray original(64, 0b101010);
    BitArray copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.to_string(), original.to_string());

    BitArray arr;
    BitArray arr1(arr);
    EXPECT_TRUE(arr1.empty());
}

TEST(BitArray_test, swap)
{
    BitArray arr1(64);
    BitArray arr2(32, ~0UL);
    arr1.swap(arr2);
    EXPECT_EQ(arr1.size(), 32);
    EXPECT_EQ(arr2.size(), 64);
    EXPECT_EQ(arr1.count(), 32);
    EXPECT_TRUE(arr2.none());

    BitArray arr;
    arr1.swap(arr);
    EXPECT_EQ(arr.size(), 32);
    EXPECT_EQ(arr.count(), 32);
    EXPECT_TRUE(arr1.empty());
    arr.swap(arr);
    EXPECT_EQ(arr.size(), 32);
    EXPECT_EQ(arr.count(), 32);
}

TEST(BitArray_test, assignment_operator)
{
    BitArray arr1(32, 0b101010);
    BitArray arr2;
    arr2 = arr1;
    EXPECT_EQ(arr2.size(), arr1.size());
    EXPECT_EQ(arr2.to_string(), arr1.to_string());
    BitArray arr3;
    arr1 = arr3;
    EXPECT_TRUE(arr1.empty());
    arr3 = arr2;
    arr2 = arr2;
    EXPECT_EQ(arr2.size(), arr3.size());
}

TEST(BitArray_test, resize)
{
    BitArray arr(32, 0b1111);
    EXPECT_THROW(arr.resize(-1), std::invalid_argument);
    EXPECT_THROW(arr[40], std::out_of_range);
    arr.resize(64, true);
    EXPECT_NO_THROW(arr[40]);
    EXPECT_EQ(arr.size(), 64);
    EXPECT_TRUE(arr[40]);
    arr.resize(16);
    EXPECT_THROW(arr[40], std::out_of_range);
    EXPECT_EQ(arr.size(), 16);
    EXPECT_TRUE(arr.none());
    arr.resize(64);
    EXPECT_NO_THROW(arr[40]);
    EXPECT_FALSE(arr[40]);
    EXPECT_TRUE(arr.none());
    arr.resize(40);
    EXPECT_THROW(arr[40], std::out_of_range);

    arr.resize(0);
    EXPECT_TRUE(arr.empty());
    arr.resize(1, true);
    EXPECT_NO_THROW(arr[0]);
}

TEST(BitArray_test, clear)
{
    BitArray arr(32, 0b1111);
    arr.clear();
    EXPECT_EQ(arr.size(), 0);
    EXPECT_TRUE(arr.empty());
    EXPECT_NO_THROW(arr.clear());

    BitArray arr1;
    EXPECT_NO_THROW(arr1.clear());
}

TEST(BitArray_test, push_back)
{
    BitArray arr(32, 0b1111);
    arr.push_back(true);
    EXPECT_EQ(arr.size(), 33);
    EXPECT_TRUE(arr[32]);

    arr.clear();
    arr.push_back(false);
    EXPECT_FALSE(arr[0]);
    arr.push_back(true);
    EXPECT_TRUE(arr[1]);
}

TEST(BitArray_test, bitwise_and)
{
    BitArray arr;
    BitArray arr1(32, 0b101010);
    BitArray arr2(32, 0b111100);
    EXPECT_THROW(arr &= arr1, std::invalid_argument);
    EXPECT_THROW(arr2 &= arr, std::invalid_argument);
    arr1 &= arr2;
    EXPECT_EQ(arr1.to_string(), BitArray(32, 0b101000).to_string());
    arr1.resize(16);
    EXPECT_THROW(arr1 &= arr2, std::runtime_error);
}

TEST(BitArray_test, bitwise_or)
{
    BitArray arr;
    BitArray arr1(32, 0b101010);
    BitArray arr2(32, 0b111100);
    EXPECT_THROW(arr |= arr1, std::invalid_argument);
    EXPECT_THROW(arr2 |= arr, std::invalid_argument);
    arr1 |= arr2;
    EXPECT_EQ(arr1.to_string(), BitArray(32, 0b111110).to_string());
    arr1.resize(16);
    EXPECT_THROW(arr1 |= arr2, std::runtime_error);
}

TEST(BitArray_test, bitwise_xor)
{
    BitArray arr;
    BitArray arr1(32, 0b101010);
    BitArray arr2(32, 0b111100);
    EXPECT_THROW(arr ^= arr1, std::invalid_argument);
    EXPECT_THROW(arr2 ^= arr, std::invalid_argument);
    arr1 ^= arr2;
    EXPECT_EQ(arr1.to_string(), BitArray(32, 0b010110).to_string());
    arr1.resize(16);
    EXPECT_THROW(arr1 ^= arr2, std::runtime_error);
}

TEST(BitArray_test, left_shift)
{
    BitArray arr(32, 0b101010);
    arr <<= 2;
    EXPECT_EQ(arr.to_string(), BitArray(32, 0b10101000).to_string());
    arr <<= 32;
    EXPECT_TRUE(arr.none());
    EXPECT_THROW(arr <<= -1, std::invalid_argument);
    arr.resize(48);
    arr.reset();
    arr.set(36);
    arr <<= 20;
    EXPECT_TRUE(arr[16]);

    BitArray arr1;
    EXPECT_THROW(arr1 <<= 2, std::invalid_argument);
}

TEST(BitArray_test, right_shift)
{
    BitArray arr(32, 0b101010);
    arr >>= 2;
    EXPECT_EQ(arr.to_string(), BitArray(32, 0b001010).to_string());
    arr >>= 32;
    EXPECT_TRUE(arr.none());
    EXPECT_THROW(arr >>= -1, std::invalid_argument);
    arr.resize(48);
    arr.reset();
    arr.set(16);
    arr >>= 20;
    EXPECT_TRUE(arr[36]);

    BitArray arr1;
    EXPECT_THROW(arr1 >>= 2, std::invalid_argument);
}

TEST(BitArray_test, left_shift_2)
{
    BitArray arr1(32, 0b101010);
    BitArray arr2 = arr1 << 2;
    EXPECT_EQ(arr2.to_string(), BitArray(32, 0b10101000).to_string());
    arr1 = arr2 << 32;
    EXPECT_TRUE(arr1.none());
    EXPECT_THROW(arr1 << -1, std::invalid_argument);
    arr1.resize(48);
    arr2.resize(48);
    arr1.reset();
    arr1.set(36);
    arr2 = arr1 << 20;
    EXPECT_TRUE(arr2[16]);

    BitArray arr;
    EXPECT_THROW(arr << 2, std::invalid_argument);
}

TEST(BitArray_test, right_shift_2)
{
    BitArray arr1(32, 0b101010);
    BitArray arr2 = arr1 >> 2;
    EXPECT_EQ(arr2.to_string(), BitArray(32, 0b001010).to_string());
    arr1 = arr2 >> 32;
    EXPECT_TRUE(arr1.none());
    EXPECT_THROW(arr1 >> -1, std::invalid_argument);
    arr1.resize(48);
    arr2.resize(48);
    arr1.reset();
    arr1.set(16);
    arr2 = arr1 >> 20;
    EXPECT_TRUE(arr2[36]);

    BitArray arr;
    EXPECT_THROW(arr >> 2, std::invalid_argument);
}

TEST(BitArray_test, set_bit)
{
    BitArray arr(32);
    arr.set(0);
    EXPECT_TRUE(arr[0]);
    arr.set(0, false);
    EXPECT_FALSE(arr[0]);
    arr.set(31);
    EXPECT_TRUE(arr[31]);
    EXPECT_THROW(arr.set(-1), std::out_of_range);
    EXPECT_THROW(arr.set(32), std::out_of_range);
    arr.push_back(false);
    arr.set(32);
    EXPECT_TRUE(arr[32]);

    BitArray arr1;
    EXPECT_THROW(arr1.set(0), std::invalid_argument);
}

TEST(BitArray_test, set_all)
{
    BitArray arr(32);
    arr.set();
    EXPECT_TRUE(arr.any());
    EXPECT_EQ(arr.count(), 32);
    arr.resize(48);
    arr.set();
    EXPECT_EQ(arr.count(), 48);

    BitArray arr1;
    EXPECT_THROW(arr1.set(), std::invalid_argument);
}

TEST(BitArray_test, reset_bit)
{
    BitArray arr(32, ~0UL);
    arr.reset(0);
    EXPECT_FALSE(arr[0]);
    EXPECT_THROW(arr.reset(-1), std::out_of_range);
    EXPECT_THROW(arr.reset(32), std::out_of_range);

    BitArray arr1;
    EXPECT_THROW(arr1.reset(0), std::invalid_argument);
}

TEST(BitArray_test, reset_all)
{
    BitArray arr(64, 0b1111);
    arr.reset();
    EXPECT_TRUE(arr.none());
    arr.resize(80, true);
    arr.reset();
    EXPECT_TRUE(arr.none());

    BitArray arr1;
    EXPECT_THROW(arr1.reset(), std::invalid_argument);
}

TEST(BitArray_test, any)
{
    BitArray arr(32, 0b1010);
    EXPECT_TRUE(arr.any());
    arr.clear();
    arr.push_back(true);
    EXPECT_TRUE(arr.any());
    arr.reset(0);
    arr.resize(32);
    EXPECT_FALSE(arr.any());
    arr.push_back(true);
    EXPECT_TRUE(arr.any());

    BitArray arr1;
    EXPECT_THROW(arr1.any(), std::invalid_argument);
}

TEST(BitArray_test, none)
{
    BitArray arr(32);
    EXPECT_TRUE(arr.none());
    arr.push_back(true);
    EXPECT_FALSE(arr.none());

    BitArray arr1;
    EXPECT_THROW(arr1.none(), std::invalid_argument);
}

TEST(BitArray_test, bitwise_not)
{
    BitArray arr1(32);
    arr1 = ~arr1;
    EXPECT_EQ(arr1.to_string(), BitArray(32, ~0UL).to_string());
    BitArray arr2 = ~arr1;
    arr1.reset();
    EXPECT_EQ(arr1.to_string(), arr2.to_string());

    BitArray arr;
    EXPECT_THROW(~arr, std::invalid_argument);
}

TEST(BitArray_test, count)
{
    BitArray arr(32, 0b1111);
    EXPECT_EQ(arr.count(), 4);
    arr.reset();
    EXPECT_EQ(arr.count(), 0);
    arr.resize(1024);
    arr.set();
    EXPECT_EQ(arr.count(), 1024);

    BitArray arr1;
    EXPECT_THROW(arr1.count(), std::invalid_argument);
}

TEST(BitArray_test, access_operator)
{
    BitArray arr(32, 0b1010);
    EXPECT_TRUE(arr[28]);
    EXPECT_FALSE(arr[29]);
    arr.clear();
    arr.push_back(true);
    EXPECT_TRUE(arr[0]);
    arr.resize(32, true);
    EXPECT_TRUE(arr[31]);
    EXPECT_THROW(arr[-1], std::out_of_range);
    EXPECT_THROW(arr[32], std::out_of_range);
    arr.push_back(false);
    EXPECT_FALSE(arr[32]);

    BitArray arr1;
    EXPECT_THROW(arr1[0], std::invalid_argument);
}

TEST(BitArray_test, size)
{
    BitArray arr(32);
    EXPECT_EQ(arr.size(), 32);
    arr.resize(150);
    EXPECT_EQ(arr.size(), 150);
    arr.clear();
    EXPECT_EQ(arr.size(), 0);

    BitArray arr1;
    EXPECT_EQ(arr1.size(), 0);
    BitArray arr2(0);
    EXPECT_EQ(arr2.size(), 0);
}

TEST(BitArray_test, empty)
{
    BitArray arr(32);
    EXPECT_FALSE(arr.empty());
    arr.clear();
    EXPECT_TRUE(arr.empty());

    BitArray arr1;
    EXPECT_TRUE(arr1.empty());
    BitArray arr2(0);
    EXPECT_TRUE(arr2.empty());
}

TEST(BitArray_test, to_string)
{
    BitArray arr(8, ~0UL);
    EXPECT_EQ(arr.to_string(), "11111111");
    arr.reset(0);
    EXPECT_EQ(arr.to_string(), "01111111");
    arr = ~arr;
    EXPECT_EQ(arr.to_string(), "10000000");
    arr.resize(1);
    EXPECT_EQ(arr.to_string(), "1");
    arr.reset();
    EXPECT_EQ(arr.to_string(), "0");

    BitArray arr1;
    EXPECT_THROW(arr1.to_string(), std::invalid_argument);
}

TEST(BitArray_test, equality_operator)
{
    BitArray arr1(32, 0b1010);
    BitArray arr2(32, 0b1010);
    EXPECT_TRUE(arr1 == arr2);
    arr1.set(0, true);
    EXPECT_FALSE(arr1 == arr2);
    arr2.resize(64, true);
    EXPECT_THROW(arr1 == arr2, std::runtime_error);
    arr1.clear();
    EXPECT_THROW(arr1 == arr2, std::invalid_argument);
    EXPECT_THROW(arr2 == arr1, std::invalid_argument);
    arr2.clear();
    EXPECT_TRUE(arr1 == arr2);
}

TEST(BitArray_test, inequality_operator)
{
    BitArray arr1(32, 0b1010);
    BitArray arr2(32, 0b1010);
    EXPECT_FALSE(arr1 != arr2);
    arr1.set(0, true);
    EXPECT_TRUE(arr1 != arr2);
    arr2.resize(64, true);
    EXPECT_THROW(arr1 != arr2, std::runtime_error);
    arr1.clear();
    EXPECT_THROW(arr1 != arr2, std::invalid_argument);
    EXPECT_THROW(arr2 != arr1, std::invalid_argument);
    arr2.clear();
    EXPECT_FALSE(arr1 != arr2);
}

TEST(BitArray_test, bitwise_and_2)
{
    BitArray arr;
    BitArray arr1(32, 0b101010);
    BitArray arr2(32, 0b111100);
    EXPECT_THROW(arr & arr1, std::invalid_argument);
    EXPECT_THROW(arr2 & arr, std::invalid_argument);
    arr = arr1 & arr2;
    EXPECT_EQ(arr.to_string(), BitArray(32, 0b101000).to_string());
    arr1.resize(16);
    EXPECT_THROW(arr1 & arr2, std::runtime_error);
}

TEST(BitArray_test, bitwise_or_2)
{
    BitArray arr;
    BitArray arr1(32, 0b101010);
    BitArray arr2(32, 0b111100);
    EXPECT_THROW(arr | arr1, std::invalid_argument);
    EXPECT_THROW(arr2 | arr, std::invalid_argument);
    arr = arr1 | arr2;
    EXPECT_EQ(arr.to_string(), BitArray(32, 0b111110).to_string());
    arr1.resize(16);
    EXPECT_THROW(arr1 | arr2, std::runtime_error);
}

TEST(BitArray_test, bitwise_xor_2)
{
    BitArray arr;
    BitArray arr1(32, 0b101010);
    BitArray arr2(32, 0b111100);
    EXPECT_THROW(arr ^ arr1, std::invalid_argument);
    EXPECT_THROW(arr2 ^ arr, std::invalid_argument);
    arr = arr1 ^ arr2;
    EXPECT_EQ(arr.to_string(), BitArray(32, 0b010110).to_string());
    arr1.resize(16);
    EXPECT_THROW(arr1 ^ arr2, std::runtime_error);
}