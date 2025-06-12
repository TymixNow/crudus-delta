#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
namespace comms
{
    using namespace std;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    void Say(string line = "", bool end = true, int colour = 15, bool reset = true)
    {
        SetConsoleTextAttribute(hConsole, colour);
        cout << line;
        if (end) cout << endl;
        if (reset) SetConsoleTextAttribute(hConsole, 15);
    }
    string Get()
    {
        string input;
        getline(cin, input);
        return input;
    }
    vector<string> Get(int length)
    {
        vector<string> out;
        for (int i = 0; i < length; i++)
        {
            out.push_back(Get());
        }
        return out;
    }
    string Get(string line, bool end = true, int colour = 15, bool reset = true, int input_box = 256)
    {
        if (input_box == 256) input_box = colour;
        Say(line, end, colour, reset);
        Say("", false, input_box, false);
        return Get();
        Say("", false);
    }
    vector<string> Get(int length, string line, bool end = true, int colour = 15, bool reset = true, int input_box = 256)
    {
        if (input_box == 256) input_box = colour;
        Say(line, end, colour, reset);
        Say("", false, input_box, false);
        return Get(length);
        Say("", false, 0, true);
    }
    template<typename T>
    int GetFrom(vector<T> list)
    {
        do
        {
            T result;
            cin >> result;
            auto pointer = find(list.begin(), list.end(), result);
            if (pointer != list.end())
            {
                return distance(list.begin(), pointer);
            }
        }
        while (true);
    }
}