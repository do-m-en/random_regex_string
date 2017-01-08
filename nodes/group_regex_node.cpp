#include "group_regex_node.hpp"

#include <type_traits>


#include "captured_group_reference_node.hpp"
#include "literal_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "or_regex_node.hpp"
#include "random_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "whitespace_regex_node.hpp"

using rand_regex::group_regex_node_g;

#include <iostream>
std::size_t group_regex_node_g::generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: group_regex_node_ " << current_index << '\n';
#endif

  std::size_t i = current_index + 1;
  for(int x = 0; x < std::get<group_regex_node_d>(nodes[current_index]).end_; ++x)
  {
    i += std::visit(
            [&nodes, &i, &os, &random_gen](auto&& node)
            {
              return std::remove_reference_t<decltype(node)>::generator::generate(nodes, i, os, random_gen);
            },
            nodes[i]
          );
  }

  return i - current_index;
}

std::size_t group_regex_node_g::regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: group_regex_node_ " << current_index << '\n';
#endif

  std::size_t i = current_index + 1;
  for(int x = 0; x < std::get<group_regex_node_d>(nodes[current_index]).end_; ++x)
    i += std::visit(
            [&nodes, &i, &os](auto&& node)
            {
              return std::remove_reference_t<decltype(node)>::generator::regenerate(nodes, i, os);
            },
            nodes[i]
          );

  return i - current_index;
}

std::size_t group_regex_node_g::get_size(const std::vector<regex_variant>& nodes, std::size_t current_index)
{
  std::size_t i = current_index + 1;
  for(int x = 0; x < std::get<group_regex_node_d>(nodes[current_index]).end_; ++x)
    i += std::visit(
            [&nodes, &i](auto&& node)
            {
              return std::remove_reference_t<decltype(node)>::generator::get_size(nodes, i);
            },
            nodes[i]
          );

  return i - current_index;
}
