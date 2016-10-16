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
#include "range_regex_node.hpp"
#include "negated_range_regex_node.hpp"
#include "random_regex_node.hpp"
#include "repeat_zero_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "repeat_one_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "repeat_min_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "whitespace_regex_node.hpp"
#include "literal_regex_node.hpp"

using rand_regex::regex_node_;
using rand_regex::group_regex_node_;
using rand_regex::or_regex_node_;
using rand_regex::range_random_regex_node_;
using rand_regex::range_regex_node_;
using rand_regex::negated_range_regex_node_;
using rand_regex::random_regex_node_;
using rand_regex::repeat_zero_regex_node_;
using rand_regex::optional_regex_node_;
using rand_regex::repeat_one_regex_node_;
using rand_regex::repeat_regex_node_;
using rand_regex::repeat_min_regex_node_;
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

  regex_node_* operator()(const std::string& regex, std::size_t& consumed) const
  {
    regex_node_* node = nullptr;

    if(regex.size() > consumed)
    {
      for(auto start_token : start_tokens_)
      {
        if(regex[consumed] == start_token)
        {
          node = consumer_(regex, consumed);
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
  and_parser() {}

private:
  std::vector<parser> parsers_;
};

struct or_parser
{
public:
  or_parser(std::initializer_list<parser> or_parsers) : or_parsers_{or_parsers} {}

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

    //throw 1; // TODO throw no match found exception
    return nullptr;
  }
private:
  std::vector<parser> or_parsers_;
};

regex_node_* parser_term(const std::string& regex, std::size_t& consumed);
regex_node_* parser_factor(const std::string& regex, std::size_t& consumed);
regex_node_* parser_base(const std::string& regex, std::size_t& consumed);

regex_node_* parse_range(const or_parser& parser_escaped, const std::string& regex, std::size_t& consumed)
{
  regex_node_* node; // TODO should be a smart pointer
  ++consumed; // consume [

  if(regex[consumed] == '^')
  {
    ++consumed;
    std::vector<std::pair<char, char>> ranges;

    // TODO have to think this through...
    // node = new negated_range_regex_node_(std::move(ranges));
  }
  else
  {
    std::vector<regex_node_*> child_nodes;

    // <range> ::= <char> | <char>-<char> | <char><range> | <char>-<char><range>
    while(regex.size() > consumed)
    {
      if(regex[consumed] == ']')
      {
        break;
      }
      else
      {
        if(regex[consumed] == '\\') /// this section is a copy from below...
        {
          ++consumed; // consume \

          // if last character of regex is \ -> error
          if(regex.size() == consumed)
          {
            /// TODO exception handling
            throw 1;
          }

          auto tmp = parser_escaped(regex, consumed);
          if(tmp)
          {
            child_nodes.push_back(tmp);
          }
          else
          {
            /// TODO exception handling
            throw 1;
          }
        }
        else if((regex[consumed] == '-' && child_nodes.size()) && (consumed+1 < regex.size() && regex[consumed+1] != ']'))
        {
          literal_regex_node_* literal_node = dynamic_cast<literal_regex_node_*>(*(child_nodes.end()-1));

          if(literal_node)
          {
            ++consumed; // TODO everywhere check that there is still one character left... (in this case if it isn't it's an error)
            char tmp;

            if(regex[consumed] == '\\')
            {
              ++consumed; // consume '\'

              // if last character of regex is \ -> error
              if(regex.size() == consumed)
              {
                /// TODO exception handling
                throw 1;
              }

              if(regex[consumed] == 'f') // form feed
              {
                *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), '\f');
              }
              else if(regex[consumed] == 'n') // new line
              {
                *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), '\n');
              }
              else if(regex[consumed] == 'r') // carriage return
              {
                *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), '\r');
              }
              else if(regex[consumed] == 't') // horizontal tab
              {
                *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), '\t');
              }
              else if(regex[consumed] == 'v') // vertical tab
              {
                *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), '\v');
              }
              else if(regex[consumed] == '\\' || regex[consumed] == '|' || regex[consumed] == '.' || regex[consumed] == '-'
                  || regex[consumed] == '^' || regex[consumed] == '?' || regex[consumed] == '*' || regex[consumed] == '+'
                  || regex[consumed] == '{' || regex[consumed] == '}' || regex[consumed] == '(' || regex[consumed] == ')'
                  || regex[consumed] == '[' || regex[consumed] == ']')
              {
                *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), regex[consumed]);
              }
