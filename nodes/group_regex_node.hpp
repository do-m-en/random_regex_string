#ifndef GROUP_REGEX_NODE_HPP_INCLUDED
#define GROUP_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#include "literal_regex_node.hpp"
#include "range_or_node.hpp"
#include "inner_group_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "captured_group_reference_node.hpp"

#include <variant>

namespace rand_regex {

class group_regex_node_ // factor stuff
{
public:
  using group_regex_node_variant = std::variant<
          literal_regex_node_,
          inner_group_node_,
          range_or_node_,
          repeat_range_regex_node_,
          range_random_regex_node_,
          captured_group_reference_node_
        >;

  void push_back(group_regex_node_variant&& item) {nodes_.push_back(std::move(item));}

  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  std::vector<group_regex_node_variant> nodes_;
};

};

#endif // GROUP_REGEX_NODE_HPP_INCLUDED
