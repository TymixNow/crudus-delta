#pragma once
template <typename T>
T mod(T a, T b)
{
    T result = a % b;
    return result >= 0 ? result : result + b;
}