#ifndef PSEUDO_JSON__DECODER_H_
#define PSEUDO_JSON__DECODER_H_
#include <fstream>
#include <stack>
#include <string>
#include "PseudoJson/Data.h"

namespace PseudoJson
{

class Value;

class Decoder
{
private:
    enum State
    {
        StateUnknown,
        StateValue,
        StateInValue,
        StateNumber,
        StateInNumber,
        StateObject,
        StateObjectKey,
        StateArray,
    };

public:
    static std::string debug_string(State s);

private:
    Value* value_;
    std::stack< State > states_;
    std::stack< Value* > paths_;
    std::stack< std::string > keys_;
    std::string token_;
    std::ifstream ifs_;

public:
    Decoder(const char* filename);
    Decoder();
    ~Decoder();
    Data data() const;
    void decode(const char* filename);
    void decode();

private:
    void decode_per_char();
};

}

#endif
