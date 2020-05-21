#pragma once

#include <initializer_list>
#include <vector>
#include <exception>
#include <functional>
#include <numeric>
#include <algorithm>

#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/algorithm/minmax_element.hpp>
#include <iostream>
#include <boost/assert.hpp>
#include <iomanip>

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
    using iterator = math::matrix_iterator<element_type>;
    using const_iterator = math::matrix_iterator<const element_type>;

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
    static self_type internalBinaryFuncApply(const self_type &lhs, const self_type &rhs, BF &fx)
    {
        BOOST_ASSERT(lhs.mRowCount == rhs.mRowCount && lhs.mColumnCount == rhs.mColumnCount);
        self_type result = make_matrix<T>(lhs.mRowCount, lhs.mColumnCount);
        boost::transform(lhs.mData, rhs.mData, result.mData.begin(), fx);
        return result;
    }

    template<class UF>
    static Matrix<T> internalUnaryFuncApply(const Matrix<T> &lhs, UF &fx)
    {
        self_type result = make_matrix<T>(lhs.mRowCount, lhs.mColumnCount);
        boost::transform(lhs.mData, result.mData.begin(), fx);
        return result;
    }

public:
    // init
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

    // size related
    self_type& resize(size_t nRow, size_t nColumn)
    {
        mData.resize(nRow * nColumn, static_cast<element_type>(0));
        mColumnCount = nColumn;
        mRowCount = nRow;
        mData.shrink_to_fit();
        return *this;
    }

    size_t getColumnCount() const
    {
        return mColumnCount;
    }

    size_t getRowCount() const
    {
        return mRowCount;
    }

    // value access
    T& get(size_t nRow, size_t nColumn)
    {
        BOOST_ASSERT(nRow < mRowCount && nColumn < mColumnCount);
        return mData[nRow * mColumnCount + nColumn];
    }

    const T& get(size_t nRow, size_t nColumn) const
    {
        BOOST_ASSERT(nRow < mRowCount && nColumn < mColumnCount);
        return mData[nRow * mColumnCount + nColumn];
    }

    self_type& set(const size_t nRow, const size_t nColumn, T value)
    {
        get(nRow, nColumn) = value;
        return *this;
    }

    // operators
    self_type apply(std::function<T(T)> fx) const
    {
        return internalUnaryFuncApply(*this, fx);
    }

    self_type transpose() const
    {
        auto result = make_matrix<element_type>(mColumnCount, mRowCount);
        for (size_t i = 0; i < mRowCount; ++i)
        {
            for (size_t j = 0; j < mColumnCount; ++j)
            {
                result.set(j, i, get(i, j));
            }
        }
        return result;
    }

    // iterators

    iterator rowBegin(size_t nRow)
    {
        return iterator(mData.data() + nRow * mColumnCount);
    }

    const_iterator rowBegin(size_t nRow) const
    {
        return const_iterator(mData.data() + nRow * mColumnCount);
    }

    iterator rowEnd(size_t nRow)
    {
        return iterator(mData.data() + nRow * mColumnCount + mColumnCount);
    }

    const_iterator rowEnd(size_t nRow) const
    {
        return const_iterator(mData.data() + nRow * mColumnCount + mColumnCount);
    }

    iterator columnBegin(size_t nColumn)
    {
        return iterator(mData.data() + nColumn, mColumnCount);
    }

    const_iterator columnBegin(size_t nColumn) const
    {
        return const_iterator(mData.data() + nColumn, mColumnCount);
    }

    iterator columnEnd(size_t nColumn)
    {
        return iterator(mData.data() + mRowCount * mColumnCount + nColumn, mColumnCount);
    }

    const_iterator columnEnd(size_t nColumn) const
    {
        return const_iterator(mData.data() + mRowCount * mColumnCount + nColumn, mColumnCount);
    }

    iterator begin()
    {
        return iterator(mData.data());
    }

    const_iterator begin() const
    {
        return const_iterator(mData.data());
    }

    iterator end()
    {
        return iterator(mData.data() + mRowCount * mColumnCount);
    }

    const_iterator end() const
    {
        return const_iterator(mData.data() + mRowCount * mColumnCount);
    }

    const_iterator cRowBegin(size_t nRow) const { return rowBegin(nRow); }
    const_iterator cRowEnd(size_t nRow) const { return rowEnd(nRow); }
    const_iterator cColumnBegin(size_t nColumn) const { return columnBegin(nColumn); }
    const_iterator cColumnEnd(size_t nColumn) const { return columnEnd(nColumn); }
    const_iterator cBegin() const { return begin(); }
    const_iterator cEnd() const { return end(); }

    self_type addColumnVector(const self_type &rhs) const
    {
        BOOST_ASSERT(rhs.mRowCount == mRowCount);
        BOOST_ASSERT(rhs.mColumnCount == 1);
        auto result = make_matrix<element_type>(mRowCount, mColumnCount);
        for (size_t i = 0; i < mRowCount; ++i)
        {
            for (auto j = 0; j < mColumnCount; ++j)
            {
                result.set(i, j, get(i,j) + rhs.get(i, 0));
            }
        }
        return result;
    }

    self_type hadamardProduct(const self_type &rhs) const
    {
        BOOST_ASSERT(mRowCount == rhs.mRowCount && mColumnCount == rhs.mColumnCount);
        auto result = math::make_matrix<element_type>(rhs.mRowCount, rhs.mColumnCount);
        std::transform(cBegin(), cEnd(), rhs.cBegin(), result.begin(), std::multiplies<float>());
        return result;
    }

    self_type divideWithRow(const self_type rhs) const
    {
        BOOST_ASSERT(rhs.getRowCount() == 1 && rhs.getColumnCount() == mColumnCount);
        auto result = math::make_matrix<element_type>(mRowCount, mColumnCount);
        for (size_t i=0; i < mRowCount; ++i)
        {
            std::transform(cRowBegin(i), cRowEnd(i), rhs.cRowBegin(0), result.rowBegin(i), [](float left, float right) {
                return left / right;
            });
        }
        return result;
    }


    // Non member functions

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
        return internalBinaryFuncApply(lhs, rhs, std::plus<T>());
    }

    friend self_type operator-(const self_type &lhs, const self_type &rhs)
    {
        return internalBinaryFuncApply(lhs, rhs, std::minus<T>());
    }

    friend self_type operator+(const self_type &lhs, T rhs_value)
    {
        return internalUnaryFuncApply(lhs, [&rhs_value](T val){
            return val + rhs_value;
        });
    }

    friend self_type operator-(const self_type &lhs, T rhs_value)
    {
        return internalUnaryFuncApply(lhs, [&rhs_value](T val) {
            return val - rhs_value;
        });
    }

    friend self_type operator-(T lhs_value, const self_type &rhs)
    {
        return internalUnaryFuncApply(rhs, [&lhs_value](T val) {
            return lhs_value - val;
        });
    }

    friend self_type operator*(const self_type &lhs, T rhs_value)
    {
        return internalUnaryFuncApply(lhs, [&rhs_value](T val) {
            return val * rhs_value;
        });
    }

    friend self_type operator*(const T &lhs,const self_type &rhs_value)
    {
        return internalUnaryFuncApply(rhs_value, [&lhs](T val) {
            return val * lhs;
        });
    }

    friend self_type operator*(const self_type &lhs, const self_type &rhs)
    {
        BOOST_ASSERT(lhs.mColumnCount == rhs.mRowCount);
        auto result = make_matrix<self_type::element_type>(lhs.mRowCount, rhs.mColumnCount);
        for (size_t i = 0; i < result.mRowCount; ++i)
        {
            for (size_t j = 0; j < result.mColumnCount; ++j)
            {
                result.set(
                    i, j,
                    std::inner_product(lhs.cRowBegin(i), lhs.cRowEnd(i), rhs.cColumnBegin(j), 0.0f)
                );
            }
        }
        return result;
    }

    friend self_type operator/(const self_type &lhs, T external_value)
    {
        return internalUnaryFuncApply(lhs, [&external_value](T val) {
            return val / external_value;
        });
    }

    void DumpInfo(const std::string &name)
    {
        auto result = boost::minmax_element(mData.cbegin(), mData.cend());
        std::cout << std::setprecision(std::numeric_limits<float>::digits10 + 1);
        std::cout << name << '['<< getRowCount() << 'x' << getColumnCount() << "] " << std::setw(8) << std::fixed << *(result.first) << "/" << *(result.second) << std::endl;
    }
};

