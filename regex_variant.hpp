#ifndef REGEX_VARIANT_HPP_INCLUDED
#define REGEX_VARIANT_HPP_INCLUDED

#include <variant>

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

} // namespace

#endif // REGEX_VARIANT_HPP_INCLUDED
