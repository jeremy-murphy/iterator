// Copyright (C) 2017 Michel Morin.
// Copyright (C) 2026 Amlal El Mahrouss.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <vector>
#include <list>
#include <boost/container/slist.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/iterator/distance.hpp>
#include <boost/iterator/transform_iterator.hpp>

#define BOOST_TEST_DISTANCE (-1)
#define BOOST_TEST_DIFF_DISTANCE (-2)

int twice(int x) { return x + x; }

template <typename Iterator>
void test_distance(Iterator it_from, Iterator it_to, int n)
{
    BOOST_TEST(boost::distance(it_from, it_to) == n);
}

// Definitely not an iterator.
struct Foo
{
    friend BOOST_CXX14_CONSTEXPR
    std::ptrdiff_t distance(Foo const &, Foo const &) { return BOOST_TEST_DISTANCE; }
};

namespace boost 
{
    static BOOST_CXX14_CONSTEXPR std::ptrdiff_t distance(Foo const &, Foo const &)  { return BOOST_TEST_DIFF_DISTANCE; }
} // namespace boost

int main()
{
    int array[3] = {1, 2, 3};
    int* ptr1 = array;
    int* ptr2 = array + 3;

    {
        test_distance(ptr1, ptr2,  3);
        test_distance(ptr2, ptr1, -3);

        test_distance(
            boost::make_transform_iterator(ptr1, twice)
          , boost::make_transform_iterator(ptr2, twice)
          , 3
        );
        test_distance(
            boost::make_transform_iterator(ptr2, twice)
          , boost::make_transform_iterator(ptr1, twice)
          , -3
        );
    }

    {
        std::vector<int> ints(ptr1, ptr2);
        test_distance(ints.begin(), ints.end(),  3);
        test_distance(ints.end(), ints.begin(), -3);

        test_distance(
            boost::make_transform_iterator(ints.begin(), twice)
          , boost::make_transform_iterator(ints.end(), twice)
          , 3
        );
        test_distance(
            boost::make_transform_iterator(ints.end(), twice)
          , boost::make_transform_iterator(ints.begin(), twice)
          , -3
        );
    }

    {
        std::list<int> ints(ptr1, ptr2);
        test_distance(ints.begin(), ints.end(),  3);

        test_distance(
            boost::make_transform_iterator(ints.begin(), twice)
          , boost::make_transform_iterator(ints.end(), twice)
          , 3
        );
    }

    {
        boost::container::slist<int> ints(ptr1, ptr2);
        test_distance(ints.begin(), ints.end(),  3);

        test_distance(
            boost::make_transform_iterator(ints.begin(), twice)
          , boost::make_transform_iterator(ints.end(), twice)
          , 3
        );
    }

    {
        auto expected = ::boost::distance(Foo{}, Foo{});
        auto right_result = distance(Foo{}, Foo{});

        BOOST_TEST(expected != right_result);
    }
    return boost::report_errors();
}