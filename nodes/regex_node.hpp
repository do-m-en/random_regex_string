#ifndef REGEX_NODE_INCLUDED
#define REGEX_NODE_INCLUDED

#include <random>

#include <vector>
#include <ostream>
#include "data_containers.hpp"

namespace rand_regex {

class random_generator_base
{
public:
  virtual int get_random(int min, int max) = 0;
};

template<typename random_engine = std::mt19937, typename random_distribution = std::uniform_int_distribution<>>
class random_generator : public random_generator_base
{
public:
  random_generator() : engine{[](){std::random_device rd; return rd();}()} {}
  explicit random_generator(int seed) : engine{seed} {}

  int get_random(int min, int max) override
  {
    random_distribution distribution{min, max};
    return distribution(engine);
  }

private:
  random_engine engine; 
};

struct empty_node_g
{
  static std::size_t generate(std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os, random_generator_base& random_gen) {return 1;}
  static std::size_t regenerate(const std::vector<regex_variant>& nodes, std::size_t current_index, std::ostream& os) {return 1;}
  static std::size_t get_size(const std::vector<regex_variant>& nodes, std::size_t current_index) {return 1;}
};

};

#endif // REGEX_NODE_INCLUDED
