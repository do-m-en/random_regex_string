#include "dependency_regex_node.hpp"

using rand_regex::dependency_regex_node_;

dependency_regex_node_::dependency_regex_node_(regex_node_* node_match, regex_node_* node_no_match)
  : node_match_{node_match}
  , node_no_match_{node_no_match}
{
  //
}

void dependency_regex_node_::generate(std::ostream& os, random_generator_base& random_gen, char& last_gen)
{
  match_ = false;

  if((last_gen >= 'A' && last_gen <= 'Z') ||
     (last_gen >= 'a' && last_gen <= 'z') ||
     (last_gen >= '0' && last_gen <= '9') ||
     last_gen == '_')
  {
    match_ = true;
    node_match_->generate(os, random_gen, last_gen);
  }
  else
  {
    node_no_match_->generate(os, random_gen, last_gen);
  }
}

void dependency_regex_node_::regenerate(std::ostream& os, char& last_gen) const
{
  if(match_)
    node_match_->regenerate(os, last_gen);
  else
    node_no_match_->regenerate(os, last_gen);
}
