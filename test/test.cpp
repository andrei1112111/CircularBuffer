#include <gtest/gtest.h>
#include "CircularBuffer.h"


TEST(Construct, without_parameters) {
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>());
}

TEST(Construct, with_capacity) {
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(-1));
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(2));
}

TEST(Construct, with_capacity_and_value) {
    int s;
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(12, s));
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(-122, s));
}

TEST(Construct, from_another_CircularBuffer) {
    int s;
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(CircularBuffer<int>()));
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(CircularBuffer<int>(-122, s)));
}

TEST(Construct, deconstruct) {
    auto *cb = new CircularBuffer<int>();
    ASSERT_NO_FATAL_FAILURE(cb->~CircularBuffer<int>());
    delete cb;

    auto *cbb = new CircularBuffer<int>(12);
    ASSERT_NO_FATAL_FAILURE(cbb->~CircularBuffer<int>());
    delete cbb;
}


TEST(Getters, with_empty_buffer) {
    auto ecb = CircularBuffer<int>();

    ASSERT_EQ(ecb.capacity(), 0);
    ASSERT_EQ(ecb.full(), true);
    ASSERT_EQ(ecb.size(), 0);
    ASSERT_EQ(ecb.empty(), true);
    ASSERT_EQ(ecb.reserve(), 0);
}

TEST(Getters, with_not_empty_buffer) {
    int s;
    auto cb = CircularBuffer<int>(12, s);

    ASSERT_EQ(cb.capacity(), 12);
    ASSERT_EQ(cb.full(), true);
    ASSERT_EQ(cb.size(), 12);
    ASSERT_EQ(cb.empty(), false);
    ASSERT_EQ(cb.reserve(), 0);
}

TEST(Getters, front_and_back) {
    int s;
    // front, back
    auto a = CircularBuffer<int>(12, s);
    auto b = CircularBuffer<int>();

    ASSERT_EQ(a.front(), s);

    a.push_front(1);
    ASSERT_EQ(a.front(), 1);
    ASSERT_EQ(a.back(), s);

    ASSERT_THROW(b.front(), std::out_of_range);
}

TEST(Getters, clear_and_check_empty) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);

    cb.clear();
    ASSERT_EQ(cb.size(), 0);
    ASSERT_TRUE(cb.empty());
}

TEST(Indexing, indexing) {
    auto cb = CircularBuffer<int>(4);

    for (int i = 1; i < 10; ++i) {
        cb.push_back(i);
    }
    for (int i = 1; i < 5; ++i) {
        cb.pop_back();
    }

    ASSERT_THROW(cb.pop_back(), std::out_of_range);
}

TEST(Methods, clear) {
    int s;

    auto cb = CircularBuffer<int>(12, s);
    ASSERT_NO_FATAL_FAILURE(cb.clear());

    auto ecb = CircularBuffer<int>();
    ASSERT_NO_FATAL_FAILURE(cb.clear());
}

TEST(Methods, swap) {
    int s;

    auto cb2 = CircularBuffer<int>(12, s);
    auto cb2_copy = cb2;
    auto ecb2 = CircularBuffer<int>(12);
    ASSERT_NO_FATAL_FAILURE(ecb2.swap(cb2));

    ASSERT_EQ(ecb2, cb2_copy);
}

TEST(Methods, push) {
    int s;

    auto cbb = CircularBuffer<int>(3);
    cbb.push_back({});
    cbb.push_back(s);
    cbb.push_back(s);
    cbb.push_back(s);
    ASSERT_EQ(cbb.front(), cbb.back());
    cbb.push_back({});
    ASSERT_NE(cbb.front(), cbb.back());

    cbb.push_front({});
    cbb.push_front({});
    ASSERT_EQ(cbb.front(), cbb.back());
    cbb.push_back(s);
    ASSERT_NE(cbb.front(), cbb.back());
}
TEST(Methods, resize) {
    int s;

    auto cb2 = CircularBuffer<int>(12, s);

    cb2.resize(2);
    ASSERT_EQ(cb2.back(), s);

    cb2.resize(10, {});
    ASSERT_EQ(cb2.back(), {});
}

