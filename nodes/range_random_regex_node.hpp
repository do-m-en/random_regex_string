#ifndef RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED
#define RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include <tuple>
#include "regex_node.hpp"

namespace rand_regex {

class inner_group_node_;

class range_random_regex_node_ // x-y that is used in range_regex_node_
{
public:
  range_random_regex_node_() {}
  range_random_regex_node_(std::tuple<char, char>&& from_to) : from_to_{std::move(from_to)} {}
  range_random_regex_node_(char from, char to) : from_to_{from, to} {}
  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  std::tuple<char, char> from_to_;
  char generated_value_;
};

};

#endif // RANGE_RANDOM_REGEX_NODE_HPP_INCLUDED
