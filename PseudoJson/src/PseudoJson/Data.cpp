#include "PseudoJson/Data.h"
#include <sstream>
#include <string>
#include <vector>
#include "PseudoJson/Value.h"

namespace PseudoJson
{

Data::Data() : value_(0) {}

Data::Data(const Value* value) : value_(value) {}

Data::Data(const Data& base) : value_(base.value_) {}

Data::~Data() {}

std::string Data::get_at(const std::string& at) const
{
    return value_->get_at(at);
}

int Data::get_int_at(const std::string& at) const
{
    return value_->get_int_at(at);
}

double Data::get_double_at(const std::string& at) const
{
    return value_->get_double_at(at);
}

void Data::copy_to_vector_at(   std::vector< int >* v,
                                const std::string& at) const
{
    if (v->capacity() == 0)
    {
        size_t size = value_->size_of(at);
        v->reserve(size);
    }

    value_->copy_to_vector_at(v, at);
}

void Data::copy_to_vector_at(   std::vector< double >* v,
                                const std::string& at) const
{
    if (v->capacity() == 0)
    {
        size_t size = value_->size_of(at);
        v->reserve(size);
    }

    value_->copy_to_vector_at(v, at);
}

void Data::copy_to_vector_at(   std::vector< std::string >* v,
                                const std::string& at) const
{
    if (v->capacity() == 0)
    {
        size_t size = value_->size_of(at);
        v->reserve(size);
    }

    value_->copy_to_vector_at(v, at);
}

namespace
{

size_t get_capacity(std::string at, const Value* value)
{
    size_t size = 1;
    size = size * value->size_of(at);

    at += ".0";
    size = size * value->size_of(at);

    return size;
}

size_t get_capacity2(std::string at, const Value* value)
{
    size_t size = 1;
    size = size * value->size_of(at);

    at += ".0";
    size = size * value->size_of(at);

    at += ".0";
    size = size * value->size_of(at);

    return size;
}

} // namespace -

template
void Data::copy_expanded_to_vector_at(  std::vector< int >*,
                                        const std::string&) const;
template
void Data::copy_expanded_to_vector_at(  std::vector< double >*,
                                        const std::string&) const;
template
void Data::copy_expanded_to_vector_at(  std::vector< std::string >*,
                                        const std::string&) const;

template< class T >
void Data::copy_expanded_to_vector_at(  std::vector< T >* v,
                                        const std::string& at) const
{
    size_t size = get_capacity(at, value_);

    if (v->capacity() == 0)
    {
        v->reserve(size);
    }

    std::ostringstream oss;
    size = value_->size_of(at);

    for (size_t i = 0; i < size; ++i)
    {
        oss.str("");
        oss << at;
        oss << ".";
        oss << i;
        value_->copy_to_vector_at(v, oss.str());
    }
}

template
void Data::copy_2expanded_to_vector_at( std::vector< int >*,
                                        const std::string&) const;
template
void Data::copy_2expanded_to_vector_at( std::vector< double >*,
                                        const std::string&) const;
template
void Data::copy_2expanded_to_vector_at( std::vector< std::string >*,
                                        const std::string&) const;

template< class T >
void Data::copy_2expanded_to_vector_at( std::vector< T >* v,
                                        const std::string& at) const
{
    size_t size = get_capacity2(at, value_);

    if (v->capacity() == 0)
    {
        v->reserve(size);
    }

    std::ostringstream oss;
    size = value_->size_of(at);

    for (size_t i = 0; i < size; ++i)
    {
        oss.str("");
        oss << at;
        oss << ".";
        oss << i;
        std::string inner_base = oss.str();
        size_t inner_size = value_->size_of(inner_base);

        for (size_t j = 0; j < inner_size; ++j)
        {
            oss.str("");
            oss << inner_base;
            oss << ".";
            oss << j;
            value_->copy_to_vector_at(v, oss.str());
        }
    }
}

size_t Data::size_of(const std::string& at) const
{
    return value_->size_of(at);
}

std::vector< std::string > Data::keys_of(const std::string& at) const
{
    return value_->keys_of(at);
}

} // namespace PseudoJson
