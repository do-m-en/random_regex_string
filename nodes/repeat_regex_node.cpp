#include "repeat_regex_node.hpp"

#include <type_traits>

#include "captured_group_reference_node.hpp"
#include "group_regex_node.hpp"
#include "literal_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "or_regex_node.hpp"
#include "random_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "whitespace_regex_node.hpp"

using rand_regex::repeat_regex_node_g;

#include <iostream>
std::size_t repeat_regex_node_g::generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: repeat_regex_node_ " << current_index << '\n';
#endif

  std::size_t len = 0;

  for(std::size_t i=0; i<std::get<repeat_regex_node_d>(nodes[current_index]).repeat_; ++i)
  {
    len = std::visit(
              [&nodes, &current_index, &os, &random_gen](auto&& node)
              {
                return std::remove_reference_t<decltype(node)>::generator::generate(nodes, current_index + 1, os, random_gen);
              },
              nodes[current_index + 1]
            );
  }

  return len + 1;
}

std::size_t repeat_regex_node_g::regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: repeat_regex_node_ " << current_index << '\n';
#endif

  std::size_t len = 0;

  for(std::size_t i=0; i<std::get<repeat_regex_node_d>(nodes[current_index]).repeat_; ++i)
  {
    len = std::visit(
              [&nodes, &current_index, &os](auto&& node)
              {
                return std::remove_reference_t<decltype(node)>::generator::regenerate(nodes, current_index + 1, os);
              },
              nodes[current_index + 1]
            );
  }

  return len + 1;
}

std::size_t repeat_regex_node_g::get_size(const std::vector<regex_variant>& nodes, std::size_t current_index)
{
  return std::visit(
            [&nodes, &current_index](auto&& node)
            {
              return std::remove_reference_t<decltype(node)>::generator::get_size(nodes, current_index + 1);
            },
            nodes[current_index + 1]
          );
}
