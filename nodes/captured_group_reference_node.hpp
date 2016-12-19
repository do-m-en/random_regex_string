#ifndef CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
#define CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class captured_group_reference_node_ : public regex_node_ // capturing (stuff) with \number
{
public:
  captured_group_reference_node_(regex_node_* referred_node);
  void generate(std::ostream& os, random_generator_base& random_gen) override;
  void regenerate(std::ostream& os) const override;
  std::string name() const override {return "captured_group_reference_node_";}

private:
  regex_node_* referred_node_;
};

};

#endif // CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
