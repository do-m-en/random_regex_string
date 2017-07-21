#include "inner_group_node.hpp"

//#include "group_regex_node.hpp"
//#include "or_regex_node.hpp"

#include <algorithm>

using rand_regex::inner_group_node_;

inner_group_node_::inner_group_node_(regex_node_* node, int capture_group_no)
  : node_{node}
  , capture_group_no_{capture_group_no}
{
  //
}

void inner_group_node_::generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups)
{
  if(capture_group_no_)
  {
    auto it = std::find_if(groups.begin(), groups.end(), [no = capture_group_no_](const std::tuple<int, regex_node_*>& e) {
            return std::get<0>(e) == no;
        });
    if(it == groups.end())
      groups.emplace_back(capture_group_no_, this);
  }

  node_->generate(os, random_gen, groups);
}

void inner_group_node_::regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const
{
  node_->regenerate(os, groups);
}
