#pragma once
namespace Mathfix
{
    template <typename T>
    T mod(T a, T b)
    {
        if (b == 0)
        {
            return 0;
        }
        T result = a % b;
        return result >= 0 ? result : result + b;
    }

    template <typename T>
    T gcd(T a, T b)
    {
        if (a == 0) return b;
        if (b == 0) return a;
        if (a <= b) return gcd(b % a, a);
        return gcd(a % b, b);
    }
    template <typename T>
    T lcm(T a, T b)
    {
        return (a * b) / gcd(a, b);
    }
}