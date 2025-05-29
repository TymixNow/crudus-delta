#ifndef WAV_STREAM_HPP
#define WAV_STREAM_HPP

#include <fstream>
#include <sstream>
#include <vector>

#define FILE_ADD file_output +=
#define sample char

namespace WavWrapper
{
    using namespace std;

    void operator+=(vector<sample> &a, vector<sample> b)
    {
        a.insert(a.end(), b.begin(),b.end());
    }
    void operator+=(vector<sample> &a, string b)
    {
        vector<sample> c(b.begin(),b.end());
        a.insert(a.end(), c.begin(),c.end());
    }
    template<typename T>
    vector<sample> byte_array(T number, short bytes)
    {
        vector<sample> out = {};
        T current = number;
        for (short i = 0; i < bytes; i++)
        {
            out.push_back(current & 0xff)
            current >>= 8;
        }
        return out;
    }
    vector<sample> byte_array(int number)
    {
        return byte_array(number, 4);
    }
    
    class wavstream
    {
    private:
        vector<sample> data;
        int sample_rate;
    public:
        void operator<<(vector<sample> input)
        {
            data += input;
        }
        void operator<<(string input)
        {
            data += input;
        }
        void operator>>(string filename)
        {
            ofstream file(filename, ios::out | ios::binary);
            size_prefix();
            auto temp = data;
            data.clear();
            data += "WAVE";
            data += "fmt ";
            //constant block size
            data += byte_array(16);
            //constant audio format
            data += byte_array(1,2);
            //channels
            data += byte_array(1,2);
            //freq
            data += byte_array(sample_rate);
            // bytes/sec
            data += byte_array(sample_rate);
            // bytes/block
            data += byte_array(1,2);
            // bits/sample
            data += byte_array(8,2);
    
            data += "data";
            data += temp;

            size_prefix();

            temp = data;
            data.clear();

            data += "RIFF";

            data += temp;

            for (auto ch : data)
            {
                file << ch;
            }

            data.clear();
        }
        void size_prefix()
        {
            int size = data.size();
            vector<sample> length = byte_array(size);
            auto temp = data;
            data = length;
            data += temp;
        }
        wavstream(int rate = 44100);
        ~wavstream();
    };
    
    wavstream::wavstream(int rate)
    {
        sample_rate = rate;
    }
    
    wavstream::~wavstream()
    {
    }
} // namespace WavWrapper
#endif
