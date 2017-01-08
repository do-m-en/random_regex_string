#ifndef OR_REGEX_NODE_HPP_INCLUDED
#define OR_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "data_containers.hpp"
#include "regex_node.hpp"

namespace rand_regex {

struct or_regex_node_g // |
{
  static std::size_t generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen);
  static std::size_t regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os);
  static std::size_t get_size(const std::vector<regex_variant>& nodes, std::size_t current_index) {return std::get<or_regex_node_d>(nodes[current_index]).end_;}
};

};

#endif // OR_REGEX_NODE_HPP_INCLUDED
