#ifndef PSEUDO_JSON__DATA_H_
#define PSEUDO_JSON__DATA_H_
#include <string>
#include <vector>

namespace PseudoJson
{

class Value;

class Data
{
private:
    const Value* value_;

public:
    Data();
    Data(const Value* value);
    Data(const Data& base);
    ~Data();
    std::string get_at(const std::string& at) const;
    int get_int_at(const std::string& at) const;
    double get_double_at(const std::string& at) const;
    void copy_to_vector_at( std::vector< int >* v,
                            const std::string& at) const;
    void copy_to_vector_at( std::vector< double >* v,
                            const std::string& at) const;
    void copy_to_vector_at( std::vector< std::string >* v,
                            const std::string& at) const;
    template< class T >
    void copy_expanded_to_vector_at(    std::vector< T >* v,
                                        const std::string& at) const;
    template< class T >
    void copy_2expanded_to_vector_at(   std::vector< T >* v,
                                        const std::string& at) const;
    size_t size_of(const std::string& at) const;
    std::vector< std::string > keys_of(const std::string& at) const;
};

} // namespace PseudoJson

#endif
