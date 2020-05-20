#include "shuffled_range.h"
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>


namespace ann
{
    ShuffledRange::ShuffledRange(size_t rangeEnd)
    {
        mRange.resize(rangeEnd);
        boost::iota(mRange, 0);
        shuffle();
    }

    void ShuffledRange::shuffle()
    {
        boost::random_shuffle(mRange);
        mLastElement = mRange.begin();
    }

    ShuffledRange::IndexedRange ShuffledRange::getNextN(size_t nextRangeSize, bool autoRestart /*= false*/)
    {
        ShuffledRange::IndexedRange result(nextRangeSize);

        auto it = result.begin();
        while (it != result.end())
        {
            if (mLastElement == mRange.cend())
            {
                if (autoRestart)
                {
                    mLastElement = mRange.cbegin();
                }
                else
                {
                    result.erase(it, result.end());
                    break;
                }
            }
            *it = *mLastElement;
            ++it;
            ++mLastElement;
        }
        return result;
    }
}