#include "PseudoJson/Decoder.h"
#include <cassert>
#include <fstream>
#include <stack>
#include <string>
#include "PseudoJson/Array.h"
#include "PseudoJson/Data.h"
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
        case StateNumber: r = "Number"; break;
        case StateInNumber: r = "InNumber"; break;
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
{
    if (value_)
    {
        delete value_;
        value_ = 0;
    }
}

Data Decoder::data() const
{
    return Data(value_);
}

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

    while (!ifs_.eof() && !states_.empty())
    {
        decode_per_char();
    }

    assert(paths_.empty());
    ifs_.close();
}

void Decoder::decode_per_char()
{
    Value* v = 0;
    char c = ifs_.get();
    assert(!ifs_.fail());
    char next_char = '\0';

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

            case '0': case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': case '-': case '+':
            ifs_.unget();
            states_.pop();
            states_.push(StateNumber);
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

            case '/':
            ifs_ >> next_char;

            if (next_char == '/')
            {
                states_.push(StateCommentOut);
            }
            else
            {
                ifs_.unget();
            }

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

        case StateNumber:
        switch (c)
        {
            case '0': case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': case '-': case '+':
            ifs_.unget();
            token_ = "";
            states_.pop();
            states_.push(StateInNumber);
            break;

            default:
            break;
        }
        break;

        case StateInNumber:
        switch (c)
        {
            case '0': case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': case '-': case '+':
            case '.':
            token_ += c;
            break;

            default:
            ifs_.unget();
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

            case '/':
            ifs_ >> next_char;

            if (next_char == '/')
            {
                states_.push(StateCommentOut);
            }
            else
            {
                ifs_.unget();
            }

            break;

            default:
            break;
        }
        break;

        case StateObject:
        switch (c)
        {
            case ' ': case '\t': case '\n': case '\r':
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

            case '/':
            ifs_ >> next_char;

            if (next_char == '/')
            {
                states_.push(StateCommentOut);
            }
            else
            {
                ifs_.unget();
            }

            break;

            default:
            break;
        }
        break;

        case StateObjectKey:
        switch (c)
        {
            case ' ': case '\t': case '\n': case '\r':
            break;

            case ':':
            keys_.push(token_);
            token_ = "";
            states_.pop();
            states_.push(StateUnknown);
            break;

            case '/':
            ifs_ >> next_char;

            if (next_char == '/')
            {
                states_.push(StateCommentOut);
            }
            else
            {
                ifs_.unget();
            }

            break;

            case '}':
            ifs_.unget();
            states_.pop();
            break;

            default:
            token_ += c;
            break;
        }
        break;

        case StateCommentOut:
        switch (c)
        {
            case '\n':
            states_.pop();
            break;

            default:
            break;
        }
        break;

        default:
        break;
    }
}

}
