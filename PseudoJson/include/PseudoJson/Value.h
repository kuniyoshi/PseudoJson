#ifndef PSEUDO_JSON__VALUE_H_
#define PSEUDO_JSON__VALUE_H_
#include <iostream>
#include <string>
#include <vector>

namespace PseudoJson
{

class Array;
class Object;

class Value
{
public:
    enum ValueType
    {
        ValueTypeObject,
        ValueTypeArray,
        ValueTypeValue,
    };

private:
    ValueType vt_;
    std::string s_;
    Object* o_;
    Array* a_;

public:
    Value(ValueType vt);
    Value(std::string s);
    Value(Object* o);
    Value(Array* a);
    ~Value();
    void print(std::ostream* os) const;
    void set(std::string s);
    void set(Object* o);
    void set(Array* a);
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;
    std::string get_value() const;
    int get_int_value() const;
    double get_double_value() const;
    Object* get_object() const;
    Array* get_array() const;
    std::string get_at(const std::string& at) const;
    int get_int_at(const std::string& at) const;
    double get_double_at(const std::string& at) const;
    void copy_to_vector_at( std::vector< int >* v,
                            const std::string& at) const;
    void copy_to_vector_at( std::vector< double >* v,
                            const std::string& at) const;
    void copy_to_vector_at( std::vector< std::string >* v,
                            const std::string& at) const;
};

} // PseudoJson

#endif
