#ifndef WHITESPACE_REGEX_NODE_HPP_INCLUDED
#define WHITESPACE_REGEX_NODE_HPP_INCLUDED

#include "regex_node.hpp"
#include "data_containers.hpp"

namespace rand_regex {

struct whitespace_regex_node_g // match any whitespace character [\r\n\t\f ]
{
  static std::size_t generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen);
  static std::size_t regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os);
  static std::size_t get_size(const std::vector<regex_variant>& nodes, std::size_t current_index) {return 1;}
};

};

#endif // WHITESPACE_REGEX_NODE_HPP_INCLUDED
