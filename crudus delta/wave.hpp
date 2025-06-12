#pragma once
#include <vector>
#include "math_fix.hpp"
namespace Synth
{
    using namespace std;
    template <class T>
    class wave
    {
    private:
        vector<T> data;
    public:
        wave(void) { data = {}; };
        wave(vector<T> d) { data = d; };
        wave(int x) { data = vector<T>(x); };
        ~wave() {};
        T& operator[](int i)
        {
            return data[Mathfix::mod(i, (int)data.size())];
        }
        int size()
        {
            return data.size();
        }
        vector<T> get()
        {
            return data;
        }
    };
} // namespace Synth