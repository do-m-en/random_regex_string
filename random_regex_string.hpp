#ifndef RANDOM_REGEX_STRING_HPP_INCLUDED
#define RANDOM_REGEX_STRING_HPP_INCLUDED

#include <experimental/string_view>
#include <vector>
#include <memory>
#include <initializer_list>

#include "regex_node.hpp"

#include "captured_group_reference_node.hpp"
#include "group_regex_node.hpp"
#include "literal_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "or_regex_node.hpp"
#include "random_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "whitespace_regex_node.hpp"

#include <variant>

namespace rand_regex {
  
using regex_variant = std::variant<
  captured_group_reference_node_,
  group_regex_node_,
  literal_regex_node_,
  optional_regex_node_,
  or_regex_node_,
  range_random_regex_node_,
  random_regex_node_,
  repeat_range_regex_node_,
  repeat_regex_node_,
  whitespace_regex_node_
  >;

class regex_template
{
public:
  regex_template(std::experimental::string_view regex);
//  std::string generate(std::function<std::string(/*TODO*/)> unicode_to_encoding) const;

  void generate(std::ostream& os) const; /// TODO add nothrow edition

private:
  std::unique_ptr<regex_node_> root_node_;
};


//std::string generate(const regex_template& gen);

}

#endif // RANDOM_REGEX_STRING_HPP_INCLUDED
