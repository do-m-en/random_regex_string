#ifndef NEGATED_RANGE_REGEX_NODE_HPP_INCLUDED
#define NEGATED_RANGE_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class negated_range_regex_node_ : public regex_node_ // [^]
{
public:
  negated_range_regex_node_(std::vector<std::pair<char, char>>&& nodes);
  void generate(std::ostream& os) const override;

private:
  std::vector<std::pair<char, char>> ranges_;
};

};

#endif // NEGATED_RANGE_REGEX_NODE_HPP_INCLUDED
