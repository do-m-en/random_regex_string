#ifndef WHITESPACE_REGEX_NODE_HPP_INCLUDED
#define WHITESPACE_REGEX_NODE_HPP_INCLUDED

#include <vector>
#include "regex_node.hpp"

namespace rand_regex {

class whitespace_regex_node_ : public regex_node_ // match any whitespace character [\r\n\t\f ]
{
public:
  whitespace_regex_node_();
  void generate(std::ostream& os) const override;

private:
  std::vector<char> whitespaces_; // TODO provide whitespaces by reference or a callback (amount, getter)...
};

};

#endif // WHITESPACE_REGEX_NODE_HPP_INCLUDED
