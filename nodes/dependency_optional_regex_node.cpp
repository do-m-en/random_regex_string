#include "dependency_optional_regex_node.hpp"

using rand_regex::dependency_optional_regex_node_;

dependency_optional_regex_node_::dependency_optional_regex_node_(bool not_matching, regex_node_* node)
  : node_{node}
  , not_matching_{not_matching}
{
  //
}

void dependency_optional_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, char& last_gen)
{
  generate_ = false;

  if((last_gen >= 'A' && last_gen <= 'Z') ||
     (last_gen >= 'a' && last_gen <= 'z') ||
     (last_gen >= '0' && last_gen <= '9') ||
     last_gen == '_')
  {
    if(!not_matching_)
    {
      generate_ = true;
      node_->generate(os, random_gen, last_gen);
    }
  }
  else if(not_matching_)
  {
    generate_ = true;
    node_->generate(os, random_gen, last_gen);
  }

  if(generate_ == false) // if generation is not mandatory still check if it should be generated non the less
  {
    generate_ = random_gen.get_random(0, 1);
    if(generate_)
      node_->generate(os, random_gen, last_gen);
  }
}

void dependency_optional_regex_node_::regenerate(std::ostream& os, char& last_gen) const
{
  if(generate_)
    node_->regenerate(os, last_gen);
}
