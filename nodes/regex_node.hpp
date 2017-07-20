#ifndef REGEX_NODE_INCLUDED
#define REGEX_NODE_INCLUDED

#include <ostream>
#include <random>

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

class regex_node_
{
public:
  virtual ~regex_node_() = default;

  virtual void generate(std::ostream& os, random_generator_base& random_gen, std::vector<std::tuple<int, regex_node_*>>& groups) {} // default for dummy node
  virtual void regenerate(std::ostream& os, const std::vector<std::tuple<int, regex_node_*>>& groups) const {} // default for dummy node
};

};

#endif // REGEX_NODE_INCLUDED
