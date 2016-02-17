#include "PseudoJson/Decoder.h"
#include <cassert>
#include <fstream>
#include <stack>
#include <string>
#include "PseudoJson/Array.h"
#include "PseudoJson/Object.h"
#include "PseudoJson/Value.h"

namespace PseudoJson
{

std::string Decoder::debug_string(State s)
{
    std::string r("");

    switch (s)
    {
        case StateUnknown: r = "Unknown"; break;
        case StateValue: r = "Value"; break;
        case StateInValue: r = "InValue"; break;
        case StateObject: r = "Object"; break;
        case StateObjectKey: r = "Key"; break;
        case StateArray: r = "Array"; break;
        default: r = "****"; break;
    }

    return r;
}

Decoder::Decoder(const char* filename)
:   value_(0),
    states_(), paths_(), keys_(), token_(),
    ifs_(filename, std::ios_base::binary)
{}

Decoder::Decoder()
: value_(0), states_(), paths_(), keys_(), token_(), ifs_()
{}

Decoder::~Decoder()
{}

void Decoder::decode(const char* filename)
{
    ifs_.open(filename, std::ios_base::binary);
    decode();
}

void Decoder::decode()
{
    if (states_.empty())
    {
        states_.push(StateUnknown);
    }

    decode_per_char();
}

const Value* Decoder::value() const { return value_; }

void Decoder::decode_per_char()
{
    Value* v = 0;
    char c = ifs_.get();

    if (ifs_.eof() || states_.empty())
    {
        assert(paths_.empty());
        ifs_.close();
        return;
    }

    assert(!ifs_.fail());

    switch (states_.top())
    {
        case StateUnknown:
        switch (c)
        {
            case '"':
            ifs_.unget();
            states_.pop();
            states_.push(StateValue);
            break;

            case '[':
            ifs_.unget();
            states_.pop();
            states_.push(StateArray);
            break;

            case ']':
            ifs_.unget();
            states_.pop();
            break;

            case '{':
            ifs_.unget();
            states_.pop();
            states_.push(StateObject);
            break;

            case '}':
            ifs_.unget();
            states_.pop();
            break;

            default:
            break;
        }
        break;

        case StateValue:
        switch (c)
        {
            case '"':
            token_ = "";
            states_.pop();
            states_.push(StateInValue);
            break;

            default:
            break;
        }
        break;

        case StateInValue:
        switch (c)
        {
            case '"':
            states_.pop();
            v = new Value(token_);
            token_ = "";

            if (states_.top() == StateArray)
            {
                paths_.top()->get_array()->push_back(v);
            }

            if (states_.top() == StateObject)
            {
                paths_.top()->get_object()->insert(keys_.top(), v);
                keys_.pop();
            }

            break;

            default:
            token_ += c;
            break;
        }
        break;

        case StateArray:
        switch (c)
        {
            case '[':
            states_.push(StateUnknown);
            v = new Value(new Array());
            paths_.push(v);

            if (value_ == 0)
            {
                value_ = v;
            }

            break;

            case ']':
            states_.pop();
            v = paths_.top();
            paths_.pop();

            if (paths_.top()->is_array())
            {
                paths_.top()->get_array()->push_back(v);
            }

            if (paths_.top()->is_object())
            {
                paths_.top()->get_object()->insert(keys_.top(), v);
                keys_.pop();
            }

            break;

            case ',':
            states_.push(StateUnknown);
            break;

            default:
            break;
        }
        break;

        case StateObject:
        switch (c)
        {
            case ' ': case '\t': case '\n':
            break;

            case '{':
            v = new Value(new Object());
            states_.push(StateObjectKey);
            paths_.push(v);

            if (value_ == 0)
            {
                value_ = v;
            }

            break;

            case '}':
            states_.pop();
            v = paths_.top();
            paths_.pop();

            if (!paths_.empty())
            {
                if (paths_.top()->is_array())
                {
                    paths_.top()->get_array()->push_back(v);
                }

                if (paths_.top()->is_object() && !keys_.empty())
                {
                    paths_.top()->get_object()->insert(keys_.top(), v);
                    keys_.pop();
                }
            }

            break;

            case ',':
            states_.push(StateObjectKey);
            break;

            default:
            break;
        }
        break;

        case StateObjectKey:
        switch (c)
        {
            case ' ': case '\t': case '\n':
            break;

            case ':':
            keys_.push(token_);
            token_ = "";
            states_.pop();
            states_.push(StateUnknown);
            break;

            default:
            token_ += c;
            break;
        }
        break;

        default:
        break;
    }

    decode_per_char();
}

}
