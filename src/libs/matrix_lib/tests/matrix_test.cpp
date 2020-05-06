#include <gtest/gtest.h>
#include <matrix/matrix.h>
#include <array>

namespace {
    using MatrixF = math::Matrix<float>;

    /**
     * Basic initialization test with getters and setters
    */
    TEST(MatrixTests, InitializationTest)
    {
        MatrixF UnityMatrix = {
            {1.0, 0.0},
            {0.0, 1.0}
        };

        ASSERT_EQ(UnityMatrix.Get(0, 0), 1.0f);
        ASSERT_EQ(UnityMatrix.Get(0, 1), 0.0f);
        ASSERT_EQ(UnityMatrix.Get(1, 0), 0.0f);
        ASSERT_EQ(UnityMatrix.Get(1, 1), 1.0f);

        MatrixF TestMatrix = UnityMatrix;
        ASSERT_EQ(UnityMatrix, TestMatrix);

        MatrixF Result = {
            {1.0, 1.0},
            {1.0, 1.0}
        };

        TestMatrix.Set(0, 1, 1.0f);
        TestMatrix.Set(1, 0, 1.0f);

        ASSERT_EQ(TestMatrix, Result);
        ASSERT_NE(TestMatrix, UnityMatrix);
    }

    TEST(MatrixTests, OperatorTest)
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

        ASSERT_EQ(MulResult.Apply(fx), UnityMatrix );
    }


    TEST(MatrixTests, IteratorTest)
    {
        MatrixF testMatrix = {
            {1.0f, 2.0f, 3.0f},
            {4.0f, 5.0f, 6.0f},
            {7.0f, 8.0f, 9.0f},
        };

        const std::array<float, 3> expectedRowResult = { 4.0f, 5.0f, 6.0f };
        const std::array<float, 3> expectedColumnResult = { 2.0f, 5.0f, 8.0f };

        std::vector<float> result;
        // traverse
        float value = 1.0f;

        for (auto it = testMatrix.Begin(); it != testMatrix.End(); ++it)
        {
            ASSERT_TRUE(*it == value);
            value += 1.0f;
        }

        // row iterator
        for (auto it = testMatrix.RowBegin(1); it != testMatrix.RowEnd(1); ++it)
        {
            result.push_back(*it);
        }
        ASSERT_TRUE(boost::equal(expectedRowResult, result));

        // column iterator
        result.clear();
        for (auto it = testMatrix.ColumnBegin(1); it != testMatrix.ColumnEnd(1); ++it)
        {
            result.push_back(*it);
        }
        ASSERT_TRUE(boost::equal(expectedColumnResult, result));
    }
}