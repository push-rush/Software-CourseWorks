#include <iostream>
#include <unordered_map>
#include <stdlib.h>
#include <vector>

using namespace std;

// 题目(1)：最大连续子数组和（最大子段和）
// 背景
// 问题： 给定n个整数（可能为负数）组成的序列a[1],a[2],a[3],…,a[n],求该序列如a[i]+a[i+1]+…+a[j]的子段和的最大值。当所给的整数均为负数时定义子段和为0，依此定义，所求的最优值为： Max{0,a[i]+a[i+1]+…+a[j]},1<=i<=j<=n
// 例如，当（a[1],a[2],a[3],a[4],a[5],a[6]）=(-2,11,-4,13,-5,-2)时，最大子段和为20。

class Solution
{
public:
    int maxSubArrays(vector<int>& nums)
    {
        int sz = nums.size();
        vector<int>dp(sz, 0);
        for (int i = 1; i <= sz; i++)
        {
            if (i == 1)
                dp[i - 1] = max(nums[i], 0);
            else
            {
                dp[i - 1] = max(dp[i - 2] + nums[i], nums[i]);
            }
        }
        
        int max = 0;
        for (int i = 0; i < sz; i++)
        {
            if (dp[i] > max)
                max = dp[i];
        }
        return max;
    }
};

int main()
{
    int n = 0;
    cin >> n;

    vector<int> nums(n + 1, 0);
    int in = 0;
    for (int i = 1; i <= n; i++)
    {
        cin >> in;
        nums[i] = in;
    }
    Solution so;
    int max = so.maxSubArrays(nums);
    cout << max << "\n";

    return 0;
}