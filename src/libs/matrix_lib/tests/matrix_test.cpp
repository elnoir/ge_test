#include <gtest/gtest.h>
#include <matrix/matrix.h>

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
}