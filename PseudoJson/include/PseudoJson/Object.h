#ifndef PSEUDO_JSON__OBJECT_H_
#define PSEUDO_JSON__OBJECT_H_
#include <iostream>
#include <map>
#include <string>

namespace PseudoJson
{

class Value;

class Object
{
private:
    std::map< std::string, Value* > map_;

public:
    Object();
    ~Object();
    void print(std::ostream* os) const;
    void insert(std::string key, Value* value);
    const Value* at(std::string key) const;
};

}

#endif