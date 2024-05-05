#ifndef Expression_Generator_
#define Expression_Generator_

#include <iostream>
#include <unordered_map>
#include <stdlib.h>
#include <vector>
#include <random>
#include <time.h>
#include <stack>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <windows.h>

using namespace std;

// 多项式项数下限
#define PolynomialNumMin 4

// 多项式项数上限
#define PolynomialNumMax 8

// 题目(2)：四则运算式生成器
// 背景
// 某个小学老师请你为她编写一个能够自动生成四则运算练习题的小程序，要求输入年级和题目数量，程序根据年级输出规定数量的题目，具体要求如下：
// ·一年级运算数为20以内，只生成加法或减法运算式，结果小于20
// ·二年级运算数为50以内，生成加法和减法混合运算式，结果小于50
// ·三年级运算数为1000以内，生成加法、减法和乘法混合运算式
// ·四到六年级运算数为非负整数，生成至少带有三个运算符（从加减乘除四个符号中任选三个）和括号的混合运算式
// ·结果不能为负数

bool isValidInteger(const std::string& str)
{
    for (char c : str)
    {
        if (c == 0)
            break;
        else if (!std::isdigit(c))
        {
            return false;
        }
    }
    return true;
}

class Combinator
{
    default_random_engine mRandomEngine;
    uniform_int_distribution<int> mDistribution;
    uniform_int_distribution<int> mPolynomialNum;

    static unordered_map<int, int> mNumRanges;
    static unordered_map<int, int> mOutRanges;
    static unordered_map<int, char> mNumToOperators;
    static unordered_map<char, int> mOperatorToNums;
    static unordered_map<int, int> mClassToOperators;

public:
    Combinator()
    {
        mRandomEngine.seed(time(0));
        mPolynomialNum = uniform_int_distribution<int>(PolynomialNumMin, PolynomialNumMax);
    }

    vector<string> generateRandomExpression(int cls, int num)
    {
        if (num <= 0 || num > INT32_MAX || cls <= 0 || cls > 6)
            return {};

        // cout << "Num max: " << mNumRanges[cls] << "\n";
        mDistribution = uniform_int_distribution<int>(0, mNumRanges[cls]);
        vector<string> res;
        for (int i = 0; i < num;)
        {
            string exp;
            int exp_sz = mPolynomialNum(mRandomEngine);

            vector<char> clsToOperators = getClsToOperators(cls);
            int cls_sz = clsToOperators.size();
            for (int j = 0; j < exp_sz; j++)
            {
                if (j % 2 != 0)
                {
                    exp.push_back(clsToOperators[mPolynomialNum(mRandomEngine) % cls_sz]);
                }
                else
                {   
                    int n = mDistribution(mRandomEngine);
                    exp += to_string(n);
                }
            }

            if (exp_sz % 2 == 0)
            {
                exp += to_string(mDistribution(mRandomEngine));
            }
            
            if (cls > 3)
            {   // 随机加入括号
                int len = exp.size();
                int idx = mDistribution(mRandomEngine) % 2 != 0 ? 0 : len / 2;
                if (idx != 0)
                {
                    while (idx < len && exp[idx] >= '0' && exp[idx] <= '9')
                    {
                        idx++;
                    }
                    if (idx < len - 1)
                    {
                        exp.insert(exp.begin() + idx + 1, '(');
                        exp.push_back(')');
                    }

                }
                else
                {
                    idx = len / 2;
                    while (idx < len && exp[idx] >= '0' && exp[idx] <= '9')
                    {
                        idx++;
                    }
                    if (idx < len)
                    {
                        exp.insert(exp.begin(), '(');
                        exp.insert(exp.begin() + idx + 1, ')');
                    }
                }
            }

            if (isValid(exp) && isOperatorNumValid(exp, cls))
            {
                float output = computeResult(exp);
                if (output < 0 || output > mNumRanges[cls])
                    continue;
                else
                {
                    res.emplace_back(exp);
                    i++;
                }
            }
        }
        return res;
    }

    bool isOperatorNumValid(string str, int cls)
    {
        int len = str.length();
        if (len < 1)
            return false;

        unordered_map<char, int> mOperatorNumsMap;

        for (int i = 0; i < len; i++)
        {
            if (mOperatorToNums.count(str[i]))
            {
                if (!mOperatorNumsMap.count(str[i]))
                    mOperatorNumsMap.emplace(str[i], 1);
                else
                    mOperatorNumsMap[str[i]]++;
            }
        }

        int cls_num = 0;
        for (auto it = mOperatorNumsMap.begin(); it != mOperatorNumsMap.end(); it++)
        {
            if ((*it).second)
                cls_num++;
        }

        // cout << cls_num << "\n";
        return cls_num >= mClassToOperators[cls];        
    }

