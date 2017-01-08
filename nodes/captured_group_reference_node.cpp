#include "captured_group_reference_node.hpp"

#include <type_traits>

#include "group_regex_node.hpp"
#include "literal_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "or_regex_node.hpp"
#include "random_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "whitespace_regex_node.hpp"

#include <iostream>

using rand_regex::captured_group_reference_node_g;

std::size_t captured_group_reference_node_g::generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: captured_group_reference_node_ " << current_index << '\n';
#endif

  return std::visit(
        [&nodes, &current_index, &os](auto&& node)
        {
          return std::remove_reference_t<decltype(node)>::generator::regenerate(nodes, std::get<captured_group_reference_node_d>(nodes[current_index]).referred_node_, os);
        },
        nodes[std::get<captured_group_reference_node_d>(nodes[current_index]).referred_node_]
      ) + 1;
}

std::size_t captured_group_reference_node_g::regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: captured_group_reference_node_ " << current_index << '\n';
#endif

  return std::visit(
        [&nodes, &current_index, &os](auto&& node)
        {
          return std::remove_reference_t<decltype(node)>::generator::regenerate(nodes, std::get<captured_group_reference_node_d>(nodes[current_index]).referred_node_, os);
        },
        nodes[std::get<captured_group_reference_node_d>(nodes[current_index]).referred_node_]
      ) + 1;
}

std::size_t captured_group_reference_node_g::get_size(const std::vector<regex_variant>& nodes, std::size_t current_index)
{
  return std::visit(
        [&nodes, &current_index](auto&& node)
        {
          return std::remove_reference_t<decltype(node)>::generator::get_size(nodes, std::get<captured_group_reference_node_d>(nodes[current_index]).referred_node_);
        },
        nodes[std::get<captured_group_reference_node_d>(nodes[current_index]).referred_node_]
      ) + 1;
}
