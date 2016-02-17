#ifndef PSEUDO_JSON__DECODER_H_
#define PSEUDO_JSON__DECODER_H_
#include <fstream>
#include <stack>
#include <string>

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
    void decode(const char* filename);
    void decode();
    const Value* value() const;

private:
    void decode_per_char();
};

}

#endif