#include "negated_range_regex_node.hpp"

#include <random>

using rand_regex::negated_range_regex_node_;

negated_range_regex_node_::negated_range_regex_node_(std::vector<std::pair<char, char>>&& nodes)
  : ranges_(std::forward<std::vector<std::pair<char, char>>> (nodes)) // TODO has to contain inverse ranges
{
  //
}

void negated_range_regex_node_::generate(std::ostream& os) const
{
  srand(time(NULL)); // TODO add option to provide random seed
  const auto& range = ranges_[rand()%ranges_.size()];

  if(range.first == range.second)
  {
    os << range.first;
  }
  else
  {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<char> distribution(range.first, range.second);

    os << distribution(generator);
  }
}
