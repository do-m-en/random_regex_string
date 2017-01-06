#include "random_regex_string.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <functional>
#include <iterator>
#include <numeric>

#include "captured_group_reference_node.hpp"
#include "group_regex_node.hpp"
#include "literal_regex_node.hpp"
#include "optional_regex_node.hpp"
#include "or_regex_node.hpp"
#include "random_regex_node.hpp"
#include "range_random_regex_node.hpp"
#include "repeat_range_regex_node.hpp"
#include "repeat_regex_node.hpp"
#include "whitespace_regex_node.hpp"

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


struct regex_param
{
  regex_param(std::string_view regex_) : regex{regex_} {}

  std::string_view regex;
  std::size_t consumed = 0;
  std::vector<int> captured_groups;

  std::size_t last_index = 0;
};

using parse_result = regex_node_*;
using regex_consumer_function = std::function<std::vector<parse_result> (regex_param&)>;

template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void)swallow{1,
        (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
    };
}

template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                  std::make_index_sequence<N>{});
}

// class regex_template
using rand_regex::regex_template;

template<typename ...ListOfChars>
struct parser
{
public:
  parser(std::tuple<ListOfChars...> start_tokens, regex_consumer_function consumer)
    : start_tokens_{start_tokens}
    , consumer_{consumer}
  {
    //
  }

  // TODO add size checked and unchecked version as other parser types can handle check at the top level for all (or/and parsers)
  //      for now failing size checks forces or/and parsers to continue untill all parsers are used up
//  template<bool truncate=true>
  std::vector<parse_result> operator()(regex_param& param) const
  {
    std::vector<parse_result> nodes;

    for_each(start_tokens_, [&nodes, &param, this](auto start_token) {
      if(!nodes.empty())
        return;

      if(param.regex[param.consumed] == start_token)
      {
        ++param.consumed;
        nodes = consumer_(param);

        if(nodes.empty()) {
          --param.consumed;
          throw 1; // TODO throw if start token was found but consumption did not take place
        }
      }
    });

    /*if(param.regex.size() > param.consumed)
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
    }*/

    return nodes;
  }

private:
  std::tuple<ListOfChars...> start_tokens_;
  regex_consumer_function consumer_;
};

template<typename ...ParserType>
struct and_parser
{
public:
  and_parser(std::tuple<ParserType...> and_parsers, char terminator,
        regex_consumer_function else_parser = nullptr)
    : parsers_{and_parsers}, terminator_{terminator}, else_parser_{else_parser} {}

  // TODO this should be optional return value
  std::vector<parse_result> operator()(regex_param& param) const
  {
    std::vector<parse_result> nodes;

    while(param.regex.size() > param.consumed && param.regex[param.consumed] != terminator_)
    {
      bool found = false;
      for_each(parsers_, [&found, &nodes, &param](auto p) {
        if(found)
          return;

        //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
        auto tmp = p(param);
        if(!tmp.empty())
        {
          nodes.insert(nodes.end(), tmp.begin(), tmp.end()); // TODO move...
          found = true;
        }
      });

      if(!found)
      {
        if(else_parser_)
        {
          auto tmp = else_parser_(param);
          nodes.insert(nodes.end(), tmp.begin(), tmp.end()); // TODO check if empty is returned... TODO move...
        }
        else
        {
          //throw 1; // TODO throw no match found exception

          throw 1;
        }
      }
    }

    if(param.regex.size() == param.consumed && param.regex[param.consumed-1] != terminator_)
      throw 1; // handle case when terminator is not reached (TODO delete...)

    return nodes;
  }

private:
  std::tuple<ParserType...> parsers_;
  regex_consumer_function else_parser_;
  char terminator_;
};

template<typename ...ParserType>
struct or_parser
{
public:
  or_parser(std::tuple<ParserType...> or_parsers,
      regex_consumer_function else_parser = nullptr
    ) : parsers_{or_parsers}, else_parser_{else_parser} {}

  std::vector<parse_result> operator()(regex_param& param) const
  {
    if(param.regex.size() == param.consumed)
      throw 1; // error should contain one more character

    std::vector<parse_result> nodes;

    for_each(parsers_, [&nodes, &param](auto p) {
      if(!nodes.empty())
        return;

      nodes = p(param);
    });

    if(!nodes.empty())
      return nodes;

/*    for(auto& p : or_parsers_)
    {
      //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
      auto node = p(param);
      if(node)
        return node;
    }*/

    if(else_parser_)
      return else_parser_(param);

    throw 1; // TODO throw no match found exception
  }
private:
  std::tuple<ParserType...> parsers_;
  regex_consumer_function else_parser_;
};

