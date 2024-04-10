#include <windows.h>
#include <gtest/gtest.h>

#include "./src/MaxSubArray.cpp"

// 定义测试套件
// class MaxSubArraysTest : public ::testing::Test 
// {
// protected:
//     // 在每个测试用例之前设置
//     void SetUp() override 
//     {
//         // 可以在这里进行一些初始化操作
//     }

//     // 在每个测试用例之后清理
//     void TearDown() override 
//     {
//         // 可以在这里进行一些清理操作
//     }

//     // 可以添加其他测试用例级别的设置/清理方法
// };

// // 定义测试用例
// TEST_F(MaxSubArraysTest, AllPositiveNumbers)
//  {
//     Solution so;
//     std::vector<int> nums = {1, 2, 3, 4, 5};
//     EXPECT_EQ(so.maxSubArrays(nums), 15);
// }

// TEST_F(MaxSubArraysTest, AllNegativeNumbers)
// {
//     Solution so;
//     std::vector<int> nums = {-1, -2, -3, -4, -5};
//     EXPECT_EQ(so.maxSubArrays(nums), 0);
// }

// TEST_F(MaxSubArraysTest, MixedPositiveAndNegativeNumbers) 
// {
//     Solution so;
//     std::vector<int> nums = {-2, 11, -4, 13, -5, -2};
//     EXPECT_EQ(so.maxSubArrays(nums), 20);
// }

// TEST_F(MaxSubArraysTest, EmptyArray) 
// {
//     Solution so;
//     std::vector<int> nums;
//     EXPECT_EQ(so.maxSubArrays(nums), 0);
// }

// TEST_F(MaxSubArraysTest, SinglePositiveNumber) 
// {
//     Solution so;
//     std::vector<int> nums = {6};
//     EXPECT_EQ(so.maxSubArrays(nums), 6);
// }

// TEST_F(MaxSubArraysTest, SingleNegativeNumber) 
// {
//     Solution so;
//     std::vector<int> nums = {-6};
//     EXPECT_EQ(so.maxSubArrays(nums), 0);
// }

// TEST_F(MaxSubArraysTest, SingleZeroNumber) 
// {
//     Solution so;
//     std::vector<int> nums = {0};
//     EXPECT_EQ(so.maxSubArrays(nums), 0);
// }

// TEST_F(MaxSubArraysTest, ContainZero) 
// {
//     Solution so;
//     std::vector<int> nums = {0, 1, 2, 0, 3, 0};
//     EXPECT_EQ(so.maxSubArrays(nums), 6);
// }

// TEST_F(MaxSubArraysTest, ContainMultipleContinuousZeros) 
// {
//     Solution so;
//     std::vector<int> nums = {0, 0, 0, 0, 0, 0};
//     EXPECT_EQ(so.maxSubArrays(nums), 0);
// }

// TEST_F(MaxSubArraysTest, ContainMultipleRepeatingNumbers) 
// {
//     Solution so;
//     std::vector<int> nums = {2, 2, 2, 2, 2, 2};
//     EXPECT_EQ(so.maxSubArrays(nums), 12);
// }

// // 添加其他测试用例...

// // 定义main函数来运行测试
// int main(int argc, char **argv) 
// {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

int main()
{
    // 设置utf-8编码
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Solution so;
    int n = 0;

    while (true)
    {
        cout << "输入测试用例：" << "\n";
        cin >> n;
        vector<int> nums(n, 0);
        int in = 0;
        for (int i = 0; i < n; i++)
        {
            cin >> in;
            nums[i] = in;
        }
        int max = so.maxSubArrays(nums);
        cout << "Output: " << max << "\n";
    }

    return 0;
}