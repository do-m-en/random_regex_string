#ifndef GROUP_REGEX_NODE_HPP_INCLUDED
#define GROUP_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class group_regex_node_ : public regex_node_ // (stuff)
{
public:
  std::size_t generate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) override;
  std::size_t regenerate(const std::vector<regex_node_*>& nodes, std::size_t current_index, std::ostream& os) const override;
  std::string name() const override {return "group_regex_node_";}
  std::size_t get_size(const std::vector<regex_node_*>& nodes, std::size_t current_index) const {
      std::size_t i = current_index + 1;
      for(int x = 0; x < end_; ++x)
        i += nodes[i]->get_size(nodes, i);

      return i - current_index;
    }

  void append() {++end_;}

private:
  std::size_t end_ = 0; // end of or index
};

};

#endif // GROUP_REGEX_NODE_HPP_INCLUDED
