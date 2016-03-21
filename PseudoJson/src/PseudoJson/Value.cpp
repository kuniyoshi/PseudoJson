#include "PseudoJson/Value.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "PseudoJson/Array.h"
#include "PseudoJson/Object.h"

namespace PseudoJson
{

Value::Value(ValueType vt)
: vt_(vt), s_(""), o_(0), a_(0)
{
    if (vt_ == ValueTypeObject)
    {
        o_ = new Object();
    }

    if (vt_ == ValueTypeArray)
    {
        a_ = new Array();
    }
}

Value::Value(std::string s)
: vt_(ValueTypeValue), s_(s), o_(0), a_(0)
{}

Value::Value(Object* o)
: vt_(ValueTypeObject), s_(""), o_(o), a_(0)
{}

Value::Value(Array* a)
: vt_(ValueTypeArray), s_(""), o_(0), a_(a)
{}

Value::~Value()
{
    if (vt_ == ValueTypeValue)
    {
        assert(o_ == 0);
        assert(a_ == 0);
        return;
    }

    if (vt_ == ValueTypeObject)
    {
        assert(a_ == 0);

        if (o_ != 0)
        {
            delete o_;
            o_ = 0;
        }

        return;
    }

    if (vt_ == ValueTypeArray)
    {
        assert(o_ == 0);

        if (a_ != 0)
        {
            delete a_;
            a_ = 0;
        }

        return;
    }
}

void Value::print(std::ostream* os) const
{
    if (vt_ == ValueTypeValue)
    {
        *os << '"' << s_ << '"';
        return;
    }

    if (vt_ == ValueTypeObject)
    {
        o_->print(os);
        return;
    }

    if (vt_ == ValueTypeArray)
    {
        a_->print(os);
        return;
    }
}

void Value::set(std::string s) { s_ = s; }
void Value::set(Object* o) { o_ = o; }
void Value::set(Array* a) { a_ = a; }

bool Value::is_string() const { return vt_ == ValueTypeValue; }
bool Value::is_object() const { return vt_ == ValueTypeObject; }
bool Value::is_array() const { return vt_ == ValueTypeArray; }

std::string Value::get_value() const { return s_; }

int Value::get_int_value() const
{
    int v = 0;
    std::istringstream iss(s_);
    iss >> v;
    return v;
}

double Value::get_double_value() const
{
    double v = 0.0;
    std::istringstream iss(s_);
    iss >> v;
    return v;
}

Object* Value::get_object() const { return o_; }

Array* Value::get_array() const { return a_; }

namespace
{

void copy_tokens(std::vector< std::string >* v, std::string s)
{
    const std::string d = ".";
    size_t from = 0;
    size_t to = 0;

    while (from < s.size())
    {
        to = s.find_first_of(d, from);

        if (to >= s.size())
        {
            to = s.size();
        }

        v->push_back(s.substr(from, to - from));

        from = to + 1;
    }
}

bool test_existence(const Value* v, std::vector< std::string > tokens)
{
    const Value* r = v;
    std::vector< std::string >::const_iterator it = tokens.begin();

    assert(tokens.size() > 1);

    for (; it != tokens.end() - 1; ++it)
    {
        if (r->is_object())
        {
            r = r->get_object()->at(*it);
        }
        else if (r->is_array())
        {
            r = r->get_array()->at(*it);
        }
        else
        {
            return false;
        }
    }

    if (r->is_string())
    {
        return true;
    }

    if (r->is_object())
    {
        return r->get_object()->does_exist(*it);
    }
    else if (r->is_array())
    {
        return r->get_array()->does_exist(*it);
    }

    assert(false); // all patterns should be matched above

    return false;
}

const Value* drill_down(const Value* v, std::vector< std::string > tokens)
{
    const Value* r = v;
    std::vector< std::string >::const_iterator it = tokens.begin();

    for (; it != tokens.end(); ++it)
    {
        if (r->is_object())
        {
            r = r->get_object()->at(*it);
        }
        else if (r->is_array())
        {
            r = r->get_array()->at(*it);
        }
    }

    return r;
}

} // namespace -

std::string Value::get_at(const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    return drill_down(this, tokens)->get_value();
}

int Value::get_int_at(const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    return drill_down(this, tokens)->get_int_value();
}

double Value::get_double_at(const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    return drill_down(this, tokens)->get_double_value();
}

void Value::copy_to_vector_at(  std::vector< int >* v,
                                const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    drill_down(this, tokens)->get_array()->copy_to_vector(v);
}

void Value::copy_to_vector_at(  std::vector< double >* v,
                                const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    drill_down(this, tokens)->get_array()->copy_to_vector(v);
}

void Value::copy_to_vector_at(  std::vector< std::string >* v,
                                const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    drill_down(this, tokens)->get_array()->copy_to_vector(v);
}

size_t Value::size_of(const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    return drill_down(this, tokens)->get_array()->size_of();
}

std::vector< std::string > Value::keys_of(const std::string& at) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, at);
    return drill_down(this, tokens)->get_object()->keys();
}

bool Value::does_exist(const std::string& what) const
{
    std::vector< std::string > tokens;
    copy_tokens(&tokens, what);
    return test_existence(this, tokens);
}

}
