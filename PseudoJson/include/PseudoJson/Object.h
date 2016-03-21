#ifndef PSEUDO_JSON__OBJECT_H_
#define PSEUDO_JSON__OBJECT_H_
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace PseudoJson
{

class Value;

class Object
{
private:
    std::map< const std::string, Value* > map_;

public:
    Object();
    ~Object();
    void print(std::ostream* os) const;
    void insert(std::string key, Value* value);
    const Value* at(const std::string& key) const;
    std::vector< std::string > keys() const;
};

}

#endif