template<typename T>
math::Matrix<T> make_matrix(const size_t rowCount, const size_t columnCount)
{
    Matrix<T> result;
    result.resize(rowCount, columnCount);
    return result;
}

template<typename T>
math::Matrix<T> make_unity_matrix(const size_t rowCount)
{
    auto result = make_matrix<T>(rowCount, rowCount);
    for (size_t i=0; i<rowCount; ++i)
    {
        result.set(i, i, static_cast<T>(1));
    }
    return result;
}

using MatrixF = Matrix<float>;

template<typename T>
T sum(const Matrix<T>& in)
{
    return std::accumulate(in.cBegin(), in.cEnd(), static_cast<T>(0));
}

template<typename T>
Matrix<T> columnSum(const Matrix<T>& in)
{
    auto result = make_matrix<T>(1, in.getColumnCount());
    for (size_t i = 0; i < in.getColumnCount(); ++i)
    {
        auto sum = std::accumulate(in.cColumnBegin(i), in.cColumnEnd(i), static_cast<T>(0));
        result.set(0, i, sum);
    }
    return result;
}

template<typename T>
Matrix<T> rowSum(const Matrix<T>& in)
{
    auto result = make_matrix<T>(in.getRowCount(), 1);
    for (size_t i = 0; i < in.getRowCount(); ++i)
    {
        auto sum = std::accumulate(in.cRowBegin(i), in.cRowEnd(i), static_cast<T>(0));
        result.set(i, 0, sum);
    }
    return result;
}

template<typename T>
Matrix<T> columnWiseSoftMax(const Matrix<T>& in)
{
    using c_ptr_t = float(__cdecl *)(float);
    c_ptr_t expf = std::expf;
    const auto r = in.apply(expf);
    const auto s = columnSum(r);
    auto result = r.divideWithRow(s);
    return  result;
}
}
