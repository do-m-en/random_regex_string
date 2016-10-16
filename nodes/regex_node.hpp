#ifndef REGEX_NODE_INCLUDED
#define REGEX_NODE_INCLUDED

#include <ostream>

namespace rand_regex {

class regex_node_
{
public:
  virtual void generate(std::ostream& os) const = 0;
};

};

#endif // REGEX_NODE_INCLUDED
