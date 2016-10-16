#include "optional_regex_node.hpp"

using rand_regex::optional_regex_node_;

optional_regex_node_::optional_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void optional_regex_node_::generate(std::ostream& os) const
{
  srand(time(NULL)); // TODO add option to provide random seed
  if(rand()%2)
  {
    node_->generate(os);
  }
}
