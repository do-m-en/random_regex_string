#include "random_regex_string.hpp"

#include <ctime>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <random>
#include <algorithm>

#include "group_regex_node.hpp"
#include "or_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "random_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "whitespace_regex_node.hpp"
#include "literal_regex_node.hpp"

using rand_regex::regex_node_;
using rand_regex::group_regex_node_;
using rand_regex::or_regex_node_;
using rand_regex::range_random_regex_node_;
using rand_regex::random_regex_node_;
using rand_regex::optional_regex_node_;
using rand_regex::repeat_regex_node_;
using rand_regex::repeat_range_regex_node_;
using rand_regex::whitespace_regex_node_;
using rand_regex::literal_regex_node_;


// Scott Schurr's str_const (constexpr string)
// see: https://github.com/boostcon/cppnow_presentations_2012/blob/master/wed/schurr_cpp11_tools_for_class_authors.pdf?raw=true
/*class str_const
{
public:
  template<std::size_t N>
  constexpr str_const(const char(&a)[N])
    : p_(a)
    , sz_(N-1)
  {
    //
  }

  constexpr char operator[](std::size_t n)
  {
    return n < sz_ ? p_[n] : throw std::out_of_range("");
  }

  constexpr std::size_t size() { return sz_; }
private:
  const char* const p_;
  const std::size_t sz_;
};*/

// class regex_template
using rand_regex::regex_template;

struct parser
{
public:
  parser(char start_token, std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> consumer)
    : start_tokens_{{start_token}}
    , consumer_{consumer}
  {
    //
  }

  parser(std::vector<char> start_tokens, std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> consumer)
    : start_tokens_{std::forward<std::vector<char>>(start_tokens)}
    , consumer_{consumer}
  {
    //
  }

  // TODO add size checked and unchecked version as other parser types can handle check at the top level for all (or/and parsers)
  //      for now failing size checks forces or/and parsers to continue untill all parsers are used up
//  template<bool truncate=true>
  regex_node_* operator()(const std::string& regex, std::size_t& consumed) const
  {
    regex_node_* node = nullptr;

    if(regex.size() > consumed)
    {
      for(auto start_token : start_tokens_)
      {
        if(regex[consumed] == start_token)
        {
          ++consumed;
          node = consumer_(regex, consumed);

          if(!node) {
            --consumed;
            throw 1; // TODO throw if start token was found but consumption did not take place
          }

          break;
        }
      }
    }

    return node;
  }

private:
  std::vector<char> start_tokens_;
  std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> consumer_;
};

struct and_parser
{
public:
  and_parser(std::initializer_list<parser> and_parsers, char terminator,
        std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> else_parser = nullptr)
    : parsers_{and_parsers}, terminator_{terminator}, else_parser_{else_parser} {}

  // TODO this should be optional return value
  std::vector<regex_node_*> operator()(const std::string& regex, std::size_t& consumed) const
  {
    std::vector<regex_node_*> nodes;

    while(regex.size() > consumed && regex[consumed] != terminator_)
    {
      bool found = false;
      for(auto& p : parsers_)
      {
        //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
        auto node = p(regex, consumed);
        if(node)
        {
          nodes.push_back(node);
          found = true;
          break;
        }
      }

      if(!found)
      {
        if(else_parser_)
          nodes.push_back(else_parser_(regex, consumed)); // TODO check if null is returned...
        else
        {
          //throw 1; // TODO throw no match found exception
          for(auto node : nodes)
            delete node;

          nodes.clear();

          throw 1;
        }
      }
    }

    return nodes;
  }

private:
  std::vector<parser> parsers_;
  std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> else_parser_;
  char terminator_;
};

struct or_parser
{
public:
  or_parser(std::initializer_list<parser> or_parsers,
      std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> else_parser = nullptr
    ) : or_parsers_{or_parsers}, else_parser_{else_parser} {}

  regex_node_* operator()(const std::string& regex, std::size_t& consumed) const
  {
    for(auto& p : or_parsers_)
    {
      //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
      auto node = p(regex, consumed);
      if(node)
      {
        return node;
      }
    }

    if(else_parser_)
      return else_parser_(regex, consumed);

    //throw 1; // TODO throw no match found exception
    return nullptr;
  }
private:
  std::vector<parser> or_parsers_;
  std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> else_parser_;
};

regex_node_* parser_term(const std::string& regex, std::size_t& consumed);
regex_node_* parser_factor(const std::string& regex, std::size_t& consumed);
regex_node_* parser_base(const std::string& regex, std::size_t& consumed);

