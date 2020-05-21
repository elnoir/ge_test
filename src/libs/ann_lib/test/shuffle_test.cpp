#include <gtest/gtest.h>
#include <boost/range/algorithm/for_each.hpp>
#include "../src/shuffled_range.h"
#include <set>


TEST(ShuffleTest, Elements)
{
    ann::ShuffledRange range(123);

    std::set<size_t> elementCheck;

    for (int i=0; range.hasMoreElements(); ++i)
    {
        auto newRange = range.getNextN(10);
        ASSERT_TRUE(newRange.size() <= 10);
        boost::for_each(newRange, [&elementCheck](size_t value) {
            ASSERT_TRUE(elementCheck.count(value) == 0);
            elementCheck.insert(value);
        });
    }

    for (int i=0; i<123; i++)
    {
        ASSERT_TRUE(elementCheck.count(i) == 1);
    }
    ASSERT_FALSE(elementCheck.count(123) == 1);
}
