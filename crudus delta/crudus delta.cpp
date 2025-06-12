#define DELTA_TITLE "         []                 [][]                                        \n        [][]           //      []                                       \n       []  []                   []                                      \n      []    []       [][][]      []         [][][][][]   [][][]  []     \n     []      []     []          [][]       []   []      []    [][]      \n    []        []     [][][]    []  []           []      []     []       \n   []          []   []        []    []          []      []    [][]  []  \n  [][][][][][][][]   [][][]  []      [][]        [][]    [][][]  [][]   \n"
//
//         []                 [][]                                        
//        [][]           //      []                                       
//       []  []                   []                                      
//      []    []       [][][]      []         [][][][][]   [][][]  []     
//     []      []     []          [][]       []   []      []    [][]      
//    []        []     [][][]    []  []           []      []     []       
//   []          []   []        []    []          []      []    [][]  []  
//  [][][][][][][][]   [][][]  []      [][]        [][]    [][][]  [][]   
//
#include <iostream>
#include <vector>
#include "communicator.hpp"
#include "wave_import.hpp"
#include "freqmod.hpp"
#include "integral_bass.hpp"
#include "wavstream.hpp"
#include "waveformer.hpp"
#include "envelope.hpp"
#define charwave Synth::wave<sample>
enum Colour { K, B, G, C, R, M, Y, W, V };
Colour b(Colour a)
{
    return (Colour)((int)a * 16);
}
Colour b(int a)
{
    return (Colour)(a * 16);
}
class wave_or_sample
{
public:
    charwave w()
    {
        if (is_int) throw "wrong Polish notation syntax";
        else return wa;
    }
    sample i()
    {
        if(!is_int) throw "wrong Polish notation syntax";
        else return in;
    }
    wave_or_sample(sample num)
    {
        in = num;
        is_int = true;
    }
    wave_or_sample(charwave wv)
    {
        wa = wv;
        is_int = false;
    }
//    ~wave_or_sample();

private:
    charwave wa;
    sample in;
    bool is_int;
};
void run(std::vector<char> commands, std::vector<wave_or_sample>& stack, std::vector<charwave>& wavedata, std::ifstream& stream, sample t)
{
    if (commands.empty()) return;
    char back = commands.back();
    commands.pop_back();
    charwave carrier;
    charwave modulator;
    sample a;
    sample b;
    if (back >= '0' && back <= '9')
    {
        stack.push_back((sample)(back - '0'));
        goto end;
    }
    switch (back)
    {
    case 'N':
        a = stack.back().i();
        stack.pop_back();
        b = stack.back().i();
        stack.pop_back();
        stack.push_back((10 * a) + b);
        break;
    case 'W':
        a = stack.back().i();
        stack.pop_back();
        while (wavedata.size() <= a)
        {
            charwave wv;
            WaveImport::get(stream, wv);
            wavedata.push_back(wv);
        }
        stack.push_back(wavedata[a]);
        break;
    case 'F':
        carrier = stack.back().w();
        stack.pop_back();
        modulator = stack.back().w();
        stack.pop_back();
        stack.push_back(Synth::fm(carrier, modulator));
        break;
    case 'f':
        carrier = stack.back().w();
        stack.pop_back();
        modulator = stack.back().w();
        stack.pop_back();
        a = stack.back().i();
        stack.pop_back();
        stack.push_back(Synth::fm(carrier, modulator, a/256.0));
        break;
    case 'I':
        carrier = stack.back().w();
        stack.pop_back();
        stack.push_back(Synth::prefix_sum(carrier));
        break;
    case 'B':
        carrier = stack.back().w();
        stack.pop_back();
        stack.push_back(Synth::odd_bass(carrier));
        break;
    case 'G':
        a = stack.back().i();
        stack.pop_back();
        carrier = charwave(a);
        carrier[0] = 127;
        stack.push_back(carrier);
        break;
    case 'Q':
        a = stack.back().i();
        stack.pop_back();
        carrier = charwave(2 * a);
        for (int i = 0; i < a; i++)
        {
            carrier[i] = 127;
            carrier[i + a] = -127;
        }
        stack.push_back(carrier);
        break;
    case 't':
        stack.push_back(t);
        break;
    default:
        break;
    }
    end: 
    run(commands, stack, wavedata, stream, t);
}
void general()
{
    std::string ifilename = comms::Get("input wave list file path: ", false, V | C, true, V | W | b(C));
    std::ifstream file(ifilename);
    std::string commands = comms::Get("input waveshaper command (Polish notation): ", false, V | B, true, V | W | b(B));
    std::string note_file = comms::Get("input notation file: ", false, V | G, true, V | W | b(G));
    int transposition = stoi(comms::Get("octaves of transposition (basswards): ", false));
    comms::Say(std::to_string(transposition));
    double damping = stod(comms::Get("dampening: ", false));
    comms::Say(std::to_string(damping));

    std::ifstream notation(note_file);
    WavWrapper::wavstream wav;
    double t = 0;
    std::vector<int> notes;
    std::vector<double> lengths;
    do
    {
        int note;
        double length;
        notation >> note;
        if (note == 0) break;
        note -= transposition * 12;
        notation >> length;
        length *= (44100.0 / 128 / 128);
        notes.push_back(note);
        lengths.push_back(length);
    } while (true);

    for (int j = 0; j < notes.size(); j++)
    {
        std::vector<charwave> wavedata;
        envelope e(damping);
        for (sample i = 1; i > 0; i++)
        {
            std::vector<char> cmds;
            for (int k = 0; k < commands.size(); k++)
            {
                cmds.push_back(commands[k]);
            }
            std::vector<wave_or_sample> stack;
            run(cmds, stack, wavedata, file, i);
            Waveformer::Waveform waveform(stack[0].w(), t);
            e << waveform.Render(notes[j], lengths[j]);
            waveform.sync(t);
        }
        e.Render();
        std::vector<sample> f;
        e >> f;
        wav << f;
    }



    std::string ofilename = comms::Get("input result file path: ", false, V | M, true, V | W | b(M));
    wav >> ofilename;
    comms::Say("It worked!", true, V | Y, true);
    comms::Say();
}
void description()
{
    std::string desc[17] =
    {
        "1. Wave file - a file with a list of waves written in a format, where each line is a wave, made up of numbers representing samples, with spaces between them. Can be left empty if unused.",
        "2. Waveshaper - accepts a single wave as final output, Polish notation executed by concatenation without spaces, the available opertors in the Polish notation are: ",
        "   2a. digits 0-9 - digit literals - functions with zero inputs(), output always equal to the digit;",
        "   2b. N - decimal concatenation operator - takes two numerical inputs(a,b), output equal to 10*a + b;",
        "   2c. W - wave import operator - takes one numerical input(a), output is the a-th wave in the Wave file;",
        "   2d. F - simple frequency modulation operator - takes in two wave inputs(A,B), output is the wave created from freqency modultion synthesis, with A as carrier, B as modulator",
        "   2e. f - scaled frequaency modulation operator - takes in two wave inputs and one numerical input(A,B,c), output is the wave created from freqency modultion synthesis, with A*c as carrier, B as modulator",
        "   2f. I - inversion operator - takes in one wave input(A), inverts the voltage and time of every other waveform",
        "   2g. B - integral / prefix sum / phase accumulator operator - takes in one wave input(A), output is the inegral of input with respect to time, normalised",
        "   2h. G - simple generation operator - takes in one numerical input(a), output is a wave with one sample of voltage 127 and (a - 1) samples of voltage 0",
        "   2i. Q - square wave operator - takes in one numerical input(a), output is a square wave with (2*a) samples",
        "   2j. t - time operator - takes in zero inputs, output is the number of wave cycles since the beginning of the note",
        "example usage: \"BIQ2\"",
        "3. Notation file - file with pairs of midi note numbers and note lengths, test file - \"Music.txt\" - included in program",
        "4. Transposition - number of octaves, the notes should be lowered, accepts negative numbers for heightening",
        "5. Dampening - the volume of the sound is multiplied by this number every frame, default value: 0.999",
        "6. Result - a .wav file to be overwritten with the sound"
    };
    for (auto& line : desc)
    {
        comms::Say(line, true, V | G, true);
    }
}
int main()
{
    comms::Say(DELTA_TITLE, true, V | G, true);
    bool quit = false;
    std::string input;
    do
    {
        input = comms::Get("Run program (R), read description (H), quit (Q): ", false, V | G, true, V | W);
        switch (input[0])
        {
        case'R':
        case'r':
            general();
            break;
        case'H':
        case'h':
            description();
            break;
        case 'q':
        case 'Q':
            quit = true;
            break;
        default:
            break;
        }
    } while (!quit);
}