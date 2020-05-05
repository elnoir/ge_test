#pragma once

#include <initializer_list>
#include <vector>
#include <exception>
#include <functional>

#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/assert.hpp>

namespace math {

template<typename T = float>
class Matrix
{
private:
    using storage = std::vector<T>;

public:
    using element_type = T;
    using row_init_list = std::initializer_list<element_type>;
    using matrix_init_list = std::initializer_list<row_init_list>;
    using function_type = std::function<element_type(element_type)>;

private:
    storage mData;
    size_t mRowCount = 0;
    size_t mColumnCount = 0;

    Matrix<T>& appendRow(const row_init_list &rowList)
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
    static Matrix<T> internal_binary_func_apply(const Matrix<T> &lhs, const Matrix<T> &rhs, BF &fx)
    {
        BOOST_ASSERT(lhs.mRowCount == rhs.mRowCount && lhs.mColumnCount == rhs.mColumnCount);
        Matrix result = make_matrix<T>(lhs.mRowCount, lhs.mColumnCount);
        boost::transform(lhs.mData, rhs.mData, result.mData.begin(), fx);
        return result;
    }

    template<class UF>
    static Matrix<T> internal_unary_func_apply(const Matrix<T> &lhs, UF &fx)
    {
        Matrix result = make_matrix<T>(lhs.mRowCount, lhs.mColumnCount);
        boost::transform(lhs.mData, result.mData.begin(), fx);
        return result;
    }

public:
    Matrix() = default;
    Matrix(const Matrix<T> &other) = default;
    Matrix(Matrix<T> &&other) = default;

    Matrix(matrix_init_list &&matrixList)
    {
        mData.clear();
        mRowCount = matrixList.size();
        for (auto&& rowList : matrixList)
        {
            appendRow(rowList);
        }
    }

    Matrix<T>& operator=(const Matrix<T> &other) = default;
    Matrix<T>& operator=(Matrix<T> &&other) = default;

    Matrix<T>& Resize(size_t nRow, size_t nColumn)
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

    Matrix<T>& Set(const size_t nRow, const size_t nColumn, T value)
    {
        Get(nRow, nColumn) = value;
        return *this;
    }

    friend bool operator==(const Matrix &lhs, const Matrix &rhs)
    {
        return lhs.mRowCount == rhs.mRowCount && lhs.mColumnCount == rhs.mColumnCount && boost::equal(lhs.mData, rhs.mData);
    }

    friend bool operator!=(const Matrix &lhs, const Matrix &rhs)
    {
        return !(lhs == rhs);
    }

    friend Matrix operator+(const Matrix &lhs, const Matrix &rhs)
    {
        return internal_binary_func_apply(lhs, rhs, std::plus<T>());
    }

    friend Matrix operator-(const Matrix &lhs, const Matrix &rhs)
    {
        return internal_binary_func_apply(lhs, rhs, std::minus<T>());
    }

    friend Matrix operator+(const Matrix &lhs, T value)
    {
        return internal_unary_func_apply(lhs, [&value](T val){
            return value + val;
        });
    }

    friend Matrix operator-(const Matrix &lhs, T value)
    {
        return internal_unary_func_apply(lhs, [&value](T val) {
            return value - val;
        });
    }

    friend Matrix operator*(const Matrix &lhs, T value)
    {
        return internal_unary_func_apply(lhs, [&value](T val) {
            return value * val;
        });
    }

    friend Matrix operator/(const Matrix &lhs, T value)
    {
        return internal_unary_func_apply(lhs, [&value](T val) {
            return value * val;
        });
    }

    Matrix Apply(std::function<T(T)> &fx)
    {
        return internal_unary_func_apply(*this, fx);
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