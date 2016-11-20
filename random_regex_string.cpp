#include "random_regex_string.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <functional>

using rand_regex::captured_group_reference_node_;
using rand_regex::group_regex_node_;
using rand_regex::literal_regex_node_;
using rand_regex::optional_regex_node_;
using rand_regex::or_regex_node_;
using rand_regex::range_random_regex_node_;
using rand_regex::random_regex_node_;
using rand_regex::regex_node_;
using rand_regex::repeat_range_regex_node_;
using rand_regex::repeat_regex_node_;
using rand_regex::whitespace_regex_node_;

constexpr char ascii_min = 0;
constexpr char ascii_max = 127;


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

struct parser_param
{
  parser_param(std::vector<regex_node_*>& nodes_, std::experimental::string_view regex_) : nodes{nodes_}, regex{regex_} {}

  std::vector<regex_node_*>& nodes;
  std::experimental::string_view regex;
  std::size_t consumed = 0;
  std::vector<regex_node_*> captured_groups;
  regex_node_* previous_node = nullptr;
};

using regex_consumer_function = std::function<regex_node_* (parser_param&)>;

// class regex_template
using rand_regex::regex_template;

struct parser
{
public:
  parser(char start_token, regex_consumer_function consumer)
    : start_tokens_{{start_token}}
    , consumer_{consumer}
  {
    //
  }

  parser(std::vector<char> start_tokens, regex_consumer_function consumer)
    : start_tokens_{std::forward<std::vector<char>>(start_tokens)}
    , consumer_{consumer}
  {
    //
  }