// <regex> ::= <term> '|' <regex> | <term>
regex_node_* parser_regex(const std::string& regex, std::size_t& consumed)
{
  regex_node_* node = parser_term(regex, consumed);

  if(consumed < regex.size() && regex[consumed] == '|')
  {
    ++consumed; // consume |
    regex_node_* other = parser_regex(regex, consumed);
    node = new or_regex_node_{node, other};
  }

  return node;
}

// <term> ::= { <factor> }
regex_node_* parser_term(const std::string& regex, std::size_t& consumed)
{
  regex_node_* node = parser_factor(regex, consumed);

  while(consumed < regex.size() && regex[consumed] != ')'
    && regex[consumed] != '|')
  {
    regex_node_* next = parser_factor(regex, consumed);
    node = new group_regex_node_(std::vector<regex_node_*>{node, next}); // TODO consider renaming it to sequence
  }

  return node;
}

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
regex_node_* parser_factor(const std::string& regex, std::size_t& consumed)
{
  regex_node_* node = parser_base(regex, consumed);

  auto truncate = [](auto& regex, auto& consumed, char item){
          while(consumed < regex.size() && regex[consumed] == item)
          {
            ++consumed; // consume *
          }
        };

  if(consumed < regex.size() && regex[consumed] == '*')
  {
    node = new repeat_range_regex_node_(node, 0);

    truncate(regex, consumed, '*');
  }

  if(consumed < regex.size() && regex[consumed] == '+')
  {
    node = new repeat_range_regex_node_(node, 1);

    // ++ is posessive but for the sake of generation it doesn't make any
    // difference as it may always match one or more times...
    truncate(regex, consumed, '+');
  }

  if(consumed < regex.size() && regex[consumed] == '?')
  {
    node = new optional_regex_node_(node);

    // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
    // so for generation sake it doesn't make any difference as it may always
    // match 0 or 1 times
    truncate(regex, consumed, '?');
  }

  if(consumed < regex.size() && regex[consumed] == '{')
  {
    ++consumed; // consume {
    std::size_t begin = consumed;
    while(regex.size() > consumed && isdigit(regex[consumed]))
    {
      ++consumed;
    }

    if(consumed < regex.size() && regex[consumed] == ',')
    {
      std::size_t min = 0;

      if(begin != consumed)
      {
        std::string number(regex.substr(begin, consumed-begin));
        if(std::all_of(number.begin(), number.end(), ::isdigit))
        {
          min = std::stoi(number);
        }
        else
        {
          /// TODO handle exception
          throw 1;
        }
      }
      else
      {
        /// TODO handle exception
        throw 1; // syntax x{,y} is not supported
      }

      begin = ++consumed;
      while(regex.size() > consumed && isdigit(regex[consumed]))
      {
        ++consumed;
      }

      if(regex.size() > consumed && regex[consumed] == '}')
      {
        if(begin != consumed)
        {
          std::string number(regex.substr(begin, consumed-begin));
          if(std::all_of(number.begin(), number.end(), ::isdigit))
          {
            node = new repeat_range_regex_node_(node, min, std::stoi(number));
          }
          else
          {
            /// TODO handle exception
            throw 1;
          }
        }
        else
        {
          node = new repeat_range_regex_node_(node, min);
        }
      }
      else
      {
        /// TODO error handling
        throw 1;
      }
    }
    else if(begin != consumed && regex.size() > consumed && regex[consumed] == '}')
    {
      std::string number(regex.substr(begin, consumed - begin));
      if(std::all_of(number.begin(), number.end(), ::isdigit))
      {
        node = new repeat_regex_node_(node, std::stoi(number));
      }
      else
      {
        /// TODO handle exception
        throw 1;
      }
    }
    else
    {
      /// TODO error handling
      throw 1;
    }
    ++consumed; // consume }
  }

  return node;
}

