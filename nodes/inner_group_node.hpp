#ifndef INNER_GROUP_NODE_HPP_INCLUDED
#define INNER_GROUP_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#include <memory>
#include <map>

namespace rand_regex {

class or_regex_node_;
class group_regex_node_;
class repeat_range_regex_node_;

class inner_group_node_ // (stuff)
{
public:
  inner_group_node_();
  inner_group_node_(inner_group_node_&&);
  ~inner_group_node_();

  void push_back(group_regex_node_&& item);
  void set_capture_group(int capture_group_no) {capture_group_no_ = capture_group_no;}

  void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, inner_group_node_*>>& groups);
  void regenerate(std::ostream& os, const std::vector<std::tuple<int, inner_group_node_*>>& groups) const;

private:
  std::unique_ptr<or_regex_node_> node_;
  int capture_group_no_ = 0;
};

};

#endif // INNER_GROUP_NODE_HPP_INCLUDED
