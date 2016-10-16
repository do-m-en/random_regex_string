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

// class regex_template
using rand_regex::regex_template;

struct parser
{
public:
  parser(char start_token, std::function<regex_node_* (const std::string& regex, std::size_t& consumed)> consumer)
    : start_token_{start_token}
    , consumer_{consumer}
  {
    //
  }

  regex_node_* operator()(const std::string& regex, std::size_t& consumed) const
  {
    regex_node_* node = nullptr;

    if(regex[consumed] == start_token_)
    {
      return consumer_(regex, consumed);
    }

    return node;
  }

private:
  char start_token_;
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
};

regex_node_* parser_term(const std::string& regex, std::size_t& consumed);
regex_node_* parser_factor(const std::string& regex, std::size_t& consumed);
regex_node_* parser_base(const std::string& regex, std::size_t& consumed);

regex_node_* parse_literal(const std::string& regex, std::size_t& consumed)
{
  return new literal_regex_node_(regex[consumed++]);
}

regex_node_* parse_random(const std::string& regex, std::size_t& consumed)
{
  ++consumed; // consume '.'
  return new random_regex_node_();
}

regex_node_* parse_escaped(const std::string& regex, std::size_t& consumed)
{
  regex_node_* node; /// TODO should be a smart pointer...
  ++consumed; // consume '\'

  // if last character of regex is \ -> error
  if(regex.size() == consumed)
  {
    /// TODO exception handling
    throw 1;
  }

  if(regex[consumed] == 'f') // form feed
  {
    node = new literal_regex_node_('\f');
  }
  else if(regex[consumed] == 'n') // new line
  {
    node = new literal_regex_node_('\n');
  }
  else if(regex[consumed] == 'r') // carriage return
  {
    node = new literal_regex_node_('\r');
  }
  else if(regex[consumed] == 't') // horizontal tab
  {
    node = new literal_regex_node_('\t');
  }
  else if(regex[consumed] == 'v') // vertical tab
  {
    node = new literal_regex_node_('\v');
  }
  else if(regex[consumed] == '\\' || regex[consumed] == '|' || regex[consumed] == '.' || regex[consumed] == '-'
      || regex[consumed] == '^' || regex[consumed] == '?' || regex[consumed] == '*' || regex[consumed] == '+'
      || regex[consumed] == '{' || regex[consumed] == '}' || regex[consumed] == '(' || regex[consumed] == ')'
      || regex[consumed] == '[' || regex[consumed] == ']')
  {
    node = new literal_regex_node_(regex[consumed]);
  }
  else if(regex[consumed] == 'd') // any digit
  {
    node = new range_random_regex_node_('0', '9');
  }
  else if(regex[consumed] == 'D') // any non-digit
  {
    // TODO implement
    throw 1;
  }
  else if(regex[consumed] == 's') // any whitespace
  {
    node = new whitespace_regex_node_();
  }
  else if(regex[consumed] == 'S') // any non-whitespace
  {
    // TODO implement
    throw 1;
  }
  else if(regex[consumed] == 'w') // any alphanumeric characters or _
  {
    node = new or_regex_node_{new range_random_regex_node_('a', 'z'),
                              new range_random_regex_node_('0', '9'),
                              new literal_regex_node_('_')};
  }
  else if(regex[consumed] == 'W') // characters other than alphanumeric characters or _
  {
    // TODO implement
    throw 1;
  }
  else
  {
    /// TODO exception handling
    throw 1;
  }

  ++consumed;
  
  return node;
}

regex_node_* parse_range(const std::string& regex, std::size_t& consumed)
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
      else if(regex[consumed] == '\\') /// this section is a copy from below...
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
          child_nodes.push_back(new literal_regex_node_('\f'));
        }
        else if(regex[consumed] == 'n') // new line
        {
          child_nodes.push_back(new literal_regex_node_('\n'));
        }
        else if(regex[consumed] == 'r') // carriage return
        {
          child_nodes.push_back(new literal_regex_node_('\r'));
        }
        else if(regex[consumed] == 't') // horizontal tab
        {
          child_nodes.push_back(new literal_regex_node_('\t'));
        }
        else if(regex[consumed] == 'v') // vertical tab
        {
          child_nodes.push_back(new literal_regex_node_('\v'));
        }
        else if(regex[consumed] == '\\' || regex[consumed] == '|' || regex[consumed] == '.' || regex[consumed] == '-'
            || regex[consumed] == '^' || regex[consumed] == '?' || regex[consumed] == '*' || regex[consumed] == '+'
            || regex[consumed] == '{' || regex[consumed] == '}' || regex[consumed] == '(' || regex[consumed] == ')'
            || regex[consumed] == '[' || regex[consumed] == ']')
        {
          child_nodes.push_back(new literal_regex_node_(regex[consumed]));
        }
        else if(regex[consumed] == 'd') // any digit
        {
          child_nodes.push_back(new range_random_regex_node_('0', '9'));
        }
        else if(regex[consumed] == 'D') // any non-digit
        {
          // TODO implement
          throw 1;
        }
        else if(regex[consumed] == 's') // any whitespace
        {
          child_nodes.push_back(new whitespace_regex_node_());
        }
        else if(regex[consumed] == 'S') // any non-whitespace
        {
          // TODO implement
          throw 1;
        }
        else if(regex[consumed] == 'w') // any alphanumeric characters or _
        {
          child_nodes.push_back(new range_random_regex_node_('a', 'z'));
          child_nodes.push_back(new range_random_regex_node_('0', '9'));
          child_nodes.push_back(new literal_regex_node_('_'));
        }
        else if(regex[consumed] == 'W') // characters other than alphanumeric characters or _
        {
          // TODO implement
          throw 1;
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
            }
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
          child_nodes.push_back(new literal_regex_node_(regex[consumed]));
        }
      }
      else
      {
        child_nodes.push_back(new literal_regex_node_(regex[consumed]));
      }

      ++consumed;
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
  regex_node_* node;

  if(regex[consumed] == '(') // TODO should there be a check for consumed < regex.size()?
  {
    ++consumed; // consume (
    node = parser_regex(regex, consumed);
    ++consumed; // consume )
  }
  else if(regex[consumed] == '[')
  {
    node = parse_range(regex, consumed);
  }
  else if(regex[consumed] == '\\')
  {
    node = parse_escaped(regex, consumed);
  }
  else if(regex[consumed] == '.')
  {
    node = parse_random(regex, consumed);
  }
  else
  {
    node = parse_literal(regex, consumed);
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
