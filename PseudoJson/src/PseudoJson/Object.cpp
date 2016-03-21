#include "PseudoJson/Object.h"
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "PseudoJson/Value.h"

namespace PseudoJson
{

Object::Object() : map_() {}
Object::~Object()
{
    std::map< const std::string, Value* >::iterator it = map_.begin();

    for (; it != map_.end(); ++it)
    {
        if (it->second != 0)
        {
            delete it->second;
        }
    }
}

void Object::print(std::ostream* os) const
{
    std::map< const std::string, Value* >::const_iterator it = map_.begin();
    int printed_count = 0;

    *os << "{";

    for (; it != map_.end(); ++it)
    {
        if (printed_count != 0)
        {
            *os << ", ";
        }

        *os << it->first;
        *os << ": ";
        it->second->print(os);
        ++printed_count;
    }

    *os << "}";
}

void Object::insert(std::string key, Value* value)
{
    map_.insert(std::pair< const std::string, Value* >(key, value));
}

const Value* Object::at(const std::string& key) const { return map_.at(key); }

std::vector< std::string > Object::keys() const
{
    std::vector< std::string > keys;

    std::map< const std::string, Value* >::const_iterator it = map_.begin();

    for (; it != map_.end(); ++it)
    {
        keys.push_back(it->first);
    }

    return keys;
}

}
