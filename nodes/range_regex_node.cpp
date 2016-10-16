#include "range_regex_node.hpp"

using rand_regex::range_regex_node_;

range_regex_node_::range_regex_node_(std::vector<regex_node_*>&& nodes)
  : range_nodes_(std::forward<std::vector<regex_node_*>>(nodes))
{
  //
}

void range_regex_node_::generate(std::ostream& os) const
{
  srand(time(NULL)); // TODO add option to provide random seed
  range_nodes_[rand()%range_nodes_.size()]->generate(os);
}