/*these all fall into else...
              else if(regex[consumed] == 'd') // any digit
              {
                /// TODO exception handling - this is not a single character so it cant be part of a range
                throw 1;
              }
              else if(regex[consumed] == 'D') // any non-digit
              {
                /// TODO exception handling - this is not a single character so it cant be part of a range
                throw 1;
              }
              else if(regex[consumed] == 's') // any whitespace
              {
                /// TODO exception handling - this is not a single character so it cant be part of a range
                throw 1;
              }
              else if(regex[consumed] == 'S') // any non-whitespace
              {
                /// TODO exception handling - this is not a single character so it cant be part of a range
                throw 1;
              }
              else if(regex[consumed] == 'w') // any alphanumeric characters or _
              {
                /// TODO exception handling - this is not a single character so it cant be part of a range
                throw 1;
              }
              else if(regex[consumed] == 'W') // characters other than alphanumeric characters or _
              {
                /// TODO exception handling - this is not a single character so it cant be part of a range
                throw 1;
              }*/
              else
              {
                /// TODO exception handling
                throw 1;
              }
            }/// TODO FIXME we still have to check which of the escaped characters it is and append that one...
            else
            {
              *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), regex[consumed]); // TODO if first > secont then error (a-z is OK z-a is an error)
            }

            delete literal_node;
          }
          else
          {
            // TODO this branch should also handle escaped chars...
            child_nodes.push_back(new literal_regex_node_(regex[consumed]));
          }
          
          ++consumed;
        }
        else
        {
          child_nodes.push_back(new literal_regex_node_(regex[consumed]));
          ++consumed;
        }
      }
    }

    node = new range_regex_node_(std::move(child_nodes));
  }

  ++consumed; // consume ]

  return node;
}

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

  if(consumed < regex.size() && regex[consumed] == '*')
  {
    node = new repeat_zero_regex_node_(node);

    do
    {
      ++consumed; // consume *
    }
    while(consumed < regex.size() && regex[consumed] == '*');
  }

  if(consumed < regex.size() && regex[consumed] == '+')
  {
    node = new repeat_one_regex_node_(node);

    // ++ is posessive but for the sake of generation it doesn't make any
    // difference as it may always match one or more times...
    do
    {
      ++consumed; // consume +
    }
    while(consumed < regex.size() && regex[consumed] == '+');
  }

  if(consumed < regex.size() && regex[consumed] == '?')
  {
    node = new optional_regex_node_(node);

    // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
    // so for generation sake it doesn't make any difference as it may always
    // match 0 or 1 times
    do
    {
      ++consumed; // consume +
    }
    while(consumed < regex.size() && regex[consumed] == '?');
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
          node = new repeat_min_regex_node_(node, min);
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

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
regex_node_* parser_base(const std::string& regex, std::size_t& consumed)
{
  auto form_feed = parser{'f', [](auto& regex, auto& consumed){++consumed; return new literal_regex_node_{'\f'};}};
  auto new_line = parser{'n', [](auto& regex, auto& consumed){++consumed; return new literal_regex_node_{'\n'};}};
  auto carriage_return = parser{'r', [](auto& regex, auto& consumed){++consumed; return new literal_regex_node_{'\r'};}};
  auto horizontal_tab = parser{'t', [](auto& regex, auto& consumed){++consumed; return new literal_regex_node_{'\t'};}};
  auto vertical_tab = parser{'v', [](auto& regex, auto& consumed){++consumed; return new literal_regex_node_{'\v'};}};
  auto escaped_literal_char = parser{{'\\', '|', '.', '-', '^', '?', '*', '+', '{', '}', '(', ')', '[', ']'},
        [](auto& regex, auto& consumed){return new literal_regex_node_(regex[consumed++]);}};
  auto any_digit = parser{'d', [](auto& regex, auto& consumed){++consumed; return new range_random_regex_node_{'0', '9'};}};
  auto any_whitespace = parser{'s', [](auto& regex, auto& consumed){++consumed; return new whitespace_regex_node_{};}};
  auto any_alphanum_or_underscore = parser{'w', [](auto& regex, auto& consumed) // any alphanumeric characters or _
        {
          ++consumed;
          return new or_regex_node_{new range_random_regex_node_('a', 'z'),
                                    new range_random_regex_node_('0', '9'),
                                    new literal_regex_node_('_')};
        }
      };

  or_parser parser_escaped{
      form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      escaped_literal_char,
//      {'f', [](const std::string& regex, std::size_t& consumed){++consumed; return new literal_regex_node_('\f');}},
      any_digit,
//      {'D', [](auto& regex, auto& consumed){throw 1;}}, // any non-digit // TODO implement
      any_whitespace,
//      {'S', [](auto& regex, auto& consumed){throw 1;}}, // any non-whitespace // TODO implement
      any_alphanum_or_underscore/*,
      {'W', [](auto& regex, auto& consumed){throw 1;}} // characters other than alphanumeric characters or _ // TODO implement*/
    };

  or_parser parser_base_type{
      {'(', [](auto& regex, auto& consumed)
        {
          ++consumed;
          auto node = parser_regex(regex, consumed);
          ++consumed;
          return node;
        }},
      {'[', [parser_escaped](auto& regex, auto& consumed){return parse_range(parser_escaped, regex, consumed);}},
      {'\\', [parser_escaped](auto& regex, auto& consumed){++consumed; return parser_escaped(regex, consumed);}},
      {'.', [](auto& regex, auto& consumed){++consumed; return new random_regex_node_();}}/*,
      {'', [](auto& regex, auto& consumed){return parse_literal(regex, consumed);}} TODO we need support for else...*/
    };

  auto node = parser_base_type(regex, consumed);
  if(!node) // else
  {
    node = new literal_regex_node_(regex[consumed++]);;
  }

  return node;
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
  root_node_->generate(os);
}

std::string generate(const regex_template& gen)
{
  std::stringstream ss;

  gen.generate(ss);

  return ss.str();
}
