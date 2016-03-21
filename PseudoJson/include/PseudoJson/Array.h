#ifndef PSEUDO_JSON__ARRAY_H_
#define PSEUDO_JSON__ARRAY_H_
#include <iostream>
#include <string>
#include <vector>

namespace PseudoJson
{

class Value;

class Array
{
private:
    std::vector< Value* > array_;

public:
    Array();
    ~Array();
    void print(std::ostream* os) const;
    void push_back(Value* new_value);
    const Value* at(int index) const;
    const Value* at(const std::string& index) const;
    void copy_to_vector(std::vector< std::string >* v) const;
    void copy_to_vector(std::vector< int >* v) const;
    void copy_to_vector(std::vector< double >* v) const;
    bool does_exist(const std::string& index) const;
    size_t size_of() const;
};

} // PseudoJson

#endif