std::vector<parse_result> parser_term(regex_param& param);
std::vector<parse_result> parser_factor(regex_param& param);
std::vector<parse_result> parser_base(regex_param& param);
#include <iostream>
// <regex> ::= <term> '|' <regex> | <term>
std::vector<parse_result> parser_regex(regex_param& param)
{
  std::vector<parse_result> nodes{new or_regex_node_{}}; // TODO remove or node if single node
  ++param.last_index;

  while(param.consumed < param.regex.size())
  {
    if(param.regex[param.consumed] == '|')
      ++param.last_index;

    auto others = (param.regex[param.consumed] != '|' ? parser_term(param) : std::vector<parse_result>{new regex_node_{}}); // term or empty node

    if(others.empty())
    {
      // ERROR
      nodes.clear();
      break;
    }

    std::move(others.begin(), others.end(), std::back_inserter(nodes));
    static_cast<or_regex_node_*>(nodes[0])->append(others.size());

    if(param.consumed == param.regex.size() || param.regex[param.consumed] != '|')
      break;

    ++param.consumed; // consume |
  }

  return nodes;
}

// <term> ::= { <factor> }
std::vector<parse_result> parser_term(regex_param& param)
{
//  auto nodes = parser_factor(param);

  std::vector<parse_result> nodes{new group_regex_node_{}}; // TODO remove group node if single node
  ++param.last_index;

  while(param.consumed < param.regex.size() && param.regex[param.consumed] != ')'
    && param.regex[param.consumed] != '|')
  {
    auto next = parser_factor(param);
    std::size_t tmp = nodes.size() - 1;
    std::move(next.begin(), next.end(), std::back_inserter(nodes));
    static_cast<group_regex_node_*>(nodes[0])->append();
  }

  return nodes;
}

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
std::vector<parse_result> parser_factor(regex_param& param)
{
  if(param.regex.size() == param.consumed) // TODO I could probably delete this now
  {
    ++param.last_index;
    return std::vector<parse_result>{new regex_node_{}}; // empty or
  }

  auto nodes = parser_base(param);

  auto repeat_range_zero = parser{std::make_tuple('*'), [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          param.consumed = param.regex.find_first_not_of('*', param.consumed);

          ++param.last_index;
          return std::vector<parse_result>{new repeat_range_regex_node_(0)};
        }};
  auto repeat_range_one = parser{std::make_tuple('+'), [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          param.consumed = param.regex.find_first_not_of('+', param.consumed);

          ++param.last_index;
          return std::vector<parse_result>{new repeat_range_regex_node_(1)};
        }};
  auto optional_item = parser{std::make_tuple('?'), [](regex_param& param){
          if(param.consumed == 1 || (param.consumed == 2 && (param.regex[0] == '^' || param.regex[0] == '|')) || (param.regex[param.consumed - 1] == '|' && param.regex[param.consumed - 2] != '\\'))
            throw 1; // star must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          param.consumed = param.regex.find_first_not_of('?', param.consumed);

          ++param.last_index;
          return std::vector<parse_result>{new optional_regex_node_()};
        }};

  repeat_range_zero(param);
  repeat_range_one(param);
  optional_item(param);

  auto digit_parser = [](regex_param& param){
        std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed); // TODO find in range 0-9 would be nice...

        if(end == std::string_view::npos || end == param.consumed)
          throw 1; // TODO exception

        int digit = std::stoi(std::string(param.regex.substr(param.consumed, end)));

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
          ++param.last_index;
          nodes.insert(nodes.begin(), new repeat_range_regex_node_(num));
        }
        else
        {
          ++param.last_index;
          nodes.insert(nodes.begin(), new repeat_range_regex_node_(num, digit_parser(param)));
        }
      }
      else
        /// TODO error handling
        throw 1;
    }
    else if(param.regex.size() > param.consumed && param.regex[param.consumed] == '}')
    {
      ++param.last_index;
      nodes.insert(nodes.begin(), new repeat_regex_node_(num));
    }
    else
      /// TODO error handling
      throw 1;

    ++param.consumed; // consume }
  }

  return nodes;
}

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
std::vector<parse_result> parser_base(regex_param& param)
{
  auto form_feed = parser{std::make_tuple('f'), [](regex_param& param){
      ++param.last_index;
      return std::vector<parse_result>{new literal_regex_node_{'\f'}};
    }};
  auto new_line = parser{std::make_tuple('n'), [](regex_param& param){
      ++param.last_index;
      return std::vector<parse_result>{new literal_regex_node_{'\n'}};
    }};
  auto carriage_return = parser{std::make_tuple('r'), [](regex_param& param){
      ++param.last_index;
      return std::vector<parse_result>{new literal_regex_node_{'\r'}};
    }};
  auto horizontal_tab = parser{std::make_tuple('t'), [](regex_param& param){
      ++param.last_index;
      return std::vector<parse_result>{new literal_regex_node_{'\t'}};
    }};
  auto vertical_tab = parser{std::make_tuple('v'), [](regex_param& param){
      return std::vector<parse_result>{new literal_regex_node_{'\v'}};
    }};
  auto any_digit = parser{std::make_tuple('d'), [](regex_param& param){
      ++param.last_index;
      return std::vector<parse_result>{new range_random_regex_node_{'0', '9'}};
    }};
  auto null_character = parser{std::make_tuple('0'), [](regex_param& param){
      ++param.last_index;
      return std::vector<parse_result>{new literal_regex_node_{'\0'}};
    }};
  auto any_non_digit = parser{std::make_tuple('D'), [](regex_param& param){
          param.last_index += 3;
          return std::vector<parse_result>{new or_regex_node_{{1, 1}},
              new range_random_regex_node_{ascii_min, '0' - 1},
              new range_random_regex_node_{'9' + 1, ascii_max}};
        }};
  auto any_whitespace = parser{std::make_tuple('s'), [](regex_param& param){
          ++param.last_index;
          return std::vector<parse_result>{new whitespace_regex_node_{}};
        }};
  auto any_non_whitespace = parser{std::make_tuple('S'), [](regex_param& param){
       /* '\t' // tab: 9
          '\n' // newline: 10
          '\v' // vertical tab: 11
          '\f' // formfeed: 12
          '\r' // carriage return: 13
          ' ' // space: 32 */
          param.last_index += 4;
          return std::vector<parse_result>{new or_regex_node_{{1, 1, 1}},
              new range_random_regex_node_{ascii_min, '\t' - 1},
              new range_random_regex_node_{'\r' + 1, ' ' - 1},
              new range_random_regex_node_{' ' + 1, ascii_max}};
        }};
  auto any_alphanum_or_underscore = parser{std::make_tuple('w'), [](regex_param& param) // any alphanumeric characters or _
        {
          param.last_index += 4;
          return std::vector<parse_result>{new or_regex_node_{{1, 1, 1}},
              new range_random_regex_node_{'a', 'z'},
              new range_random_regex_node_{'0', '9'},
              new literal_regex_node_{'_'}};
        }};
  auto any_not_alphanum_or_underscore = parser{std::make_tuple('W'), [](regex_param& param){
          param.last_index += 6;
          return std::vector<parse_result>{new or_regex_node_{{1, 1, 1, 1, 1}},
              new range_random_regex_node_{ascii_min, '0' - 1},
              new range_random_regex_node_{'9' + 1, 'A' - 1},
              new range_random_regex_node_{'Z' + 1, '_' - 1},
              new range_random_regex_node_{'_' + 1, 'a' - 1},
              new range_random_regex_node_{'z' + 1, ascii_max}};
        }};
  auto hexadecimal_ascii_character_representation = parser{std::make_tuple('x'), [](regex_param& param){ // \x00 to \x7F
          if(param.regex.size() < param.consumed+2 || !::isxdigit((int)param.regex[param.consumed]) || !::isxdigit((int)param.regex[param.consumed+1]))
            throw 1;

          int ascii_hex = std::stoi(std::string(&param.regex[param.consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw 1;

          param.consumed += 2;

          ++param.last_index;
          return std::vector<parse_result>{new literal_regex_node_(static_cast<char>(ascii_hex))};
        }};
  auto word_boundary = parser{std::make_tuple('b'), [](regex_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \b doesn't have a value after it - not valid
            throw 1;

          if(param.consumed - 2 == 0) // case when \b doesn't have a value before it - not valid
            throw 1;

          if(!::isspace(param.regex[param.consumed - 3]) && !::isspace(param.regex[param.consumed + 1]))
            throw 1;

          ++param.last_index;
          return std::vector<parse_result>{new regex_node_()}; // TODO remove dummy node as it is useles
        }};
  auto not_word_boundary = parser{std::make_tuple('B'), [](regex_param& param){ // not \b
          if(param.regex.size() < param.consumed+1) // case when \B doesn't have a value after it - could cause a partial match
            throw 1;

          if(param.consumed - 2 == 0) // case when \B doesn't have a value before it - could cause a partial match
            throw 1;

          if(::isspace(param.regex[param.consumed - 3]) || ::isspace(param.regex[param.consumed + 1]))
            throw 1;

          ++param.last_index;
          return std::vector<parse_result>{new regex_node_()}; // TODO remove dummy node as it is useles
        }};
  auto backreference_parser = parser{std::make_tuple('1', '2', '3', '4', '5', '6', '7', '8', '9'),
        [](regex_param& param){
          std::size_t end = param.regex.find_first_not_of("0123456789", param.consumed);

          int digit = std::stoi(std::string(param.regex.substr(param.consumed-1, end)));

          if(digit > param.captured_groups.size() || param.captured_groups[digit - 1] == -1)
            throw 1; // captured group doesn't exist || capturing group while inside of it
          else
            param.consumed = end;

          ++param.last_index;
          return std::vector<parse_result>{new captured_group_reference_node_(param.captured_groups[digit - 1])};
        }};


  or_parser parser_escaped {
      std::make_tuple(form_feed,
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
      backreference_parser),
      [](regex_param& param){
          ++param.last_index;
          return std::vector<parse_result>{new literal_regex_node_(param.regex[param.consumed++])}; // escaped_literal_char
        }
    };

  or_parser escaped_or_literal {
      std::make_tuple(parser{std::make_tuple('\\'), [parser_escaped](regex_param& param){return parser_escaped(param);}}),
      [](regex_param& param){
          ++param.last_index;
          return std::vector<parse_result>{new literal_regex_node_(param.regex[param.consumed++])};
        }
    };

  or_parser parse_end_range_escaped {
      std::make_tuple(form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      hexadecimal_ascii_character_representation),
      [](regex_param& param){
          ++param.last_index;
          return std::vector<parse_result>{new literal_regex_node_(param.regex[param.consumed++])}; // escaped_literal_char (TODO should non valid escapes be removed? Throwing exception out of them...)
        }
    };

  or_parser end_range_escaped_or_literal {
      std::make_tuple(parser{std::make_tuple('\\'), [parse_end_range_escaped](regex_param& param){return parse_end_range_escaped(param);}}),
      [](regex_param& param){
          ++param.last_index;
          return std::vector<parse_result>{new literal_regex_node_(param.regex[param.consumed++])};
        }
    };

  and_parser range_parser {
      {},
      ']',
      [escaped_or_literal, end_range_escaped_or_literal](regex_param& param)
      {
        auto tmp_nodes = escaped_or_literal(param);
        if(tmp_nodes.empty())
        {
          /// TODO exception handling
          throw 1;
        }

        if(param.consumed+2 < param.regex.size() && param.regex[param.consumed] == '-' && param.regex[param.consumed+1] != ']')
        {
          literal_regex_node_* literal_node = static_cast<literal_regex_node_*>(tmp_nodes.back());

          ++param.consumed;

          if(param.consumed+1 >= param.regex.size())
          {
            /// TODO exception handling
            throw 1;
          }

          auto tmp = end_range_escaped_or_literal(param);
          if(tmp.empty())
          {
            /// TODO exception handling
            throw 1;
          }

          ++param.last_index;
          tmp_nodes = std::vector<parse_result>{new range_random_regex_node_(literal_node->getLiteral(),
                                                              static_cast<literal_regex_node_*>(tmp.back())->getLiteral())}; // TODO range_random_regex_node_ should take two regex_node elements and cast them to literal_regex_node_
        }

        return tmp_nodes;
      }
    };

  or_parser range_or_negated_range {
      std::make_tuple(parser{std::make_tuple('^'), [range_parser](regex_param& param){
          auto tmp = range_parser(param);

          param.last_index -= tmp.size();
          // TODO check if range parser returns an empty vector

          std::vector<regex_node_*> invert;
          std::vector<std::pair<char, char>> ranges;

          // convert
          for(auto& element : tmp)
          {
            if(auto lit = dynamic_cast<literal_regex_node_*>(element))
              ranges.emplace_back(lit->getLiteral(), lit->getLiteral());
            else if(auto range = dynamic_cast<range_random_regex_node_*>(element))
              ranges.emplace_back(range->get_from(), range->get_to());
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

          std::vector<std::size_t> lenghts;
          for(std::size_t i = 0; i<invert.size(); ++i)
          {
            lenghts.push_back(1);
          }

          ++param.consumed; // consume ]
          invert.insert(invert.begin(), new or_regex_node_(lenghts));
          param.last_index += invert.size();
          return invert;
        }}),
      [range_parser](regex_param& param){
          auto tmp_vec = range_parser(param);
          std::vector<std::size_t> lenghts;
          for(std::size_t i = 0; i<tmp_vec.size(); ++i)
          {
            lenghts.push_back(1);
          }

          ++param.last_index;
          tmp_vec.insert(tmp_vec.begin(), new or_regex_node_(lenghts)); // TODO check if range parser returns an empty vector
          
          ++param.consumed; // consume ]
          return tmp_vec;
        }
    };

  or_parser parser_base_type{
      std::make_tuple(
        parser{std::make_tuple('('), [](regex_param& param)
          {
            if(param.regex.size() == param.consumed + 1) // handle () case
            {
              if(param.regex[param.consumed] == ')')
              {
                ++param.consumed;
                ++param.last_index;
                return std::vector<parse_result>{new regex_node_()}; // empty
              }
              else
                throw 1; // not enough characters...
            }

            std::size_t capture_index = param.last_index;
            int index = -1;
            if(param.regex[param.consumed] == '?')
            {
              // (?:...) is a non capturing group
              if(param.regex.size() > param.consumed + 2 && param.regex[param.consumed+1] == ':')
              {
                param.consumed += 2; // discard matches everything enclosed as that's exactly what will be generated anyway

                if(param.regex[param.consumed] == ')') // handle (?:) case
                {
                  ++param.consumed;
                  ++param.last_index;
                  return std::vector<parse_result>{new regex_node_()}; // empty
                }
              }
              else
                throw 1; // not enough characters
            }
            else
            {
              param.captured_groups.push_back(-1);
              index = param.captured_groups.size() - 1;
            }

            auto nodes = parser_regex(param);
            if(param.regex.size() == param.consumed)
              throw 1; // missing closing bracket
            ++param.consumed;

            if(index != -1)
              param.captured_groups[index] = capture_index;

            return nodes;
          }}, // TODO, ')'},
        parser{std::make_tuple('['), [range_or_negated_range](regex_param& param){return range_or_negated_range(param);}},
        parser{std::make_tuple('\\'), [parser_escaped](regex_param& param){return parser_escaped(param);}},
        parser{std::make_tuple('.'), [](regex_param& param){
            ++param.last_index;
            return std::vector<parse_result>{new random_regex_node_()};
          }}
      ),
      [](regex_param& param){
          if(param.regex[param.consumed] == '$' && param.regex.size() == param.consumed + 1) // handle end of text symbol (TODO add random text generation if $ is missing at the end - check if OK)
          {
            ++param.consumed;
            ++param.last_index;
            return std::vector<parse_result>{new regex_node_()};
          }

          ++param.last_index;
          return std::vector<parse_result>{new literal_regex_node_(param.regex[param.consumed++])};
        }
    };

  return parser_base_type(param);
}
#include <iostream> // TODO delete
regex_template::regex_template(std::string_view regex)
{
  if(regex.size())
  {
    std::size_t consumed = (regex[0] == '^'); // set to 0 if regex doesn't start with start of text symbol (^) otherwise start with 1 (TODO add random text generation if ^ is missing at the beginning - check if OK)

    if(!consumed || regex.size() != 1)
    {
      regex_param param{regex};
      param.consumed = consumed;
      nodes_ = parser_regex(param);

      std::size_t i = 0;

#ifdef RANDOM_REGEX_DEBUG
      for(auto& node : nodes_)
        std::cout << node->name() << "   " << i++ << '\n';

      std::cout << '\n';
#endif

      /// TODO in some cases this is also true: regex.size() < consumed (check why???)
      if(param.regex.size() > param.consumed)
        // TODO throw invalid regex exception
        throw 1;
    }
  }
}

regex_template::~regex_template() = default;

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
  std::size_t len = 0;

  if(len < nodes_.size()) // TODO this if could be changed for dummy regex node
  {
    len += nodes_[len]->generate(nodes_, len, os, rand);
  }
}
