#include <gtest/gtest.h>
#include <matrix/matrix.h>
#include <array>

namespace {
    using MatrixF = math::MatrixF;

    /**
     * Basic initialization test with getters and setters
    */
    TEST(MatrixTests, InitializationTest)
    {
        MatrixF UnityMatrix = {
            {1.0, 0.0},
            {0.0, 1.0}
        };

        ASSERT_EQ(UnityMatrix.get(0, 0), 1.0f);
        ASSERT_EQ(UnityMatrix.get(0, 1), 0.0f);
        ASSERT_EQ(UnityMatrix.get(1, 0), 0.0f);
        ASSERT_EQ(UnityMatrix.get(1, 1), 1.0f);

        ASSERT_EQ(UnityMatrix.getColumnCount(), 2);
        ASSERT_EQ(UnityMatrix.getRowCount(), 2);

        MatrixF TestMatrix = UnityMatrix;
        ASSERT_EQ(UnityMatrix, TestMatrix);

        TestMatrix.set(0, 1, 1.0f);
        TestMatrix.set(1, 0, 1.0f);

        const MatrixF Result = {
            {1.0, 1.0},
            {1.0, 1.0}
        };

        ASSERT_EQ(TestMatrix, Result);
        ASSERT_NE(TestMatrix, UnityMatrix);
    }


    TEST(MatrixTests, BasicOperatorTest)
    {
        MatrixF UnityMatrix = {
            {1.0, 0.0},
            {0.0, 1.0}
        };

        MatrixF OtherMatrix = {
            {0.0, 1.0},
            {2.0, 3.0}
        };
        MatrixF AddResult = {
            {1.0, 1.0},
            {2.0, 4.0}
        };

        MatrixF TestResult = UnityMatrix + OtherMatrix;

        ASSERT_EQ(TestResult, AddResult);
        ASSERT_EQ(TestResult - OtherMatrix, UnityMatrix);

        MatrixF MulResult = {
            {2.5, 0.0},
            {0.0, 2.5}
        };

        ASSERT_EQ(UnityMatrix * 2.5f, MulResult);
        ASSERT_EQ(MulResult - UnityMatrix * 1.5f, UnityMatrix);
        ASSERT_EQ(MulResult / 2.5f, UnityMatrix);

        std::function<float(float)> fx = [](float value) {
            return value / 2.5f;
        };

        ASSERT_EQ(MulResult.apply(fx), UnityMatrix);
    }


    TEST(MatrixTests, IteratorTest)
    {
        MatrixF testMatrix2 = {
            {1.0f, 2.0f, 3.0f},
            {4.0f, 5.0f, 6.0f},
            {7.0f, 8.0f, 9.0f},
        };

        const auto &testMatrix = testMatrix2;

        const std::array<float, 3> expectedRowResult = { 4.0f, 5.0f, 6.0f };
        const std::array<float, 3> expectedColumnResult = { 2.0f, 5.0f, 8.0f };

        std::vector<float> result;
        // const version
        // traverse
        float value = 1.0f;

        for (auto it = testMatrix.cBegin(); it != testMatrix.cEnd(); ++it)
        {
            ASSERT_TRUE(*it == value);
            value += 1.0f;
        }

        // row iterator
        for (auto it = testMatrix.cRowBegin(1); it != testMatrix.cRowEnd(1); ++it)
        {
            result.push_back(*it);
        }
        ASSERT_TRUE(boost::equal(expectedRowResult, result));

        // column iterator
        result.clear();
        for (auto it = testMatrix.cColumnBegin(1); it != testMatrix.cColumnEnd(1); ++it)
        {
            result.push_back(*it);
        }
        ASSERT_TRUE(boost::equal(expectedColumnResult, result));

        // non const version
        value = 1.0f;
        for (auto it = testMatrix2.begin(); it != testMatrix2.end(); ++it)
        {
            ASSERT_TRUE(*it == value);
            value += 1.0f;
        }

        result.clear();
        // row iterator
        for (auto it = testMatrix2.rowBegin(1); it != testMatrix2.rowEnd(1); ++it)
        {
            result.push_back(*it);
        }
        ASSERT_TRUE(boost::equal(expectedRowResult, result));

        // column iterator
        result.clear();
        for (auto it = testMatrix2.columnBegin(1); it != testMatrix2.columnEnd(1); ++it)
        {
            result.push_back(*it);
        }
        ASSERT_TRUE(boost::equal(expectedColumnResult, result));
    }


    TEST(MatrixTests, Multiplication)
    {
        const MatrixF operand1 = {
            {5.0f, 8.0f, -4.0f},
            {6.0f, 9.0f, -5.0f},
            {4.0f, 7.0f, -2.0f},
        };

        const MatrixF operand2 = {
            {2.0f},
            {-3.0f},
            {1.0f}
        };

        const MatrixF expectedResult = {
            {-18.0f},
            {-20.0f},
            {-15.0f}
        };

        const auto result = operand1 * operand2;

        ASSERT_TRUE(result == expectedResult);
    }


    TEST(MatrixTests, Transpose)
    {
        const MatrixF UnityMatrix = {
            {1.0f, 0.0f},
            {0.0f, 1.0f}
        };

        ASSERT_TRUE(UnityMatrix == UnityMatrix.transpose());

        const MatrixF row3 = {
            {1.0f, 2.0f, 3.0f}
        };

        const auto column3 = row3.transpose();

        for (int i = 0; i < row3.getColumnCount(); ++i)
        {
            ASSERT_EQ(row3.get(0, i), column3.get(i, 0));
        }

        ASSERT_EQ(row3, row3.transpose().transpose());
    }
}