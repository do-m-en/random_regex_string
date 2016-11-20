#ifndef RANDOM_REGEX_STRING_HPP_INCLUDED
#define RANDOM_REGEX_STRING_HPP_INCLUDED

#include <experimental/string_view>
#include <vector>
#include <memory>
#include <initializer_list>

#include "regex_variant.hpp"

namespace rand_regex {
  
class regex_template
{
public:
  regex_template(std::experimental::string_view regex);
//  std::string generate(std::function<std::string(/*TODO*/)> unicode_to_encoding) const;

  void generate(std::ostream& os) const; /// TODO add nothrow edition

private:
  std::unique_ptr<regex_node_> root_node_;
  std::vector<regex_node_*> nodes_;
};


//std::string generate(const regex_template& gen);

}

#endif // RANDOM_REGEX_STRING_HPP_INCLUDED