    vector<char> getClsToOperators(int cls)
    {
        vector<char> res;
        if (cls == 1)
        {
            if (mDistribution(mRandomEngine) % 2 != 0)
                res.emplace_back('-');
            else
                res.emplace_back('+');
        }
        else if (cls == 2)
        {
            res.emplace_back('+');
            res.emplace_back('-');
        }
        else if (cls == 3)
        {
            res.emplace_back('+');
            res.emplace_back('-');
            res.emplace_back('*');
        }
        else if (cls > 3)
        {
            res.emplace_back('+');
            res.emplace_back('-');
            res.emplace_back('*');
            res.emplace_back('/');
        }
        return res;
    }

    float computeResult(string exp)
    {
        int len = exp.length();
        if (len < 1)
            return -1;
        
        float res;
        stack<string> s;
        for (int i = 0; i < len; i++)
        {
            if (exp[i] == ' ')
                continue;
            else if (exp[i] >= '0' && exp[i] <= '9')
            {
                string num_str1 = findNumString(exp, i, i);
                
                if ((int)s.size() > 1)
                {
                    string sign_str = s.top();
                    if (!strcmp(sign_str.c_str(), "*"))
                    {
                        s.pop();
                        string num_str2 = s.top();
                        s.pop();

                        // cout << num_str2 << " " << num_str1 << "\n";
                        res = stof(num_str1) * stof(num_str2);
                        s.push(to_string(res));
                    }
                    else if (!strcmp(sign_str.c_str(), "/"))
                    {
                        s.pop();
                        string num_str2 = s.top();
                        s.pop();

                        res = stof(num_str2) / stof(num_str1);
                        s.push(to_string(res));
                    }
                    else
                        s.push(num_str1);
                }
                else
                    s.push(num_str1);
            }
            else if (exp[i] == '(')
            {
                s.push(string(exp.begin() + i, exp.begin() + i + 1));
            }
            else if (mOperatorToNums.count(exp[i]) && mOperatorToNums[exp[i]] < 2)
            {
                if ((int)s.size() > 2)
                {
                    string num_str1 = s.top();
                    s.pop();

                    string sign_str = s.top();
                    s.pop();

                    string num_str2 = s.top();
                    s.pop();

                    if (!strcmp(sign_str.c_str(), "+"))
                    {
                        res = stof(num_str1) + stof(num_str2);
                        s.push(to_string(res));
                    }
                    else if (!strcmp(sign_str.c_str(), "-"))
                    {
                        res = stof(num_str2) - stof(num_str1);
                        s.push(to_string(res));
                    }
                    else
                    {
                        s.push(num_str2);
                        s.push(sign_str);
                        s.push(num_str1);
                    }
                }
                s.push(string(exp.begin() + i, exp.begin() + i + 1));
            }
            else if (mOperatorToNums.count(exp[i]) && mOperatorToNums[exp[i]] > 1)
            {
                s.push(string(exp.begin() + i, exp.begin() + i + 1));
            }
            else if (exp[i] == ')')
            {
                // cout << "right..." << "\n";

                string num_str1 = s.top();
                s.pop();

                string sign_str = s.top();
                s.pop();
                if (!strcmp(sign_str.c_str(), "("))
                {
                    s.push(num_str1);
                }
                else
                {
                    string num_str2 = s.top();
                    s.pop();

                    if (!strcmp(sign_str.c_str(), "+"))
                    {
                        res = stof(num_str1) + stof(num_str2);
                        s.pop();
                        s.push(to_string(res));
                    }
                    else if (!strcmp(sign_str.c_str(), "-"))
                    {
                        res = stof(num_str2) - stof(num_str1);
                        s.pop();
                        s.push(to_string(res));
                    }
                }
            }
            else
            {
                cout << "Exception..." << "\n";
            }
            
            if (i == len - 1)
            {
                // cout << "stack size: " << s.size() << "\n";

                if ((int)s.size() > 2)
                {
                    // cout << (int)s.size() << "\n";

                    string num_str1 = s.top();
                    s.pop();

                    string sign_str = s.top();
                    s.pop();

                    string num_str2 = s.top();
                    s.pop();

                    if (!strcmp(sign_str.c_str(), "+"))
                    {
                        res = stof(num_str1) + stof(num_str2);
                        s.push(to_string(res));
                    }
                    else if (!strcmp(sign_str.c_str(), "-"))
                    {
                        res = stof(num_str2) - stof(num_str1);
                        s.push(to_string(res));
                    }
                    else if (!strcmp(sign_str.c_str(), "*"))
                    {
                        res = stof(num_str1) * stof(num_str2);
                        s.push(to_string(res));
                    }
                    else if (!strcmp(sign_str.c_str(), "/"))
                    {
                        res = stof(num_str2) / stof(num_str1);
                        s.push(to_string(res));
                    }
                }
            }
        }

        res = stof(s.top());
        return res;
    }

