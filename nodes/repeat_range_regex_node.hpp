#ifndef REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
#define REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED

#include <variant>
#include "regex_node.hpp"

#include "literal_regex_node.hpp"
#include "inner_group_node.hpp"
#include "range_or_node.hpp"
#include "range_random_regex_node.hpp"
#include "captured_group_reference_node.hpp"

namespace rand_regex {

class repeat_range_regex_node_ // {x,y}
{
public:
    using repeat_range_regex_node_variant = std::variant<
            literal_regex_node_,
            inner_group_node_,
            range_or_node_,
            range_random_regex_node_,
            captured_group_reference_node_
          >;

  repeat_range_regex_node_(repeat_range_regex_node_variant&& node, std::tuple<int, int>&& min_max)
    : node_{std::move(node)}, min_{std::get<0>(min_max)}, max_{std::get<1>(min_max)} {}
  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  repeat_range_regex_node_variant node_;
  int min_;
  int max_;
  int random_value_;
};

};

#endif // REPEAT_RANGE_REGEX_NODE_HPP_INCLUDED
