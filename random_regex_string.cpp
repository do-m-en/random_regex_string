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


using regex_consumer_function = std::function<regex_node_* (std::experimental::string_view regex, std::size_t& consumed, std::vector<regex_node_*>& captured_groups, regex_node_* previous_node)>;


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
  regex_node_* operator()(std::experimental::string_view regex, std::size_t& consumed,
    std::vector<regex_node_*>& captured_groups, regex_node_* previous_node) const
  {
    regex_node_* node = nullptr;

    if(regex.size() > consumed)
    {
      for(auto start_token : start_tokens_)
      {
        if(regex[consumed] == start_token)
        {
          ++consumed;
          node = consumer_(regex, consumed, captured_groups, previous_node);

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
  regex_consumer_function consumer_;
};

struct and_parser
{
public:
  and_parser(std::initializer_list<parser> and_parsers, char terminator,
        regex_consumer_function else_parser = nullptr)
    : parsers_{and_parsers}, terminator_{terminator}, else_parser_{else_parser} {}

  // TODO this should be optional return value
  std::vector<regex_node_*> operator()(std::experimental::string_view regex, std::size_t& consumed,
    std::vector<regex_node_*>& captured_groups, regex_node_* previous_node) const
  {
    std::vector<regex_node_*> nodes;

    while(regex.size() > consumed && regex[consumed] != terminator_)
    {
      bool found = false;
      for(auto& p : parsers_)
      {
        //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
        auto node = p(regex, consumed, captured_groups, previous_node);
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
          nodes.push_back(else_parser_(regex, consumed, captured_groups, previous_node)); // TODO check if null is returned...
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

    if(regex.size() == consumed && regex[consumed-1] != terminator_)
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

  regex_node_* operator()(std::experimental::string_view regex, std::size_t& consumed,
    std::vector<regex_node_*>& captured_groups, regex_node_* previous_node) const
  {
    if(regex.size() == consumed)
      throw 1; // error should contain one more character

    for(auto& p : or_parsers_)
    {
      //if(auto node = p(regex, consumed); node) - gcc is missing P0305R1 support...
      auto node = p(regex, consumed, captured_groups, previous_node);
      if(node)
        return node;
    }

    if(else_parser_)
      return else_parser_(regex, consumed, captured_groups, previous_node);

    throw 1; // TODO throw no match found exception
  }
private:
  std::vector<parser> or_parsers_;
  regex_consumer_function else_parser_;
};

regex_node_* parser_term(std::experimental::string_view regex, std::size_t& consumed,
  std::vector<regex_node_*>& captured_groups, regex_node_* previous_node);
regex_node_* parser_factor(std::experimental::string_view regex, std::size_t& consumed,
  std::vector<regex_node_*>& captured_groups, regex_node_* previous_node);
regex_node_* parser_base(std::experimental::string_view regex, std::size_t& consumed,
  std::vector<regex_node_*>& captured_groups, regex_node_* previous_node);

// <regex> ::= <term> '|' <regex> | <term>
regex_node_* parser_regex(std::experimental::string_view regex, std::size_t& consumed,
  std::vector<regex_node_*>& captured_groups, regex_node_* previous_node)
{
  if(regex[consumed] == '|')
  {
    ++consumed;
  }

  regex_node_* node = parser_term(regex, consumed, captured_groups, previous_node);

  if(consumed < regex.size() && regex[consumed] == '|')
  {
    ++consumed; // consume |
    regex_node_* other = parser_regex(regex, consumed, captured_groups, previous_node);
    node = new or_regex_node_{node, other};
  }

  return node;
}

// <term> ::= { <factor> }
regex_node_* parser_term(std::experimental::string_view regex, std::size_t& consumed,
  std::vector<regex_node_*>& captured_groups, regex_node_* previous_node)
{
  regex_node_* node = parser_factor(regex, consumed, captured_groups, previous_node);

  while(consumed < regex.size() && regex[consumed] != ')'
    && regex[consumed] != '|')
  {
    regex_node_* next = parser_factor(regex, consumed, captured_groups, previous_node);
    node = new group_regex_node_(std::vector<regex_node_*>{node, next}); // TODO consider renaming it to sequence
  }

  return node;
}

// <factor> ::= <base> { '*' } | <base> { '+' } | <base> { '?' } | <base> { '{}' }
regex_node_* parser_factor(std::experimental::string_view regex, std::size_t& consumed,
  std::vector<regex_node_*>& captured_groups, regex_node_* previous_node)
{
  if(regex.size() == consumed)
  {
    return new regex_node_{}; // empty or
  }

  regex_node_* node = parser_base(regex, consumed, captured_groups, previous_node);

  auto repeat_range_zero = parser{'*', [node](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          if(consumed == 1 || (consumed == 2 && (regex[0] == '^' || regex[0] == '|')) || (regex[consumed - 1] == '|' && regex[consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          consumed = regex.find_first_not_of('*', consumed);
          return new repeat_range_regex_node_(node, 0);
        }};
  auto repeat_range_one = parser{'+', [node](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          if(consumed == 1 || (consumed == 2 && (regex[0] == '^' || regex[0] == '|')) || (regex[consumed - 1] == '|' && regex[consumed - 2] != '\\'))
            throw 1; // must be perceeded by a character that is not an anchor

          // ++ is posessive but for the sake of generation it doesn't make any
          // difference as it may always match one or more times...
          consumed = regex.find_first_not_of('+', consumed);
          return new repeat_range_regex_node_(node, 1);
        }};
  auto optional_item = parser{'?', [node](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          if(consumed == 1 || (consumed == 2 && (regex[0] == '^' || regex[0] == '|')) || (regex[consumed - 1] == '|' && regex[consumed - 2] != '\\'))
            throw 1; // star must be perceeded by a character that is not an anchor

          // ? is gready 0 or 1, ?? is lazy 0 or 1 matching as few times as possible
          // so for generation sake it doesn't make any difference as it may always
          // match 0 or 1 times
          consumed = regex.find_first_not_of('?', consumed);
          return new optional_regex_node_(node);
        }};

  repeat_range_zero(regex, consumed, captured_groups, previous_node);
  repeat_range_one(regex, consumed, captured_groups, previous_node);
  optional_item(regex, consumed, captured_groups, previous_node);

  auto digit_parser = [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
        std::size_t end = regex.find_first_not_of("0123456789", consumed); // TODO find in range 0-9 would be nice...

        if(end == std::experimental::string_view::npos || end == consumed)
          throw 1; // TODO exception

        int digit = std::stoi(regex.substr(consumed, end).to_string());

        if(digit < 0)
          throw 1; // TODO exception

        consumed = end;

        return digit;
      };

  if(consumed < regex.size() && regex[consumed] == '{')
  {
    ++consumed; // consume {
    std::size_t num = 0;
    if(consumed < regex.size())
    {
      if(regex[consumed] != ',')
        num = digit_parser(regex, consumed, captured_groups, previous_node);
      else
        /// TODO handle exception
        throw 1; // syntax x{,y} is not supported
    }
    else
      throw 1; // TODO exception - unexpected end of regex

    if(consumed < regex.size() && regex[consumed] == ',')
    {
      if(regex.size() > ++consumed)
      {
        if(regex[consumed] == '}')
          node = new repeat_range_regex_node_(node, num);
        else
          node = new repeat_range_regex_node_(node, num, digit_parser(regex, consumed, captured_groups, previous_node));
      }
      else
        /// TODO error handling
        throw 1;
    }
    else if(regex.size() > consumed && regex[consumed] == '}')
      node = new repeat_regex_node_(node, num);
    else
      /// TODO error handling
      throw 1;

    ++consumed; // consume }
  }

  return node;
}

// <base> ::= <char> | '\' <char> | '(' <regex> ')' | . | '[' <range> ']'
regex_node_* parser_base(std::experimental::string_view regex, std::size_t& consumed, std::vector<regex_node_*>& captured_groups, regex_node_* previous_node)
{
  auto any_non_whitespace_node = [](){
          return new or_regex_node_{new range_random_regex_node_{ascii_min, '\t' - 1},
                                    new range_random_regex_node_{'\r' + 1, ' ' - 1},
                                    new range_random_regex_node_{' ' + 1, ascii_max}};
        };

  auto form_feed = parser{'f', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_{'\f'};}};
  auto new_line = parser{'n', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_{'\n'};}};
  auto carriage_return = parser{'r', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_{'\r'};}};
  auto horizontal_tab = parser{'t', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_{'\t'};}};
  auto vertical_tab = parser{'v', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_{'\v'};}};
  auto any_digit = parser{'d', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new range_random_regex_node_{'0', '9'};}};
  auto null_character = parser{'0', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_{'\0'};}};
  auto any_non_digit = parser{'D', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          return new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                                    new range_random_regex_node_{'9' + 1, ascii_max}};
        }};
  auto any_whitespace = parser{'s', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new whitespace_regex_node_{};}};
  auto any_non_whitespace = parser{'S', [any_non_whitespace_node](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
       /* '\t' // tab: 9
          '\n' // newline: 10
          '\v' // vertical tab: 11
          '\f' // formfeed: 12
          '\r' // carriage return: 13
          ' ' // space: 32 */
          return any_non_whitespace_node();
        }};
  auto any_alphanum_or_underscore = parser{'w', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node) // any alphanumeric characters or _
        {
          return new or_regex_node_{new range_random_regex_node_{'a', 'z'},
                                    new range_random_regex_node_{'0', '9'},
                                    new literal_regex_node_{'_'}};
        }};
  auto any_not_alphanum_or_underscore = parser{'W', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          return new or_regex_node_{new range_random_regex_node_{ascii_min, '0' - 1},
                                    new range_random_regex_node_{'9' + 1, 'A' - 1},
                                    new range_random_regex_node_{'Z' + 1, '_' - 1},
                                    new range_random_regex_node_{'_' + 1, 'a' - 1},
                                    new range_random_regex_node_{'z' + 1, ascii_max}};
        }};
  auto hexadecimal_ascii_character_representation = parser{'x', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){ // \x00 to \x7F
          if(regex.size() < consumed+2 || !::isxdigit((int)regex[consumed]) || !::isxdigit((int)regex[consumed+1]))
            throw 1;

          int ascii_hex = std::stoi(std::string(&regex[consumed], 2), nullptr, 16);

          if(ascii_hex > ascii_max)
            throw 1;

          consumed += 2;

          return new literal_regex_node_(static_cast<char>(ascii_hex));
        }};
  auto word_boundary = parser{'b', [any_non_whitespace_node](auto regex, auto& consumed, auto& captured_groups, auto previous_node){ // not \b
          if(regex.size() < consumed+1) // case when \b doesn't have a value after it - not valid
            throw 1;

          if(consumed - 2 == 0) // case when \b doesn't have a value before it - not valid
            throw 1;

          if(!::isspace(regex[consumed - 3]) && !::isspace(regex[consumed + 1]))
            throw 1;

          return new regex_node_(); // TODO remove dummy node as it is useles
        }};
  auto not_word_boundary = parser{'B', [any_non_whitespace_node](auto regex, auto& consumed, auto& captured_groups, auto previous_node){ // not \b
          if(regex.size() < consumed+1) // case when \B doesn't have a value after it - could cause a partial match
            throw 1;

          if(consumed - 2 == 0) // case when \B doesn't have a value before it - could cause a partial match
            throw 1;

          if(::isspace(regex[consumed - 3]) || ::isspace(regex[consumed + 1]))
            throw 1;

          return new regex_node_(); // TODO remove dummy node as it is useles
        }};
  auto backreference_parser = parser{{'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          std::size_t end = regex.find_first_not_of("0123456789", consumed);

          int digit = std::stoi(regex.substr(consumed-1, end).to_string());

          if(digit > captured_groups.size() || captured_groups[digit - 1] == nullptr)
            throw 1; // captured group doesn't exist || capturing group while inside of it
          else
            consumed = end;

          return new captured_group_reference_node_(captured_groups[digit - 1]);
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
      [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_(regex[consumed++]);} // escaped_literal_char
    };

  or_parser escaped_or_literal {
      {{'\\', [parser_escaped](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return parser_escaped(regex, consumed, captured_groups, previous_node);}}},
      [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_(regex[consumed++]);}
    };

  or_parser parse_end_range_escaped {
      {form_feed,
      new_line,
      carriage_return,
      horizontal_tab,
      vertical_tab,
      hexadecimal_ascii_character_representation},
      [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_(regex[consumed++]);} // escaped_literal_char (TODO should non valid escapes be removed? Throwing exception out of them...)
    };

  or_parser end_range_escaped_or_literal {
      {{'\\', [parse_end_range_escaped](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return parse_end_range_escaped(regex, consumed, captured_groups, previous_node);}}},
      [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new literal_regex_node_(regex[consumed++]);}
    };

  and_parser range_parser {
      {},
      ']',
      [escaped_or_literal, end_range_escaped_or_literal](auto regex, auto& consumed, auto& captured_groups, auto previous_node)
      {
        auto tmp_node = escaped_or_literal(regex, consumed, captured_groups, previous_node);
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

          auto tmp = end_range_escaped_or_literal(regex, consumed, captured_groups, previous_node);
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
      {{'^', [range_parser](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          auto tmp = range_parser(regex, consumed, captured_groups, previous_node);
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

          ++consumed; // consume ]
          return new or_regex_node_(std::move(invert));
        }}},
      [range_parser](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          auto tmp = new or_regex_node_(range_parser(regex, consumed, captured_groups, previous_node)); // TODO check if range parser returns an empty vector
          ++consumed; // consume ]
          return tmp;
        }
    };

  or_parser parser_base_type{{
        {'(', [](auto regex, auto& consumed, auto& captured_groups, auto* previous_node)
          {
            if(regex.size() == consumed + 1) // handle () case
            {
              if(regex[consumed] == ')')
              {
                ++consumed;
                return new regex_node_(); // empty
              }
              else
                throw 1; // not enough characters...
            }

            int capture_index = -1;
            if(regex[consumed] == '?')
            {
              // (?:...) is a non capturing group
              if(regex.size() > consumed + 2 && regex[consumed+1] == ':')
              {
                consumed += 2; // discard matches everything enclosed as that's exactly what will be generated anyway

                if(regex[consumed] == ')') // handle (?:) case
                {
                  ++consumed;
                  return new regex_node_(); // empty
                }
              }
              else
                throw 1; // not enough characters
            }
            else
            {
              captured_groups.push_back(nullptr);
              capture_index = captured_groups.size() - 1;
            }

            auto node = parser_regex(regex, consumed, captured_groups, nullptr);
            if(regex.size() == consumed)
              throw 1; // missing closing bracket
            ++consumed;

            if(capture_index != -1)
              captured_groups[capture_index] = node;

            return node;
          }}, // TODO, ')'},
        {'[', [range_or_negated_range](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return range_or_negated_range(regex, consumed, captured_groups, previous_node);}},
        {'\\', [parser_escaped](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return parser_escaped(regex, consumed, captured_groups, previous_node);}},
        {'.', [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){return new random_regex_node_();}}
      },
      [](auto regex, auto& consumed, auto& captured_groups, auto previous_node){
          if(regex[consumed] == '$' && regex.size() == consumed + 1) // handle end of text symbol (TODO add random text generation if $ is missing at the end - check if OK)
          {
            ++consumed;
            return new regex_node_();
          }

          return static_cast<regex_node_*>(new literal_regex_node_(regex[consumed++]));
        }
    };

  return parser_base_type(regex, consumed, captured_groups, previous_node);
}

regex_template::regex_template(std::experimental::string_view regex)
{
  if(regex.size())
  {
    std::size_t consumed = (regex[0] == '^'); // set to 0 if regex doesn't start with start of text symbol (^) otherwise start with 1 (TODO add random text generation if ^ is missing at the beginning - check if OK)

    std::vector<regex_node_*> captured_groups;
    if(!consumed || regex.size() != 1)
    {
      root_node_.reset(parser_regex(regex, consumed, captured_groups, nullptr));

      /// TODO in some cases this is also true: regex.size() < consumed (check why???)
      if(regex.size() > consumed)
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