  // TODO add size checked and unchecked version as other parser types can handle check at the top level for all (or/and parsers)
  //      for now failing size checks forces or/and parsers to continue untill all parsers are used up
//  template<bool truncate=true>
  regex_node_* operator()(parser_param& param) const
  {
    regex_node_* node = nullptr;

    if(param.regex.size() > param.consumed)
    {
      for(auto start_token : start_tokens_)
      {
        if(param.regex[param.consumed] == start_token)
        {
          ++param.consumed;
          node = consumer_(param);

          if(!node) {
            --param.consumed;
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
  regex_consumer_function consumer_;
};

struct and_parser
{
public:
  and_parser(std::initializer_list<parser> and_parsers, char terminator,
        regex_consumer_function else_parser = nullptr)
    : parsers_{and_parsers}, terminator_{terminator}, else_parser_{else_parser} {}

  // TODO this should be optional return value
  std::vector<regex_node_*> operator()(parser_param& param) const
  {
    std::vector<regex_node_*> nodes;

    while(param.regex.size() > param.consumed && param.regex[param.consumed] != terminator_)
    {
      bool found = false;
      for(auto& p : parsers_)
      {
        //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
        auto node = p(param);
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
          nodes.push_back(else_parser_(param)); // TODO check if null is returned...
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

    if(param.regex.size() == param.consumed && param.regex[param.consumed-1] != terminator_)
      throw 1; // handle case when terminator is not reached

    return nodes;
  }

private:
  std::vector<parser> parsers_;
  regex_consumer_function else_parser_;
  char terminator_;
};

struct or_parser
{
public:
  or_parser(std::initializer_list<parser> or_parsers,
      regex_consumer_function else_parser = nullptr
    ) : or_parsers_{or_parsers}, else_parser_{else_parser} {}

  regex_node_* operator()(parser_param& param) const
  {
    if(param.regex.size() == param.consumed)
      throw 1; // error should contain one more character

    for(auto& p : or_parsers_)
    {
      //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
      auto node = p(param);
      if(node)
        return node;
    }

    if(else_parser_)
      return else_parser_(param);

    throw 1; // TODO throw no match found exception
  }
private:
  std::vector<parser> or_parsers_;
  regex_consumer_function else_parser_;
};

regex_node_* parser_term(parser_param& param);
regex_node_* parser_factor(parser_param& param);
regex_node_* parser_base(parser_param& param);

// <regex> ::= <term> '|' <regex> | <term>
regex_node_* parser_regex(parser_param& param)
{
  if(param.regex[param.consumed] == '|')
  {
    ++param.consumed;
  }

  regex_node_* node = parser_term(param);

  if(param.consumed < param.regex.size() && param.regex[param.consumed] == '|')
  {
    ++param.consumed; // consume |
    regex_node_* other = parser_regex(param);
    param.nodes.push_back(new or_regex_node_{node, other});
    node = param.nodes[param.nodes.size()-1];
  }

  return node;
}

// <term> ::= { <factor> }
regex_node_* parser_term(parser_param& param)
{
  regex_node_* node = parser_factor(param);

  while(param.consumed < param.regex.size() && param.regex[param.consumed] != ')'
    && param.regex[param.consumed] != '|')
  {
    regex_node_* next = parser_factor(param);
    param.nodes.push_back(new group_regex_node_(std::vector<regex_node_*>{node, next})); // TODO consider renaming it to sequence
    node = param.nodes[param.nodes.size()-1];
  }

  return node;
}

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
regex_node_* parser_factor(parser_param& param)
{
  if(param.regex.size() == param.consumed)
  {
    param.nodes.push_back(new regex_node_{});
    return new regex_node_{}; // empty or
  }

  regex_node_* node = parser_base(param);

  auto repeat_range_zero = parser{'*', [node](parser_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          param.consumed = param.regex.find_first_not_of('*', param.consumed);
          
          param.nodes.push_back(new repeat_range_regex_node_(node, 0));
          return param.nodes[param.nodes.size()-1];
        }};
  auto repeat_range_one = parser{'+', [node](parser_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          param.consumed = param.regex.find_first_not_of('+', param.consumed);
          param.nodes.push_back(new repeat_range_regex_node_(node, 1));
          return param.nodes[param.nodes.size()-1];
        }};
  auto optional_item = parser{'?', [node](parser_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // star must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          param.consumed = param.regex.find_first_not_of('?', param.consumed);
          param.nodes.push_back(new optional_regex_node_(node));
          return param.nodes[param.nodes.size()-1];
        }};

  repeat_range_zero(param);
  repeat_range_one(param);
  optional_item(param);

  auto digit_parser = [](parser_param& param){
        std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed); // TODO find in range 0-9 would be nice...

        if(end == std::experimental::string_view::npos || end == param.consumed)
          throw 1; // TODO exception

        int digit = std::stoi(param.regex.substr(param.consumed, end).to_string());

        if(digit < 0)
          throw 1; // TODO exception

        param.consumed = end;

        return digit;
      };

  if(param.consumed < param.regex.size() && param.regex[param.consumed] == '{')
  {
    ++param.consumed; // consume {
    std::size_t num = 0;
    if(param.consumed < param.regex.size())
    {
      if(param.regex[param.consumed] != ',')
        num = digit_parser(param);
      else
        /// TODO handle exception
        throw 1; // syntax x{,y} is not supported
    }
    else
      throw 1; // TODO exception - unexpected end of regex

    if(param.consumed < param.regex.size() && param.regex[param.consumed] == ',')
    {
      if(param.regex.size() > ++param.consumed)
      {
        if(param.regex[param.consumed] == '}')
        {
          param.nodes.push_back(new repeat_range_regex_node_(node, num));
          node = param.nodes.back();
        }
        else
        {
          param.nodes.push_back(new repeat_range_regex_node_(node, num, digit_parser(param)));
          node = param.nodes.back();
        }
      }
      else
        /// TODO error handling
        throw 1;
    }
    else if(param.regex.size() > param.consumed && param.regex[param.consumed] == '}')
    {
      param.nodes.push_back(new repeat_regex_node_(node, num));
      node = param.nodes.back();
    }
    else
      /// TODO error handling
      throw 1;

    ++param.consumed; // consume }
  }

  return node;
}

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
regex_node_* parser_base(parser_param& param)
{
  auto form_feed = parser{'f', [](parser_param& param){
      param.nodes.push_back(new literal_regex_node_{'\f'});
      return param.nodes.back();
    }};
  auto new_line = parser{'n', [](parser_param& param){
      param.nodes.push_back(new literal_regex_node_{'\n'});
      return param.nodes.back();
    }};
  auto carriage_return = parser{'r', [](parser_param& param){
      param.nodes.push_back(new literal_regex_node_{'\r'});
      return param.nodes.back();
    }};
  auto horizontal_tab = parser{'t', [](parser_param& param){
      param.nodes.push_back(new literal_regex_node_{'\t'});
      return param.nodes.back();
    }};
  auto vertical_tab = parser{'v', [](parser_param& param){
      param.nodes.push_back(new literal_regex_node_{'\v'});
      return param.nodes.back();
    }};
  auto any_digit = parser{'d', [](parser_param& param){
      param.nodes.push_back(new range_random_regex_node_{'0', '9'});
      return param.nodes.back();
    }};
  auto null_character = parser{'0', [](parser_param& param){
      param.nodes.push_back(new literal_regex_node_{'\0'});
      return param.nodes.back();
    }};
  auto any_non_digit = parser{'D', [](parser_param& param){
          return new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                                    new range_random_regex_node_{'9' + 1, ascii_max}};
        }};
  auto any_whitespace = parser{'s', [](parser_param& param){return new whitespace_regex_node_{};}};
  auto any_non_whitespace = parser{'S', [](parser_param& param){
       /* '\t' // tab: 9
          '\n' // newline: 10
          '\v' // vertical tab: 11
          '\f' // formfeed: 12
          '\r' // carriage return: 13
          ' ' // space: 32 */
          param.nodes.push_back(new range_random_regex_node_{ascii_min, '\t' - 1});
          param.nodes.push_back(new range_random_regex_node_{'\r' + 1, ' ' - 1});
          param.nodes.push_back(new range_random_regex_node_{' ' + 1, ascii_max});
          param.nodes.push_back(new or_regex_node_{param.nodes[param.nodes.size()-3],
                                               param.nodes[param.nodes.size()-2],
                                               param.nodes[param.nodes.size()-1]});
          return param.nodes.back();
        }};
  auto any_alphanum_or_underscore = parser{'w', [](parser_param& param) // any alphanumeric characters or _
        {
          param.nodes.push_back(new range_random_regex_node_{'a', 'z'});
          param.nodes.push_back(new range_random_regex_node_{'0', '9'});
          param.nodes.push_back(new literal_regex_node_{'_'});
          param.nodes.push_back(new or_regex_node_{param.nodes[param.nodes.size()-3],
                                               param.nodes[param.nodes.size()-2],
                                               param.nodes[param.nodes.size()-1]});
          return param.nodes.back();
        }};
  auto any_not_alphanum_or_underscore = parser{'W', [](parser_param& param){
          param.nodes.push_back(new range_random_regex_node_{ascii_min, '0' - 1});
          param.nodes.push_back(new range_random_regex_node_{'9' + 1, 'A' - 1});
          param.nodes.push_back(new range_random_regex_node_{'Z' + 1, '_' - 1});
          param.nodes.push_back(new range_random_regex_node_{'_' + 1, 'a' - 1});
          param.nodes.push_back(new range_random_regex_node_{'z' + 1, ascii_max});
          param.nodes.push_back(new or_regex_node_{param.nodes[param.nodes.size()-5],
                                               param.nodes[param.nodes.size()-4],
                                               param.nodes[param.nodes.size()-3],
                                               param.nodes[param.nodes.size()-2],
                                               param.nodes[param.nodes.size()-1]});
          return param.nodes.back();
        }};
  auto hexadecimal_ascii_character_representation = parser{'x', [](parser_param& param){ // \x00 to \x7F
          if(param.regex.size() < param.consumed+2 || !::isxdigit((int)param.regex[param.consumed]) || !::isxdigit((int)param.regex[param.consumed+1]))
            throw 1;

          int ascii_hex = std::stoi(std::string(&param.regex[param.consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw 1;

          param.consumed += 2;

          param.nodes.push_back(new literal_regex_node_(static_cast<char>(ascii_hex)));
          return param.nodes.back();
        }};
  auto word_boundary = parser{'b', [](parser_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \b doesn't have a value after it - not valid
            throw 1;

          if(param.consumed - 2 == 0) // case when \b doesn't have a value before it - not valid
            throw 1;

          if(!::isspace(param.regex[param.consumed - 3]) && !::isspace(param.regex[param.consumed + 1]))
            throw 1;

          param.nodes.push_back(new regex_node_{});
          return new regex_node_(); // TODO remove dummy node as it is useles
        }};
  auto not_word_boundary = parser{'B', [](parser_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \B doesn't have a value after it - could cause a partial match
            throw 1;

          if(param.consumed - 2 == 0) // case when \B doesn't have a value before it - could cause a partial match
            throw 1;

          if(::isspace(param.regex[param.consumed - 3]) || ::isspace(param.regex[param.consumed + 1]))
            throw 1;

          param.nodes.push_back(new regex_node_{});
          return new regex_node_(); // TODO remove dummy node as it is useles
        }};
  auto backreference_parser = parser{{'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        [](parser_param& param){
          std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed);

          int digit = std::stoi(param.regex.substr(param.consumed-1, end).to_string());

          if(digit > param.captured_groups.size() || param.captured_groups[digit - 1] == nullptr)
            throw 1; // captured group doesn't exist || capturing group while inside of it
          else
            param.consumed = end;

          param.nodes.push_back(new captured_group_reference_node_{param.captured_groups[digit - 1]});
          return param.nodes.back();
        }};


  or_parser parser_escaped {
      {form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      any_digit,
      any_non_digit,
      any_whitespace,
      any_non_whitespace,
      any_alphanum_or_underscore,
      any_not_alphanum_or_underscore,
      hexadecimal_ascii_character_representation,
      word_boundary,
      not_word_boundary,
      null_character,
      backreference_parser},
      [](parser_param& param){
        param.nodes.push_back(new literal_regex_node_{param.regex[param.consumed++]}); // escaped_literal_char
        return param.nodes.back();
      }
    };

  or_parser escaped_or_literal {
      {{'\\', [parser_escaped](parser_param& param){return parser_escaped(param);}}},
      [](parser_param& param){
        param.nodes.push_back(new literal_regex_node_{param.regex[param.consumed++]});
        return param.nodes.back();
      }
    };

  or_parser parse_end_range_escaped {
      {form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      hexadecimal_ascii_character_representation},
      [](parser_param& param){
        param.nodes.push_back(new literal_regex_node_(param.regex[param.consumed++])); // escaped_literal_char (TODO should non valid escapes be removed? Throwing exception out of them...)
        return param.nodes.back();
      }
    };

  or_parser end_range_escaped_or_literal {
      {{'\\', [parse_end_range_escaped](parser_param& param){return parse_end_range_escaped(param);}}},
      [](parser_param& param){
        param.nodes.push_back(new literal_regex_node_(param.regex[param.consumed++]));
        return param.nodes.back();
      }
    };

  and_parser range_parser {
      {},
      ']',
      [escaped_or_literal, end_range_escaped_or_literal](parser_param& param)
      {
        auto tmp_node = escaped_or_literal(param);
        if(!tmp_node)
        {
          /// TODO exception handling
          throw 1;
        }

        if(param.consumed+2 < param.regex.size() && param.regex[param.consumed] == '-' && param.regex[param.consumed+1] != ']')
        {
          literal_regex_node_* literal_node = static_cast<literal_regex_node_*>(tmp_node);

          ++param.consumed;

          if(param.consumed+1 >= param.regex.size())
          {
            /// TODO exception handling
            delete tmp_node; // TODO RAII
            throw 1;
          }

          auto tmp = end_range_escaped_or_literal(param);
          if(!tmp)
          {
            /// TODO exception handling
            delete tmp_node; // TODO RAII
            throw 1;
          }

          param.nodes.push_back(new range_random_regex_node_(literal_node->getLiteral(), static_cast<literal_regex_node_*>(tmp)->getLiteral())); // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_
          tmp_node = param.nodes.back();
          delete tmp;
          delete literal_node;
        }

        return tmp_node;
      }
    };

  or_parser range_or_negated_range {
      {{'^', [range_parser](parser_param& param){
          auto tmp = range_parser(param);
          // TODO check if range parser returns an empty vector

          std::vector<regex_node_*> invert;
          std::vector<std::pair<char, char>> ranges;

          // convert
          for(auto element : tmp)
          {
            if(auto lit = dynamic_cast<literal_regex_node_*>(element))
              ranges.emplace_back(lit->getLiteral(), lit->getLiteral());
            else if(auto range = dynamic_cast<range_random_regex_node_*>(element))
              ranges.emplace_back(range->get_from(), range->get_to());

            delete element;
          }

          // handle single range case
          if(tmp.size() == 1)
          {
            // TODO merge
            if(ranges[0].first != ascii_min && ranges[0].second != ascii_max)
            {
              invert.push_back(new range_random_regex_node_(ascii_min, ranges[0].first - 1));
              invert.push_back(new range_random_regex_node_(ranges[0].second + 1, ascii_max));
            }
            else if(ranges[0].first == ascii_min && ranges[0].second == ascii_max)
              // in rare case where no characters are allowed throw an exception as regex is faulty...
              throw 1; // TODO exception
            else if(ranges[0].first == ascii_min)
              invert.push_back(new range_random_regex_node_(ranges[0].second + 1, ascii_max));
            else
              invert.push_back(new range_random_regex_node_(ascii_min, ranges[0].first - 1));
          }
          else
          {
            std::sort(ranges.begin(), ranges.end(), [](auto& i, auto& j){return (i.first<j.first);});

            // merge and invert ranges
            auto min = ascii_min;
            auto cur_it = ranges.begin();
            for(auto it = ranges.begin() + 1; it != ranges.end(); ++it)
            {
              auto& cur = *cur_it;
              auto& next = *it;

              if(cur.second >= next.second) // because of the sort we know that first is either same or smaller so a sub range
              {
                if(it+1 == ranges.end()) // TODO merge with lower
                {
                  if(cur.first != ascii_min && cur.second != ascii_max)
                  {
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                    invert.push_back(new range_random_regex_node_(cur.second + 1, ascii_max));
                  }
                  else if(cur.first == ascii_min && cur.second == ascii_max)
                    // in rare case where no characters are allowed throw an exception as regex is faulty...
                    throw 1; // TODO exception
                  else if(cur.first == ascii_min)
                    invert.push_back(new range_random_regex_node_(cur.second + 1, ascii_max));
                  else
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));

                  break;
                }

                continue;
              }
              else if(cur.second+1 >= next.first) // adjacent or overlapping ranges
              {
                if(it+1 == ranges.end()) // TODO merge with upper
                {
                  if(cur.first != ascii_min && next.second != ascii_max)
                  {
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));
                    invert.push_back(new range_random_regex_node_(next.second + 1, ascii_max));
                  }
                  else if(cur.first == ascii_min && next.second == ascii_max)
                    // in rare case where no characters are allowed throw an exception as regex is faulty...
                    throw 1; // TODO exception
                  else if(cur.first == ascii_min)
                    invert.push_back(new range_random_regex_node_(next.second + 1, ascii_max));
                  else
                    invert.push_back(new range_random_regex_node_(min, cur.first - 1));

                  break;
                }

                cur.second = next.second;
              }
              else // ranges aren't connected
              {
                if(cur.first != ascii_min && cur.second != ascii_max)
                {
                  invert.push_back(new range_random_regex_node_(min, static_cast<char>(cur.first - 1)));
                  invert.push_back(new range_random_regex_node_(static_cast<char>(cur.second + 1), static_cast<char>(next.first - 1)));
                }
                else if(cur.first == ascii_min && cur.second == ascii_max)
                  // in rare case where no characters are allowed throw an exception as regex is faulty...
                  throw 1; // TODO exception
                // else skip the element as not allowed numbers are from beginning to end of this range
                // TODO !min but ==max with merge, merge, merge...???

                min = cur.second + 1;
                cur_it = it;
              }
            }
          }

          ++param.consumed; // consume ]
          param.nodes.push_back(new or_regex_node_(std::move(invert)));
          return param.nodes.back();
        }}},
      [range_parser](parser_param& param){
          param.nodes.push_back(new or_regex_node_(range_parser(param))); // TODO check if range parser returns an empty vector
          ++param.consumed; // consume ]
          return param.nodes.back();
        }
    };

  or_parser parser_base_type{{
        {'(', [](parser_param& param) // TODO parser_param& param -> auto [...] (proposal for exploding of params?) - also auto [fds,sfd,sfd] -> parser_param& for the call part... possibly not feasible as this can be done one line before or after with auto [dfs,dsfs,sdf]...
          {
            if(param.regex.size() == param.consumed + 1) // handle () case
            {
              if(param.regex[param.consumed] == ')')
              {
                ++param.consumed;
                param.nodes.push_back(new regex_node_{});
                return new regex_node_(); // empty
              }
              else
                throw 1; // not enough characters...
            }

            int capture_index = -1;
            if(param.regex[param.consumed] == '?')
            {
              // (?:...) is a non capturing group
              if(param.regex.size() > param.consumed + 2 && param.regex[param.consumed+1] == ':')
              {
                param.consumed += 2; // discard matches everything enclosed as that's exactly what will be generated anyway

                if(param.regex[param.consumed] == ')') // handle (?:) case
                {
                  ++param.consumed;
                  param.nodes.push_back(new regex_node_{});
                  return new regex_node_(); // empty
                }
              }
              else
                throw 1; // not enough characters
            }
            else
            {
              param.captured_groups.push_back(nullptr);
              capture_index = param.captured_groups.size() - 1;
            }

            param.previous_node = nullptr;
            auto node = parser_regex(param);
            if(param.regex.size() == param.consumed)
              throw 1; // missing closing bracket
            ++param.consumed;

            if(capture_index != -1)
              param.captured_groups[capture_index] = node;

            return node;
          }}, // TODO, ')'},
        {'[', [range_or_negated_range](parser_param& param){return range_or_negated_range(param);}},
        {'\\', [parser_escaped](parser_param& param){return parser_escaped(param);}},
        {'.', [](parser_param& param){
            param.nodes.push_back(new random_regex_node_()); // TODO swap for min max node?
            return param.nodes.back();
          }}
      },
      [](parser_param& param){
          if(param.regex[param.consumed] == '$' && param.regex.size() == param.consumed + 1) // handle end of text symbol (TODO add random text generation if $ is missing at the end - check if OK)
          {
            ++param.consumed;
            param.nodes.push_back(new regex_node_{});
            return new regex_node_();
          }

          param.nodes.push_back(new literal_regex_node_(param.regex[param.consumed++]));
          return param.nodes.back();
        }
    };

  return parser_base_type(param);
}

regex_template::regex_template(std::experimental::string_view regex)
{
  if(regex.size())
  {
    parser_param param{nodes_, regex};
    param.consumed = (regex[0] == '^'); // set to 0 if regex doesn't start with start of text symbol (^) otherwise start with 1 (TODO add random text generation if ^ is missing at the beginning - check if OK)

    if(!param.consumed || regex.size() != 1)
    {
      root_node_.reset(parser_regex(param));

      /// TODO in some cases this is also true: regex.size() < consumed (check why???)
      if(param.regex.size() > param.consumed)
        // TODO throw invalid regex exception
        throw 1;
    }
  }
}

#include <iostream>
void regex_template::generate(std::ostream& os) const
{
  random_generator<> rand; // TODO should be provided from the outside

//---------------------------------
/*  regex_variant testv_{whitespace_regex_node_()};
std::cout << '\'';
std::visit([&](auto&& n){std::stringstream ss; n.generate(ss, rand); std::cout << ss.str();}, testv_);*/
/*  auto& tmp_n = std::get<whitespace_regex_node_>(testv_);
  std::stringstream ss;
  tmp_n.generate(ss, rand);*/
//    std::cout << "v.index = " << v.index() << '\n';
//std::cout << "'\n";
//---------------------------------

  if(root_node_.get()) // TODO this if could be changed for dummy regex node
    root_node_->generate(os, rand);
}
