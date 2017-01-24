#ifndef CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
#define CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

#ifdef RANDOM_REGEX_DEBUG_NAME
  #include <string>
#endif

namespace rand_regex {

class captured_group_reference_node_ : public regex_node_ // capturing (stuff) with \number
{
public:
  captured_group_reference_node_(regex_node_* referred_node);
  void generate(std::vector<regex_node_*>& nodes, std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::vector<regex_node_*>& nodes, std::ostream& os) const override;

#ifdef RANDOM_REGEX_DEBUG_NAME
  std::string name() const {return "captured_group_reference_node_";}
#endif

private:
  regex_node_* referred_node_;
};

};

#endif // CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
