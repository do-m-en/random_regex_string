#ifndef RANDOM_REGEX_STRING_HPP_INCLUDED
#define RANDOM_REGEX_STRING_HPP_INCLUDED

#include <string_view>
#include <vector>
#include <ostream>

#include "data_containers.hpp"

namespace rand_regex {

class regex_template
{
public:
  regex_template(std::string_view regex);
  ~regex_template();
//  std::string generate(std::function<std::string(/*TODO*/)> unicode_to_encoding) const;

  void generate(std::ostream& os); /// TODO add nothrow edition

private:
  std::vector<regex_variant> nodes_;
};


//std::string generate(const regex_template& gen);

}

#endif // RANDOM_REGEX_STRING_HPP_INCLUDED
