#pragma once
#include <iterator>
#include <boost/assert.hpp>

namespace math {

template<class M>
class matrix_iterator
{
public:
    using self_type = matrix_iterator<M>;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename M::element_type;
    using pointer = typename M::element_type *;
    using reference = typename M::element_type &;
    using difference_type = std::ptrdiff_t;
private:
    M *mPointsTo = nullptr;
    pointer mCurrentElement = nullptr;
    size_t mIncrement = 1;
public:
    matrix_iterator() = default;
    matrix_iterator(M *pointsTo, pointer currentElement, size_t increment = 1)
        : mPointsTo(pointsTo)
        , mCurrentElement(currentElement)
        , mIncrement(increment)
    {
    }

    matrix_iterator(const self_type &) = default;
    self_type& operator=(const self_type &) = default;

    bool operator==(const self_type &otherIt)
    {
        BOOST_ASSERT(mPointsTo == otherIt.mPointsTo);
        return mPointsTo == otherIt.mPointsTo &&
            mCurrentElement == otherIt.mCurrentElement;
    }

    bool operator!=(const self_type &otherIt)
    {
        return !(*this == otherIt);
    }

    reference operator*()
    {
        return *mCurrentElement;
    }

    pointer operator->()
    {
        return mCurrentElement;
    }

    // postfix
    self_type operator++(int)
    {
        auto returnValue = *this;
        mCurrentElement += mIncrement;
        return returnValue;
    }

    // prefix
    self_type operator++()
    {
        mCurrentElement += mIncrement;
        return *this;
    }
    // postfix
    self_type operator--(int)
    {
        auto returnValue = *this;
        mCurrentElement -= mIncrement;
        return returnValue;
    }

    // prefix
    self_type operator--()
    {
        mCurrentElement -= mIncrement;
        return *this;
    }
};

}
