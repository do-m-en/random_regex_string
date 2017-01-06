#ifndef CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
#define CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class captured_group_reference_node_ : public regex_node_ // capturing (stuff) with \number
{
public:
  captured_group_reference_node_(std::size_t referred_node);
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "captured_group_reference_node_";}

private:
  std::size_t referred_node_;
};

};

#endif // CAPTURED_GROUP_REFERENCE_NODE_HPP_INCLUDED