/*
regex_node_* parser_factor(const std::string& regex, std::size_t& consumed)
{
  regex_node_* node = parser_base(regex, consumed);

  auto repeat_range_zero = parser{'*', [truncate, node](auto& regex, auto& consumed){
          truncate(regex, consumed, '*');
          return new repeat_range_regex_node_(node, 0);
        }};
  auto repeat_range_one = parser{'+', [truncate, node](auto& regex, auto& consumed){
          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          truncate(regex, consumed, '+');
          return new repeat_range_regex_node_(node, 1);
        }};
  auto optional_item = parser{'?', [truncate, node](auto& regex, auto& consumed){
          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          truncate(regex, consumed, '?');
          return new optional_regex_node_(node);
        }};

  auto digit_parser = [](auto& regex, auto& consumed){
            while(regex.size() > consumed && isdigit(regex[consumed]))
            {
              ++consumed;
            }
        };

  or_parser parser_factor__{
      {repeat_range_zero,
      repeat_range_one,
      optional_item},
      [&node, digit_parser](auto& regex, auto& consumed) {
          if(consumed < regex.size() && regex[consumed] == '{')
          {
            ++consumed; // consume {
            std::size_t begin = consumed;
            digit_parser(regex, consumed);

            if(consumed < regex.size() && regex[consumed] == ',')
            {
              std::size_t min = 0;

              if(begin != consumed)
              {
                std::string number(regex.substr(begin, consumed-begin));
                if(std::all_of(number.begin(), number.end(), ::isdigit))
                {
                  min = std::stoi(number);
                }
                else
                {
                  /// TODO handle exception
                  throw 1;
                }
              }
              else
              {
                /// TODO handle exception
                throw 1; // syntax x{,y} is not supported
              }

              begin = ++consumed;
              while(regex.size() > consumed && isdigit(regex[consumed]))
              {
                ++consumed;
              }

              if(regex.size() > consumed && regex[consumed] == '}')
              {
                if(begin != consumed)
                {
                  std::string number(regex.substr(begin, consumed-begin));
                  if(std::all_of(number.begin(), number.end(), ::isdigit))
                  {
                    node = new repeat_range_regex_node_(node, min, std::stoi(number));
                  }
                  else
                  {
                    /// TODO handle exception
                    throw 1;
                  }
                }
                else
                {
                  node = new repeat_range_regex_node_(node, min);
                }
              }
              else
              {
                /// TODO error handling
                throw 1;
              }
            }
            else if(begin != consumed && regex.size() > consumed && regex[consumed] == '}')
            {
              std::string number(regex.substr(begin, consumed - begin));
              if(std::all_of(number.begin(), number.end(), ::isdigit))
              {
                node = new repeat_regex_node_(node, std::stoi(number));
              }
              else
              {
                /// TODO handle exception
                throw 1;
              }
            }
            else
            {
              /// TODO error handling
              throw 1;
            }
            ++consumed; // consume }
          }

          return node;
        }
    };

  return parser_factor__(regex, consumed);
}
*/

