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

  // TODO add size checked and unchecked version as other parser types can handle check at the top level for all (or/and parsers)
  //      for now failing size checks forces or/and parsers to continue untill all parsers are used up
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
  and_parser(std::initializer_list<parser> and_parsers, char terminator)
    : parsers_{and_parsers}, terminator_{terminator} {}

  // TODO this should be optional return value
  std::vector<regex_node_*> operator()(const std::string& regex, std::size_t& consumed) const
  {
    std::vector<regex_node_*> nodes;

    while(regex.size() > consumed)
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
        //throw 1; // TODO throw no match found exception
        for(auto node : nodes)
          delete node;

        nodes.clear();
      }
    }

    return nodes;
  }

private:
  std::vector<parser> parsers_;
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
  auto any_digit = parser{'d', [](auto& regex, auto& consumed){++consumed; return new range_random_regex_node_{'0', '9'};}}; // TODO this should not be in [] section if range is used ([x-y])
  auto any_whitespace = parser{'s', [](auto& regex, auto& consumed){++consumed; return new whitespace_regex_node_{};}}; // TODO this should not be in [] section if range is used ([x-y])
  auto any_alphanum_or_underscore = parser{'w', [](auto& regex, auto& consumed) // any alphanumeric characters or _  // TODO this should not be in [] section if range is used ([x-y])
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

  or_parser escaped_or_literal {
      {{'\\', [parser_escaped](auto& regex, auto& consumed){++consumed; return parser_escaped(regex, consumed);}}},
      [](auto& regex, auto& consumed){return new literal_regex_node_(regex[consumed++]);}
    };

  or_parser range_or_negated_range {
      {{'^', [](auto& regex, auto& consumed){
          ++consumed;
          std::vector<std::pair<char, char>> ranges;

          // TODO have to think this through...
          // node = new negated_range_regex_node_(std::move(ranges));
          return nullptr;
        }}},
      [escaped_or_literal](auto& regex, auto& consumed){
          //return escaped_or_literal(regex, consumed);

          std::vector<regex_node_*> child_nodes;

          regex_node_* tmp_node = nullptr; // TODO should be a smart ptr...

          // <range> ::= <char> | <char>-<char> | <char><range> | <char>-<char><range>
          while(regex.size() > consumed)
          {
            if(regex[consumed] == ']')
            {
              break;
            }
            else
            {
              if((regex[consumed] == '-' && tmp_node) && (consumed+1 < regex.size() && regex[consumed+1] != ']')) // TODO if this second part happens an exception is probably in order
              {
                literal_regex_node_* literal_node = dynamic_cast<literal_regex_node_*>(tmp_node);

                ++consumed; // TODO everywhere check that there is still one character left... (in this case if it isn't it's an error)

                auto tmp = escaped_or_literal(regex, consumed);
                if(!tmp)
                {
                  /// TODO exception handling
                  throw 1;
                }

                child_nodes.push_back(new range_random_regex_node_(literal_node->getLiteral(), dynamic_cast<literal_regex_node_*>(tmp)->getLiteral())); // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_
                delete tmp;

                delete literal_node;
                tmp_node = nullptr;
              }
              else
              {
                auto tmp = escaped_or_literal(regex, consumed);
                if(tmp)
                {
                  if(tmp_node)
                    child_nodes.push_back(tmp_node);

                  tmp_node = tmp;
                  //child_nodes.push_back(tmp);
                }
                else
                {
                  /// TODO exception handling
                  throw 1;
                }
              }
            }
          }

          if(tmp_node)
            child_nodes.push_back(tmp_node);

          ++consumed; // consume ]

          return new range_regex_node_(std::move(child_nodes));
        }
    };

/*  and_parser parse_range_p {
      {{[](auto& regex, auto& consumed){
          return nullptr; // TODO
        }}
      },
      ']'
    };*/

  or_parser parser_base_type{{
        {'(', [](auto& regex, auto& consumed)
          {
            ++consumed;
            auto node = parser_regex(regex, consumed);
            ++consumed;
            return node;
          }},
        {'[', [range_or_negated_range](auto& regex, auto& consumed){++consumed; return range_or_negated_range(regex, consumed);}},
  /*      {'[', [parser_escaped](auto& regex, auto& consumed)
          {
            std::vector<regex_node_*> child_nodes;

            return new range_regex_node_(std::move(child_nodes));
          }
        },*/
        {'\\', [parser_escaped](auto& regex, auto& consumed){++consumed; return parser_escaped(regex, consumed);}},
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
  root_node_->generate(os);
}

std::string generate(const regex_template& gen)
{
  std::stringstream ss;

  gen.generate(ss);

  return ss.str();
}
