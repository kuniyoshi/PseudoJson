#include "PseudoJson/Array.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include "PseudoJson/Value.h"

namespace PseudoJson
{

Array::Array() : array_() {}

Array::~Array()
{
    std::vector< Value* >::iterator it = array_.begin();

    for (; it != array_.end(); ++it)
    {
        if (*it != 0)
        {
            delete *it;
            *it = 0;
        }
    }
}

void Array::print(std::ostream* os) const
{
    std::vector< Value* >::const_iterator it = array_.begin();
    int printed_count = 0;

    *os << "[";

    for (; it != array_.end(); ++it)
    {
        if (printed_count != 0)
        {
            *os << ", ";
        }

        (*it)->print(os);
        ++printed_count;
    }

    *os << "]";
}

void Array::push_back(Value* v) { array_.push_back(v); }

const Value* Array::at(int index) const { return array_.at(index); }

const Value* Array::at(const std::string& index) const
{
    std::istringstream iss(index);
    int i = 0;
    iss >> i;
    assert(!iss.fail());
    return at(i);
}

void Array::copy_to_vector(std::vector< std::string >* v) const
{
    std::vector< Value* >::const_iterator it = array_.begin();

    for (; it != array_.end(); ++it)
    {
        v->push_back((*it)->get_value());
    }
}

void Array::copy_to_vector(std::vector< int >* v) const
{
    std::vector< Value* >::const_iterator it = array_.begin();

    for (; it != array_.end(); ++it)
    {
        v->push_back((*it)->get_int_value());
    }
}

void Array::copy_to_vector(std::vector< double >* v) const
{
    std::vector< Value* >::const_iterator it = array_.begin();

    for (; it != array_.end(); ++it)
    {
        v->push_back((*it)->get_double_value());
    }
}

bool Array::does_exist(const std::string& index) const
{
    std::istringstream iss(index);
    int i = 0;
    iss >> i;
    return !iss.fail();
}

size_t Array::size_of() const { return array_.size(); }

}