#include <limits> // TODO move to the top
// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
regex_node_* parser_base(const std::string& regex, std::size_t& consumed)
{
  auto form_feed = parser{'f', [](auto& regex, auto& consumed){return new literal_regex_node_{'\f'};}};
  auto new_line = parser{'n', [](auto& regex, auto& consumed){return new literal_regex_node_{'\n'};}};
  auto carriage_return = parser{'r', [](auto& regex, auto& consumed){return new literal_regex_node_{'\r'};}};
  auto horizontal_tab = parser{'t', [](auto& regex, auto& consumed){return new literal_regex_node_{'\t'};}};
  auto vertical_tab = parser{'v', [](auto& regex, auto& consumed){return new literal_regex_node_{'\v'};}};
  auto escaped_literal_char = parser{{'\\', '|', '.', '-', '^', '?', '*', '+', '{', '}', '(', ')', '[', ']'},
        [](auto& regex, auto& consumed){return new literal_regex_node_(regex[consumed-1]);}};
  auto any_digit = parser{'d', [](auto& regex, auto& consumed){return new range_random_regex_node_{'0', '9'};}};
  auto any_non_digit = parser{'D', [](auto& regex, auto& consumed){
          return new or_regex_node_{new range_random_regex_node_{std::numeric_limits<char>::min(), '0' - 1},
                                    new range_random_regex_node_{'9' + 1, std::numeric_limits<char>::max()}};
        }};
  auto any_whitespace = parser{'s', [](auto& regex, auto& consumed){return new whitespace_regex_node_{};}};
  auto any_non_whitespace = parser{'S', [](auto& regex, auto& consumed){
       /* '\t' // tab: 9
          '\n' // newline: 10
          '\v' // vertical tab: 11
          '\f' // formfeed: 12
          '\r' // carriage return: 13
          ' ' // space: 32 */
          return new or_regex_node_{new range_random_regex_node_{std::numeric_limits<char>::min(), '\t' - 1},
                                    new range_random_regex_node_{'\r' + 1, ' ' - 1},
                                    new range_random_regex_node_{' ' + 1, std::numeric_limits<char>::max()}};
        }};
  auto any_alphanum_or_underscore = parser{'w', [](auto& regex, auto& consumed) // any alphanumeric characters or _
        {
          return new or_regex_node_{new range_random_regex_node_{'a', 'z'},
                                    new range_random_regex_node_{'0', '9'},
                                    new literal_regex_node_{'_'}};
        }};
  auto any_not_alphanum_or_underscore = parser{'W', [](auto& regex, auto& consumed){
          return new or_regex_node_{new range_random_regex_node_{std::numeric_limits<char>::min(), '0' - 1},
                                    new range_random_regex_node_{'9' + 1, 'A' - 1},
                                    new range_random_regex_node_{'Z' + 1, '_' - 1},
                                    new range_random_regex_node_{'_' + 1, 'a' - 1},
                                    new range_random_regex_node_{'z' + 1, std::numeric_limits<char>::max()}};
        }};

  or_parser parser_escaped {
      form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      escaped_literal_char,
      any_digit,
      any_non_digit,
      any_whitespace,
      any_non_whitespace,
      any_alphanum_or_underscore,
      any_not_alphanum_or_underscore
    };

  or_parser escaped_or_literal {
      {{'\\', [parser_escaped](auto& regex, auto& consumed){return parser_escaped(regex, consumed);}}},
      [](auto& regex, auto& consumed){return new literal_regex_node_(regex[consumed++]);}
    };

  or_parser parse_end_range_escaped {
      form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      escaped_literal_char
    };

  or_parser end_range_escaped_or_literal {
      {{'\\', [parse_end_range_escaped](auto& regex, auto& consumed){return parse_end_range_escaped(regex, consumed);}}},
      [](auto& regex, auto& consumed){return new literal_regex_node_(regex[consumed++]);}
    };

  and_parser range_parser {
      {},
      ']',
      [escaped_or_literal, end_range_escaped_or_literal](auto& regex, auto& consumed)
      {
        auto tmp_node = escaped_or_literal(regex, consumed);
        if(!tmp_node)
        {
          /// TODO exception handling
          throw 1;
        }

        if(consumed+2 < regex.size() && regex[consumed] == '-' && regex[consumed+1] != ']')
        {
          literal_regex_node_* literal_node = static_cast<literal_regex_node_*>(tmp_node);

          ++consumed;

          if(consumed+1 >= regex.size())
          {
            /// TODO exception handling
            delete tmp_node; // TODO RAII
            throw 1;
          }

          auto tmp = end_range_escaped_or_literal(regex, consumed);
          if(!tmp)
          {
            /// TODO exception handling
            delete tmp_node; // TODO RAII
            throw 1;
          }

          tmp_node = new range_random_regex_node_(literal_node->getLiteral(), static_cast<literal_regex_node_*>(tmp)->getLiteral()); // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_
          delete tmp;
          delete literal_node;
        }

        return tmp_node;
      }
    };

  or_parser range_or_negated_range {
      {{'^', [range_parser](auto& regex, auto& consumed){
          auto tmp = range_parser(regex, consumed);
          // TODO check if range parser returns an empty vector

          std::vector<regex_node_*> invert;
          std::vector<std::pair<char, char>> ranges;

          // convert
          for(auto element : tmp)
          {
            if(auto lit = dynamic_cast<literal_regex_node_*>(element))
            {
              ranges.emplace_back(lit->getLiteral(), lit->getLiteral());
            }
            else if(auto range = dynamic_cast<range_random_regex_node_*>(element))
            {
              ranges.emplace_back(range->get_from(), range->get_to());
            }

            delete element;
          }

          // handle single range case
          if(tmp.size() == 1)
          {
            // TODO merge
            if(ranges[0].first != std::numeric_limits<char>::min() && ranges[0].second != std::numeric_limits<char>::max())
            {
              invert.push_back(new range_random_regex_node_(std::numeric_limits<char>::min(), ranges[0].first - 1));
              invert.push_back(new range_random_regex_node_(ranges[0].second + 1, std::numeric_limits<char>::max()));
            }
            else if(ranges[0].first == std::numeric_limits<char>::min() && ranges[0].second == std::numeric_limits<char>::max())
            {
              // in rare case where no characters are allowed throw an exception as regex is faulty...
              throw 1; // TODO exception
            }
            else if(ranges[0].first == std::numeric_limits<char>::min())
            {
              invert.push_back(new range_random_regex_node_(ranges[0].second + 1, std::numeric_limits<char>::max()));
            }
            else
            {
              invert.push_back(new range_random_regex_node_(std::numeric_limits<char>::min(), ranges[0].first - 1));
            }
          }
          else
          {
            std::sort(ranges.begin(), ranges.end(), [](auto& i, auto& j){return (i.first<j.first);});

            // merge and invert ranges
            auto min = std::numeric_limits<char>::min();
            auto cur_it = ranges.begin();
            for(auto it = ranges.begin() + 1; it != ranges.end(); ++it)
            {
              auto& cur = *cur_it;
              auto& next = *it;

              if(cur.second >= next.second) // because of the sort we know that first is either same or smaller so a sub range
              {
                if(it+1 == ranges.end()) // TODO merge with lower
                {
                  if(cur.first != std::numeric_limits<char>::min() && cur.second != std::numeric_limits<char>::max())
                  {
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                    invert.push_back(new range_random_regex_node_(cur.second + 1, std::numeric_limits<char>::max()));
                  }
                  else if(cur.first == std::numeric_limits<char>::min() && cur.second == std::numeric_limits<char>::max())
                  {
                    // in rare case where no characters are allowed throw an exception as regex is faulty...
                    throw 1; // TODO exception
                  }
                  else if(cur.first == std::numeric_limits<char>::min())
                  {
                    invert.push_back(new range_random_regex_node_(cur.second + 1, std::numeric_limits<char>::max()));
                  }
                  else
                  {
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                  }

                  break;
                }

                continue;
              }
              else if(cur.second+1 >= next.first) // adjacent or overlapping ranges
              {
                if(it+1 == ranges.end()) // TODO merge with upper
                {
                  if(cur.first != std::numeric_limits<char>::min() && next.second != std::numeric_limits<char>::max())
                  {
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                    invert.push_back(new range_random_regex_node_(next.second + 1, std::numeric_limits<char>::max()));
                  }
                  else if(cur.first == std::numeric_limits<char>::min() && next.second == std::numeric_limits<char>::max())
                  {
                    // in rare case where no characters are allowed throw an exception as regex is faulty...
                    throw 1; // TODO exception
                  }
                  else if(cur.first == std::numeric_limits<char>::min())
                  {
                    invert.push_back(new range_random_regex_node_(next.second + 1, std::numeric_limits<char>::max()));
                  }
                  else
                  {
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                  }

                  break;
                }

                cur.second = next.second;
              }
              else // ranges aren't connected
              {
                if(cur.first != std::numeric_limits<char>::min() && cur.second != std::numeric_limits<char>::max())
                {
                  invert.push_back(new range_random_regex_node_(min, static_cast<char>(cur.first - 1)));
                  invert.push_back(new range_random_regex_node_(static_cast<char>(cur.second + 1), static_cast<char>(next.first - 1)));
                }
                else if(cur.first == std::numeric_limits<char>::min() && cur.second == std::numeric_limits<char>::max())
                {
                  // in rare case where no characters are allowed throw an exception as regex is faulty...
                  throw 1; // TODO exception
                }
                // else skip the element as not allowed numbers are from beginning to end of this range
                // TODO !min but ==max with merge, merge, merge...???

                min = cur.second + 1;
                cur_it = it;
              }
            }
          }

          ++consumed; // consume ]
          return new or_regex_node_(std::move(invert));
        }}},
      [range_parser](auto& regex, auto& consumed){
          auto tmp = new or_regex_node_(range_parser(regex, consumed)); // TODO check if range parser returns an empty vector
          ++consumed; // consume ]
          return tmp;
        }
    };

  or_parser parser_base_type{{
        {'(', [](auto& regex, auto& consumed)
          {
            auto node = parser_regex(regex, consumed);
            ++consumed;
            return node;
          }}, // TODO, ')'},
        {'[', [range_or_negated_range](auto& regex, auto& consumed){return range_or_negated_range(regex, consumed);}},
        {'\\', [parser_escaped](auto& regex, auto& consumed){return parser_escaped(regex, consumed);}},
        {'.', [](auto& regex, auto& consumed){++consumed; return new random_regex_node_();}}
      },
      [](auto& regex, auto& consumed){return new literal_regex_node_(regex[consumed++]);}
    };

  return parser_base_type(regex, consumed);
}

regex_template::regex_template(const std::string& regex)
{
  std::size_t consumed = 0;

  root_node_.reset(parser_regex(regex, consumed));

  if(regex.size() < consumed)
  {
    // TODO throw invalid regex exception
  }
}

void regex_template::generate(std::ostream& os) const
{
  random_generator<> rand; // TODO should be provided from the outside
  root_node_->generate(os, rand);
}
