#pragma once
#include <iterator>
#include <type_traits>
#include <boost/assert.hpp>

namespace math {

template<typename VALUE_TYPE>
class matrix_iterator
{
public:
    using self_type = matrix_iterator<VALUE_TYPE>;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename VALUE_TYPE;
    using pointer =  typename std::add_pointer<typename VALUE_TYPE>::type;
    using reference = typename std::add_lvalue_reference<typename VALUE_TYPE>::type;
    using difference_type = std::ptrdiff_t;
private:
    pointer mCurrentElement = nullptr;
    size_t mIncrement = 1;
public:
    matrix_iterator() = default;
    matrix_iterator(pointer currentElement, size_t increment = 1)
        : mCurrentElement(currentElement)
        , mIncrement(increment)
    {
    }

    matrix_iterator(const self_type &) = default;
    self_type& operator=(const self_type &) = default;

    bool operator==(const self_type &otherIt)
    {
        return mCurrentElement == otherIt.mCurrentElement && mIncrement == otherIt.mIncrement;
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