    bool isNumber(string str)
    {
        int len = str.length();
        for (int i = 0; i < len; i++)
        {
            if (str[i] < '0' || str[i] > '9')
                return false; 
        }
        return true;
    }

    string findNumString(string exp, int st, int& end)
    {
        int len = exp.length();

        int i = st + 1;
        for (; i < len; i++)
        {
            if (exp[i] < '0' || exp[i] > '9')
                break;
        }

        end = i - 1;
        return string(exp.begin() + st, exp.begin() + i);
    }

    bool isValid(string str)
    {
        int len = str.length();
        if (!len)
            return false;

        stack<char> s;
        char ch = str[0];
        if (mOperatorToNums.count(ch))
            return false;
        s.push(ch);

        for (int i = 1; i < len; i++)
        {
            if (str[i] == '0')
            {   // 考虑除以0的情况
                char ch = s.top();
                if (ch == '/')
                    return false;
                s.push(str[i]);
            }
            else if (str[i] > '0' || str[i] <= '9')
            {
                s.push(str[i]);
            }
            else if (mOperatorToNums.count(str[i]) || str[i] == '(')
            {
                if (i == len - 1)
                    return false;
                else if (mOperatorToNums.count(str[i + 1]) || str[i+1] == ')')
                    return false;
                s.push(str[i]);
            }
            else if (str[i] == ')')
            {
                bool sign = false;
                if (mOperatorToNums.count(str[i - 1]))
                    return false;

                while (!s.empty())
                {
                    char ch = s.top();
                    s.pop();

                    if (ch == '(')
                    {
                        sign = true;
                        break;
                    }
                }
                if (!sign)
                    return false;
            }
            else
                return false;
        }
        return true;
    }
};

unordered_map<int, int> Combinator::mNumRanges = {
    {1, 20},
    {2, 50},
    {3, 1000},
    {4, 1000},
    {5, 1000},
    {6, 10000}
};

unordered_map<int, int> Combinator::mOutRanges = {
    {1, 20},
    {2, 50},
    {3, 1000000},
    {4, 1000000},
    {5, 1000000},
    {6, 100000000}
};

unordered_map<int, char> Combinator::mNumToOperators = {
    {0, '+'},
    {1, '-'},
    {2, '*'},
    {3, '/'},
};

unordered_map<char, int> Combinator::mOperatorToNums = {
    {'+', 0},
    {'-', 1},
    {'*', 2},
    {'/', 3},
};

unordered_map<int, int> Combinator::mClassToOperators = {
    {1, 1},
    {2, 2},
    {3, 3},
    {4, 3},
    {5, 3},
    {6, 3},
};
#endif

// int main()
// {
//     // 设置utf-8编码
//     SetConsoleCP(CP_UTF8);
//     SetConsoleOutputCP(CP_UTF8);

//     int cls = 0, n = 0;
//     Combinator comb;

//     while (true)
//     {
//         cout << "请输入班级号+运算式数量：" << "\n";
//         cin >> cls >> n;

//         cout << "Expressions:" << " ";
//         vector<string> exps = comb.generateRandomExpression(cls, n);
//         for (auto e : exps)
//         {
//             cout << e << " ";
//         }
//         cout << "\n";

//         cout << "Outputs: " << " ";
//         for (auto e : exps)
//         {
//             float res = comb.computeResult(e);
//             if (cls < 4)
//                 cout << int(res) << " ";
//             else
//                 cout << res << " ";
//         }
//         cout << "\n";
//     }

//     return 0;
// }