#pragma once
#include <vector>

namespace ann {

class ShuffledRange
{
public:
    using IndexedRange = std::vector<size_t>;
private:
    IndexedRange mRange;
    IndexedRange::const_iterator mLastElement;
public:
    ShuffledRange(size_t rangeEnd);
    void shuffle();
    IndexedRange getNextN(size_t nextRangeSize);
    bool hasMoreElements() const;
};

}