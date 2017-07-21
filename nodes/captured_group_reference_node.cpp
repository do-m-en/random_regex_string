#include "captured_group_reference_node.hpp"

#include <tuple>
#include <algorithm>

using rand_regex::captured_group_reference_node_;

captured_group_reference_node_::captured_group_reference_node_(int referred_node)
  : referred_group_(referred_node)
{
  //
}

void captured_group_reference_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups)
{
  auto it = std::find_if(groups.begin(), groups.end(), [ref = referred_group_](const std::tuple<int, regex_node_*>& e) {
          return std::get<0>(e) == ref;
      });
  if(it != groups.end())
    std::get<1>(*it)->regenerate(os, groups);
  // else empty result as the referred capture group did not generate anything (a different path was taken)
}

void captured_group_reference_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const
{
  auto it = std::find_if(groups.begin(), groups.end(), [ref = referred_group_](const std::tuple<int, regex_node_*>& e) {
          return std::get<0>(e) == ref;
      });
  if(it != groups.end())
    std::get<1>(*it)->regenerate(os, groups);
  // else empty result as the referred capture group did not generate anything (a different path was taken)
}
