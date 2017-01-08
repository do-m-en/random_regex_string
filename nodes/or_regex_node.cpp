#include "or_regex_node.hpp"

#include <type_traits>

#include "captured_group_reference_node.hpp"
#include "group_regex_node.hpp"
#include "literal_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "random_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "whitespace_regex_node.hpp"

using rand_regex::or_regex_node_g;

#include <iostream>
std::size_t or_regex_node_g::generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "G: or_regex_node_ " << current_index << '\n';
#endif

  if(std::get<or_regex_node_d>(nodes[current_index]).elements_count_ == 1)
  {
    std::get<or_regex_node_d>(nodes[current_index]).random_value_ = 0;
  }
  else
  {
    std::get<or_regex_node_d>(nodes[current_index]).random_value_ = random_gen.get_random(0, std::get<or_regex_node_d>(nodes[current_index]).elements_count_ - 1);
  }

  std::size_t next_index = current_index + 1;
  for(std::size_t i = 0; i < std::get<or_regex_node_d>(nodes[current_index]).random_value_; ++i)
  {
    next_index += std::visit(
                      [&nodes, &next_index](auto&& node)
                      {
                        return std::remove_reference_t<decltype(node)>::generator::get_size(nodes, next_index);
                      },
                      nodes[next_index]
                    );
  }

  std::visit(
      [&nodes, &next_index, &os, &random_gen](auto&& node)
      {
        return std::remove_reference_t<decltype(node)>::generator::generate(nodes, next_index, os, random_gen);
      },
      nodes[next_index]
    );

  return std::get<or_regex_node_d>(nodes[current_index]).end_;
}

std::size_t or_regex_node_g::regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os)
{
#ifdef RANDOM_REGEX_DEBUG
  std::cout << "R: or_regex_node_ " << current_index << '\n';
#endif

  std::size_t next_index = current_index + 1;
  for(std::size_t i = 0; i < std::get<or_regex_node_d>(nodes[current_index]).random_value_; ++i)
  {
    next_index += std::visit(
                      [&nodes, &next_index](auto&& node)
                      {
                        return std::remove_reference_t<decltype(node)>::generator::get_size(nodes, next_index);
                      },
                      nodes[next_index]
                    );
  }

  std::visit(
      [&nodes, &next_index, &os](auto&& node)
      {
        return std::remove_reference_t<decltype(node)>::generator::regenerate(nodes, next_index, os);
      },
      nodes[next_index]
    );

  return std::get<or_regex_node_d>(nodes[current_index]).end_;
}
