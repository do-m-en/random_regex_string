#ifndef RANGE_OR_NODE_HPP_INCLUDED
#define RANGE_OR_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#include "literal_regex_node.hpp"
#include "range_random_regex_node.hpp"

#include <variant>

namespace rand_regex {

class range_or_node_ // [...]
{
public:
  using range_or_node_variant = std::variant<
          literal_regex_node_,
          range_random_regex_node_
        >;

  void push_back(range_or_node_variant&& item) {nodes_.push_back(std::move(item));}

  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  std::vector<range_or_node_variant> nodes_;
  int random_value_;
};

};

#endif // RANGE_OR_NODE_HPP_INCLUDED