TEST(Methods, clear_and_check_empty) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);

    cb.clear();
    ASSERT_EQ(cb.size(), 0);
    ASSERT_TRUE(cb.empty());
}

TEST(Operators, eq) {
    int s;

    // not empty
    ASSERT_EQ(CircularBuffer<int>(12, s), CircularBuffer<int>(12, s));

    // empty
    ASSERT_EQ(CircularBuffer<int>(), CircularBuffer<int>());
}

TEST(Operators, neq) {
    int s;

    // not empty
    ASSERT_NE(CircularBuffer<int>(3), CircularBuffer<int>(3, {}));
    ASSERT_NE(CircularBuffer<int>(10), CircularBuffer<int>(12));

    // empty
    ASSERT_NE(CircularBuffer<int>(), CircularBuffer<int>(1));
}

TEST(Assignment, assignment) {
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>() = CircularBuffer<int>());
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(12, {}) = CircularBuffer<int>());
    ASSERT_NO_FATAL_FAILURE(CircularBuffer<int>(12, {}) = CircularBuffer<int>());

    auto a = CircularBuffer<int>();
    a = CircularBuffer<int>(12, {});

    ASSERT_EQ(
            CircularBuffer<int>(12, {}),
            a
    );

    a = CircularBuffer<int>();
    ASSERT_EQ(
            a,
            CircularBuffer<int>()
    );

    ASSERT_NE(
            CircularBuffer<int>(12, {}),
            a
    );
}






TEST(Methods, linearization) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);

    cb.linearize();
    ASSERT_EQ(cb.front(), 1);

    cb.push_front(0);
    ASSERT_EQ(cb.front(), 0);
}

TEST(Methods, rotate) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);

    cb.rotate(2);
    ASSERT_EQ(cb.front(), 3);
}

TEST(Methods, insert) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    cb.insert(1, 99);
    ASSERT_EQ(cb[1], 99);
    ASSERT_EQ(cb.size(), 4);
}

TEST(Methods, erase) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);
    cb.push_back(4);

    cb.erase(1, 3);
    ASSERT_EQ(cb.size(), 2);
    ASSERT_EQ(cb[0], 1);
    ASSERT_EQ(cb[1], 4);
}

TEST(Methods, set_capacity) {
    CircularBuffer<int> cb(5);

    // Fill the buffer
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    // Test increasing capacity
    cb.set_capacity(10);
    ASSERT_EQ(cb.capacity(), 10);
    ASSERT_EQ(cb.size(), 3);
    ASSERT_EQ(cb[0], 1);
    ASSERT_EQ(cb[1], 2);
    ASSERT_EQ(cb[2], 3);

    // Test decreasing capacity
    cb.set_capacity(2);
    ASSERT_EQ(cb.capacity(), 2);
    ASSERT_EQ(cb.size(), 2);
    ASSERT_EQ(cb[0], 1);
    ASSERT_EQ(cb[1], 2);

    // Test setting the same capacity
    cb.set_capacity(2);
    ASSERT_EQ(cb.size(), 2);

    // Test setting to zero capacity
    cb.set_capacity(0);
    ASSERT_EQ(cb.capacity(), 0);
    ASSERT_EQ(cb.size(), 0);

    // Test negative capacity
    ASSERT_THROW(cb.set_capacity(-1), std::out_of_range);
}

TEST(Exceptions, rotate_out_of_bounds) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    ASSERT_THROW(cb.rotate(5), std::out_of_range);
}

TEST(Exceptions, insert_out_of_bounds) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);

    ASSERT_THROW(cb.insert(3, 99), std::out_of_range);
}

TEST(Exceptions, erase_out_of_bounds) {
    CircularBuffer<int> cb(5);
    cb.push_back(1);
    cb.push_back(2);

    ASSERT_THROW(cb.erase(1, 3), std::out_of_range);
}
