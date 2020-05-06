#pragma once

#include <initializer_list>
#include <vector>
#include <exception>
#include <functional>

#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/assert.hpp>

#include <matrix/matrix_iterators.h>

namespace math {

template<typename T = float>
class Matrix
{
private:
    using storage = std::vector<T>;

public:
    using element_type = T;
    using self_type = Matrix<T>;
    using row_init_list = std::initializer_list<element_type>;
    using matrix_init_list = std::initializer_list<row_init_list>;
    using function_type = std::function<element_type(element_type)>;
    using iterator = math::matrix_iterator<Matrix<T>>;

private:
    storage mData;
    size_t mRowCount = 0;
    size_t mColumnCount = 0;

    self_type& appendRow(const row_init_list &rowList)
    {
        if (mColumnCount == 0)
        {
            mColumnCount = rowList.size();
        }
        else if (mColumnCount != rowList.size())
        {
            throw std::logic_error("Malformed initialization list");
        }
        boost::push_back(mData, rowList);
        return *this;
    }

    template<class BF>
    static self_type internal_binary_func_apply(const self_type &lhs, const self_type &rhs, BF &fx)
    {
        BOOST_ASSERT(lhs.mRowCount == rhs.mRowCount && lhs.mColumnCount == rhs.mColumnCount);
        self_type result = make_matrix<T>(lhs.mRowCount, lhs.mColumnCount);
        boost::transform(lhs.mData, rhs.mData, result.mData.begin(), fx);
        return result;
    }

    template<class UF>
    static Matrix<T> internal_unary_func_apply(const Matrix<T> &lhs, UF &fx)
    {
        self_type result = make_matrix<T>(lhs.mRowCount, lhs.mColumnCount);
        boost::transform(lhs.mData, result.mData.begin(), fx);
        return result;
    }

public:
    Matrix() = default;
    Matrix(const self_type &other) = default;
    Matrix(self_type &&other) = default;

    Matrix(matrix_init_list &&matrixList)
    {
        mData.clear();
        mRowCount = matrixList.size();
        for (auto&& rowList : matrixList)
        {
            appendRow(rowList);
        }
    }

    self_type& operator=(const self_type &other) = default;
    self_type& operator=(self_type &&other) = default;

    self_type& Resize(size_t nRow, size_t nColumn)
    {
        mData.resize(nRow * nColumn);
        mColumnCount = nColumn;
        mRowCount = nRow;
        mData.shrink_to_fit();
        return *this;
    }

    T& Get(size_t nRow, size_t nColumn)
    {
        BOOST_ASSERT(nRow < mRowCount && nColumn < mColumnCount);
        return mData[nRow * mRowCount + nColumn];
    }

    const T& Get(size_t nRow, size_t nColumn) const
    {
        BOOST_ASSERT(nRow < mRowCount && nColumn < mColumnCount);
        return mData[nRow * mRowCount + nColumn];
    }

    self_type& Set(const size_t nRow, const size_t nColumn, T value)
    {
        Get(nRow, nColumn) = value;
        return *this;
    }

    friend bool operator==(const self_type &lhs, const self_type &rhs)
    {
        return lhs.mRowCount == rhs.mRowCount && lhs.mColumnCount == rhs.mColumnCount && boost::equal(lhs.mData, rhs.mData);
    }

    friend bool operator!=(const self_type &lhs, const self_type &rhs)
    {
        return !(lhs == rhs);
    }

    friend self_type operator+(const self_type &lhs, const self_type &rhs)
    {
        return internal_binary_func_apply(lhs, rhs, std::plus<T>());
    }

    friend self_type operator-(const self_type &lhs, const self_type &rhs)
    {
        return internal_binary_func_apply(lhs, rhs, std::minus<T>());
    }

    friend self_type operator+(const self_type &lhs, T value)
    {
        return internal_unary_func_apply(lhs, [&value](T val){
            return value + val;
        });
    }

    friend self_type operator-(const self_type &lhs, T value)
    {
        return internal_unary_func_apply(lhs, [&value](T val) {
            return value - val;
        });
    }

    friend self_type operator*(const self_type &lhs, T value)
    {
        return internal_unary_func_apply(lhs, [&value](T val) {
            return value * val;
        });
    }

    friend self_type operator/(const self_type &lhs, T external_value)
    {
        return internal_unary_func_apply(lhs, [&external_value](T val) {
            return val / external_value;
        });
    }

    self_type Apply(std::function<T(T)> &fx)
    {
        return internal_unary_func_apply(*this, fx);
    }

    iterator RowBegin(size_t nRow)
    {
        auto startElement = mData.data() + nRow * mColumnCount;
        return iterator(this, startElement, 1);
    }

    iterator RowEnd(size_t nRow)
    {
        auto startElement = mData.data() + nRow * mColumnCount + mColumnCount;
        return iterator(this, startElement, 1);
    }

    iterator ColumnBegin(size_t nColumn)
    {
        auto startElement = mData.data() + nColumn;
        return iterator(this, startElement, mColumnCount);
    }

    iterator ColumnEnd(size_t nColumn)
    {
        auto startElement = mData.data() + mRowCount * mColumnCount + nColumn;
        return iterator(this, startElement, mColumnCount);
    }

    iterator Begin()
    {
        return iterator(this, mData.data(), 1);
    }

    iterator End()
    {
        return iterator(this, mData.data() + mRowCount * mColumnCount, 1);
    }

};

template<typename T>
math::Matrix<T> make_matrix(const size_t rowCount, const size_t columnCount)
{
    Matrix<T> result;
    result.Resize(rowCount, columnCount);
    return result;
}

}