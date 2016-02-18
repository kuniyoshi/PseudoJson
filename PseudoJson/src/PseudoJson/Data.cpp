#include "PseudoJson/Data.h"
#include <string>
#include <vector>
#include "PseudoJson/Value.h"

namespace PseudoJson
{

Data::Data() : value_(0) {}

Data::Data(const Value* value) : value_(value) {}

Data::Data(const Data& base) : value_(base.value_) {}

Data::~Data() {}

void Data::copy_to_vector_at(   std::vector< int >* v,
                                const std::string& at) const
{
    value_->copy_to_vector_at(v, at);
}

void Data::copy_to_vector_at(   std::vector< double >* v,
                                const std::string& at) const
{
    value_->copy_to_vector_at(v, at);
}

void Data::copy_to_vector_at(   std::vector< std::string >* v,
                                const std::string& at) const
{
    value_->copy_to_vector_at(v, at);
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
