#include "inner_group_node.hpp"

#include "group_regex_node.hpp"
#include "or_regex_node.hpp"

#include <algorithm>

using rand_regex::inner_group_node_;

inner_group_node_::inner_group_node_() : node_{new or_regex_node_{}} {}
inner_group_node_::inner_group_node_(inner_group_node_&&) = default;
inner_group_node_::~inner_group_node_() = default;

void inner_group_node_::push_back(group_regex_node_&& item)
{
  node_->push_back(std::move(item));
}

void inner_group_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups)
{
  if(capture_group_no_)
  {
    auto it = std::find_if(groups.begin(), groups.end(), [no = capture_group_no_](const std::tuple<int, inner_group_node_*>& e) {
            return std::get<0>(e) == no;
        });
    if(it == groups.end())
      groups.emplace_back(capture_group_no_, this);
  }

  node_->generate(os, random_gen, groups);
}

void inner_group_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const
{
  node_->regenerate(os, groups);
}
