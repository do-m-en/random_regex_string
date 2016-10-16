#include "random_regex_string.hpp"

#include <ctime>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <random>
#include <algorithm>

using rand_regex::regex_node_;

// class or_regex_node_
using rand_regex::or_regex_node_;

or_regex_node_::or_regex_node_(regex_node_* first, regex_node_* second)
  : or_nodes_(2)
{
  or_nodes_[0] = first;
  or_nodes_[1] = second;
}

void or_regex_node_::append(regex_node_* or_node)
{
  or_nodes_.push_back(or_node);
}

void or_regex_node_::generate(std::stringstream& ss) const
{
  std::size_t index;

  if(or_nodes_.size() == 1)
  {
    index = 0;
  }
  else
  {
    srand(time(NULL)); // TODO add option to provide random seed
    index = rand()%or_nodes_.size();
  }

  or_nodes_[index]->generate(ss);
}

// class or_regex_node_
using rand_regex::range_random_regex_node_;

range_random_regex_node_::range_random_regex_node_(char from, char to)
  : from_(from)
  , to_(to)
{
  //
}

void range_random_regex_node_::generate(std::stringstream& ss) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<char> distribution(from_, to_);

  ss << distribution(generator);
}

// class range_regex_node_
using rand_regex::range_regex_node_;

range_regex_node_::range_regex_node_(std::vector<regex_node_*>&& nodes)
  : range_nodes_(std::forward<std::vector<regex_node_*>>(nodes))
{
  //
}

void range_regex_node_::generate(std::stringstream& ss) const
{
  srand(time(NULL)); // TODO add option to provide random seed
  range_nodes_[rand()%range_nodes_.size()]->generate(ss);
}

// class random_regex_node_
using rand_regex::random_regex_node_;

void random_regex_node_::generate(std::stringstream& ss) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<char> distribution(' ', '~');

  ss << distribution(generator);
}

// class repeat_zero_regex_node_
using rand_regex::repeat_zero_regex_node_;

repeat_zero_regex_node_::repeat_zero_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void repeat_zero_regex_node_::generate(std::stringstream& ss) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(0, 1024);

  for(int i = distribution(generator); i >= 0; --i)
  {
    node_->generate(ss);
  }
}

// class optional_regex_node_
using rand_regex::optional_regex_node_;

optional_regex_node_::optional_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void optional_regex_node_::generate(std::stringstream& ss) const
{
  srand(time(NULL)); // TODO add option to provide random seed
  if(rand()%2)
  {
    node_->generate(ss);
  }
}

// class repeat_one_regex_node_
using rand_regex::repeat_one_regex_node_;

repeat_one_regex_node_::repeat_one_regex_node_(regex_node_* node)
  : node_(node)
{
  //
}

void repeat_one_regex_node_::generate(std::stringstream& ss) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(1, 1024);

  for(int i = distribution(generator); i >= 0; --i)
  {
    node_->generate(ss);
  }
}

// class repeat_regex_node_
using rand_regex::repeat_regex_node_;

repeat_regex_node_::repeat_regex_node_(regex_node_* node, std::size_t repeat)
  : node_(node)
  , repeat_(repeat)
{
  //
}

void repeat_regex_node_::generate(std::stringstream& ss) const
{
  for(std::size_t i=0; i<repeat_; ++i)
  {
    node_->generate(ss);
  }
}

// class repeat_min_regex_node_
using rand_regex::repeat_min_regex_node_;

repeat_min_regex_node_::repeat_min_regex_node_(regex_node_* node, std::size_t min)
  : node_(node)
  , min_(min)
{
  //
}

void repeat_min_regex_node_::generate(std::stringstream& ss) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(min_, 1024);

  for(int i = distribution(generator); i > 0; --i)
  {
    node_->generate(ss);
  }
}

// class repeat_range_regex_node_
using rand_regex::repeat_range_regex_node_;

repeat_range_regex_node_::repeat_range_regex_node_(regex_node_* node, std::size_t min, std::size_t max)
  : node_(node)
  , min_(min)
  , max_(max)
{
  //
}

void repeat_range_regex_node_::generate(std::stringstream& ss) const
{
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(min_, max_);

  for(int i = distribution(generator); i > 0; --i)
  {
    node_->generate(ss);
  }
}

// class repeat_posessive_regex_node_
using rand_regex::repeat_posessive_regex_node_;

repeat_posessive_regex_node_::repeat_posessive_regex_node_(regex_node_* node, regex_node_* next_node)
  : node_(node)
  , next_node_(next_node)
{
  //
}

void repeat_posessive_regex_node_::generate(std::stringstream& ss) const
{
  std::stringstream tmp;

  node_->generate(tmp); // TODO with random character generation this should also be considered for regeneration

  if(next_node_)
  {
    std::string first(tmp.str());

    std::size_t retries = 1000;
    do
    {
      tmp.str("");
      ss.clear();
      next_node_->generate(tmp);
    }
    while(--retries > 0 && tmp.str() == first);

    if(tmp.str() == first)
    {
      // TODO error handling
      throw 1;
    }

    ss << first;
  }

  ss << tmp.rdbuf();
}

// class group_regex_node_
using rand_regex::group_regex_node_;

group_regex_node_::group_regex_node_(std::vector<regex_node_*>&& grouped_nodes)
  : grouped_nodes_(grouped_nodes)
{
  //
}

void group_regex_node_::generate(std::stringstream& ss) const
{
  for(const auto node : grouped_nodes_)
  {
    node->generate(ss);
  }
}

// class inclusive_regex_node_
using rand_regex::inclusive_regex_node_;

inclusive_regex_node_::inclusive_regex_node_()
{
}

void inclusive_regex_node_::generate(std::stringstream& ss) const
{
}

// class exclusive_regex_node_
using rand_regex::exclusive_regex_node_;

exclusive_regex_node_::exclusive_regex_node_()
{
}

void exclusive_regex_node_::generate(std::stringstream& ss) const
{
}

// class literal_regex_node_
using rand_regex::literal_regex_node_;

literal_regex_node_::literal_regex_node_(char literal)
  : literal_(literal)
{
  //
}

void literal_regex_node_::generate(std::stringstream& ss) const
{
  ss << literal_;
}

// function regexToRegexNodes
void regexToRegexNodes(const std::string& regex, std::size_t& consumed, std::vector<regex_node_*>& nodes)
{
  if(regex.size() == consumed)
  {
    return;
  }

  if(regex[consumed] == '.')
  {
    nodes.push_back(new random_regex_node_());
  }
  else if(regex[consumed] == '[')
  {
    std::vector<regex_node_*> child_nodes;
    ++consumed;
    while(regex.size() > consumed)
    {
      if(regex[consumed] == ']')
      {
        break;
      }
      else if((regex[consumed] == '-' && child_nodes.size()) && (consumed+1 < regex.size() && regex[consumed+1] != ']'))
      {
        literal_regex_node_* literal_node = dynamic_cast<literal_regex_node_*>(*(child_nodes.end()-1));

        if(literal_node)
        {
          ++consumed;
          *(child_nodes.end()-1) = new range_random_regex_node_(literal_node->getLiteral(), regex[consumed]);
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

    nodes.push_back(new range_regex_node_(std::move(child_nodes)));
  }
  else if(regex[consumed] == '*')
  {
    // if end -1 == repeat_one_regex_node_ -> error
    if(nodes.size() && dynamic_cast<repeat_one_regex_node_*>(*(nodes.end()-1)))
    {
      /// TODO throw exception
      throw 1;
    }
    // if end -1 == repeat_zero_regex_node_ -> error
    else if(nodes.size() && dynamic_cast<repeat_zero_regex_node_*>(*(nodes.end()-1)))
    {
      /// TODO throw exception
      throw 1;
    }

    *(nodes.end()-1) = new repeat_zero_regex_node_(*(nodes.end()-1));
  }
  else if(regex[consumed] == '+')
  {
    // if end -1 == repeat_one_regex_node_ -> error
    if(nodes.size() && dynamic_cast<repeat_one_regex_node_*>(*(nodes.end()-1)))
    {
      /// TODO throw exception
      throw 1;
    }
    // if end -1 == repeat_zero_regex_node_ -> posessive (takes as much as possible and gives nothing back)
    else if(repeat_zero_regex_node_* node = (nodes.size() ? dynamic_cast<repeat_zero_regex_node_*>(*(nodes.end()-1)) : nullptr))
    {
      regex_node_* next_node = nullptr;
      std::vector<regex_node_*> child_nodes;
      regexToRegexNodes(regex, ++consumed, child_nodes);
      --consumed; // consumed is incremented at the end of function

      if(child_nodes.size())
      {
        next_node = child_nodes[0];
      }

      *(nodes.end()-1) = new repeat_posessive_regex_node_(node, next_node); // TODO rethink as with posessive repeated group more than one next node may be affected
    }
    else
    {
      *(nodes.end()-1) = new repeat_one_regex_node_(*(nodes.end()-1));
    }
  }
  else if(regex[consumed] == '{')
  {
    ++consumed;
    std::size_t begin = consumed;
    while(regex.size() > consumed && isdigit(regex[consumed]))
    {
      ++consumed;
    }

    if(regex.size() > consumed && regex[consumed] == ',')
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
            *(nodes.end()-1) = new repeat_range_regex_node_(*(nodes.end()-1), min, std::stoi(number));
          }
          else
          {
            /// TODO handle exception
            throw 1;
          }
        }
        else
        {
          *(nodes.end()-1) = new repeat_min_regex_node_(*(nodes.end()-1), min);
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
        *(nodes.end()-1) = new repeat_regex_node_(*(nodes.end()-1), std::stoi(number));
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
  }
  else if(regex[consumed] == '?')
  {
    // if end -1 == optional_regex_node_ -> error
    if(nodes.size() && dynamic_cast<optional_regex_node_*>(*(nodes.end()-1)))
    {
      /// TODO throw exception
      throw 1;
    }

    *(nodes.end()-1) = new optional_regex_node_(*(nodes.end()-1));
  }
  else if(regex[consumed] == '|')
  {
    std::vector<regex_node_*> or_nodes;
    regexToRegexNodes(regex, ++consumed, or_nodes);
    --consumed; // consumed is incremented at the end of function

    if(or_nodes.size())
    {
      if(!nodes.size())
      {
        /// TODO exception handling - | can't be the first item (add this check to other items)
        throw std::runtime_error("or regex node | can't be first char!");
      }
      else if(or_regex_node_* or_node = dynamic_cast<or_regex_node_*>(*(nodes.end()-1)))
      {
        or_node->append(or_nodes[0]);
      }
      else
      {
        *(nodes.end()-1) = new or_regex_node_(*(nodes.end()-1), or_nodes[0]);
      }
    }
    else
    {
      /// TODO exception handling
      throw 1;
    }
  }
  else if(regex[consumed] == '(')
  {
    std::vector<regex_node_*> grouped_nodes;
    ++consumed;
    while(regex.size() > consumed && regex[consumed] != ')')
    {
      regexToRegexNodes(regex, consumed, grouped_nodes);
    }

    if(regex.size() == consumed && regex[consumed-1] != ')')
    {
      /// TODO exception handling
      throw 1;
    }
    // don't decrement consumed as we consume ) with that

    nodes.push_back(new group_regex_node_(std::move(grouped_nodes)));
  }
  else if(regex[consumed] == '\\')
  {
    // if last character of regex is \ -> error
    if(regex.size() == ++consumed)
    {
      /// TODO exception handling
      throw 1;
    }

    if(regex[consumed] == 'n') // new line
    {
      nodes.push_back(new literal_regex_node_('\n'));
    }
    else if(regex[consumed] == 'r') // carriage return
    {
      nodes.push_back(new literal_regex_node_('\r'));
    }
    else if(regex[consumed] == 't') // tab
    {
      nodes.push_back(new literal_regex_node_('\t'));
    }
    else if(regex[consumed] == 'p' || regex[consumed] == 'P')
    {
      bool inclusive = (regex[consumed] == 'p');

      ++consumed;
      if(regex[consumed] != '{')
      {
        /// TODO exception handling
        throw 1;
      }
      ++consumed;

      std::size_t found = regex.find('}', consumed);

      if(found!=std::string::npos && found != consumed +1 )
      {
        std::string type(regex.substr(consumed, found-consumed));

        if(type.size() < 3) // character category
        {
          if(type[0] == 'L')
          {
            ++consumed;
            if(type.size() == 1) // L - letter, any
            {
            }
            else
            {
              ++consumed;
              if(type[1] == 'u') // Lu - letter, upercase
              {
              }
              else if(type[1] == 'l') // Ll - letter, lowercase
              {
              }
              else if(type[1] == 't') // Lt - letter, titlecase
              {
              }
              else if(type[1] == 'm') // Lm - letter, modifier
              {
              }
              else if(type[1] == 'o') // Lo - letter, other
              {
              }
              else
              {
                /// TODO exception handling
                throw 1;
              }
            }
          }
          else if(type[0] == 'M')
          {
            if(type.size() == 1) // M - mark, any
            {
            }
            else if(type[1] == 'n') // Mn - mark, nonspacing
            {
            }
            else if(type[1] == 'c') // Mc - mark, spacing combining
            {
            }
            else if(type[1] == 'e') // Me - mark, enclosing
            {
            }
            else
            {
              /// TODO exception handling
              throw 1;
            }
          }
          else if(type[0] == 'N')
          {
            if(type.size() == 1) // N - number, any
            {
            }
            else if(type[1] == 'd') // Nd - number, decimal digit
            {
            }
            else if(type[1] == 'l') // Nd - number, letter
            {
            }
            else if(type[1] == 'o') // Nd - number, other
            {
            }
            else
            {
              /// TODO exception handling
              throw 1;
            }
          }
          else if(type[0] == 'P')
          {
            if(type.size() == 1) // P - punctuation, any
            {
            }
            else if(type[1] == 'c') // Pc - punctuation, connector
            {
            }
            else if(type[1] == 'd') // Pd - punctuation, dash
            {
            }
            else if(type[1] == 's') // Ps - punctuation, open
            {
            }
            else if(type[1] == 'e') // Pe - punctuation, close
            {
            }
            else if(type[1] == 'i') // Pi - punctuation, initian quote
            {
            }
            else if(type[1] == 'f') // Pf - punctuation, final quote
            {
            }
            else if(type[1] == 'o') // Po - punctuation, other
            {
            }
            else
            {
              /// TODO exception handling
              throw 1;
            }
          }
          else if(type[0] == 'S')
          {
            if(type.size() == 1) // S - symbol, any
            {
            }
            else if(type[1] == 'm') // Sm - symbol, math
            {
            }
            else if(type[1] == 'c') // Sc - symbol, currency
            {
            }
            else if(type[1] == 'k') // Sc - symbol, modifier
            {
            }
            else if(type[1] == 'o') // Sc - symbol, other
            {
            }
            else
            {
              /// TODO exception handling
              throw 1;
            }
          }
          else if(type[0] == 'Z')
          {
            if(type.size() == 1) // Z - separator, any
            {
            }
            else if(type[1] == 's') // Zs - separator, space
            {
            }
            else if(type[1] == 'l') // Zl - separator, line
            {
            }
            else if(type[1] == 'p') // Zp - separator, paragraph
            {
            }
            else
            {
              /// TODO exception handling
              throw 1;
            }
          }
          else if(type[0] == 'C')
          {
            /// TODO check if any group has to support Cs and Co... (probably yes :/)
            if(type.size() == 1) // C - other, any
            {
            }
            else if(type[1] == 'c') // Cc - other, control
            {
            }
            else if(type[1] == 'f') // Cf - other, format
            {
            }
/** TODO check this group if any characters overlap with others
            else if(type[1] == 's') // Cs - other, surrogate
            {
            }
TODO check this group if any characters overlap with others
            else if(type[1] == 'o') // Co - other, private use
            {
            }
0 characters in this group - not supported
            else if(type[1] == 'n') // Cn - other, not assigned
            {
            }*/
            else
            {
              /// TODO exception handling
              throw 1;
            }
          }
          else
          {
            /// TODO exception handling
            throw 1;
          }
        }
        else // character blocks
        {
          if(type == "BasicLatin")
          {
          }
          else if(type == "Latin-1Supplement")
          {
          }
          else if(type == "LatinExtended-A")
          {
          }
          else if(type == "LatinExtended-B")
          {
          }
          else if(type == "IPAExtensions")
          {
          }
          else if(type == "SpacingModifierLetters")
          {
          }
          else if(type == "CombiningDiacriticalMarks")
          {
          }
          else if(type == "Greek")
          {
          }
          else if(type == "Cyrillic")
          {
          }
          else if(type == "Armenian")
          {
          }
          else if(type == "Hebrew")
          {
          }
          else if(type == "Arabic")
          {
          }
          else if(type == "Syriac")
          {
          }
          else if(type == "Thaana")
          {
          }
          else if(type == "Devanagari")
          {
          }
          else if(type == "Bengali")
          {
          }
          else if(type == "Gurmukhi")
          {
          }
          else if(type == "Gujarati")
          {
          }
          else if(type == "Oriya")
          {
          }
          else if(type == "Tamil")
          {
          }
          else if(type == "Telugu")
          {
          }
          else if(type == "Kannada")
          {
          }
          else if(type == "Malayalam")
          {
          }
          else if(type == "Sinhala")
          {
          }
          else if(type == "Thai")
          {
          }
          else if(type == "Lao")
          {
          }
          else if(type == "Tibetan")
          {
          }
          else if(type == "Myanmar")
          {
          }
          else if(type == "Georgian")
          {
          }
          else if(type == "HangulJamo")
          {
          }
          else if(type == "Ethiopic")
          {
          }
          else if(type == "Cherokee")
          {
          }
          else if(type == "UnifiedCanadianAboriginalSyllabics")
          {
          }
          else if(type == "Ogham")
          {
          }
          else if(type == "Runic")
          {
          }
          else if(type == "Khmer")
          {
          }
          else if(type == "Mongolian")
          {
          }
          else if(type == "LatinExtendedAdditional")
          {
          }
          else if(type == "GreekExtended")
          {
          }
          else if(type == "GeneralPunctuation")
          {
          }
          else if(type == "SuperscriptsandSubscripts")
          {
          }
          else if(type == "CurrencySymbols")
          {
          }
          else if(type == "CombiningMarksforSymbols")
          {
          }
          else if(type == "LetterlikeSymbols")
          {
          }
          else if(type == "NumberForms")
          {
          }
          else if(type == "Arrows")
          {
          }
          else if(type == "MathematicalOperators")
          {
          }
          else if(type == "MiscellaneousTechnical")
          {
          }
          else if(type == "ControlPictures")
          {
          }
          else if(type == "OpticalCharacterRecognition")
          {
          }
          else if(type == "EnclosedAlphanumerics")
          {
          }
          else if(type == "BoxDrawing")
          {
          }
          else if(type == "BlockElements")
          {
          }
          else if(type == "GeometricShapes")
          {
          }
          else if(type == "MiscellaneousSymbols")
          {
          }
          else if(type == "Dingbats")
          {
          }
          else if(type == "BraillePatterns")
          {
          }
          else if(type == "CJKRadicalsSupplement")
          {
          }
          else if(type == "KangxiRadicals")
          {
          }
          else if(type == "IdeographicDescriptionCharacters")
          {
          }
          else if(type == "CJKSymbolsandPunctuation")
          {
          }
          else if(type == "Hiragana")
          {
          }
          else if(type == "Katakana")
          {
          }
          else if(type == "Bopomofo")
          {
          }
          else if(type == "HangulCompatibilityJamo")
          {
          }
          else if(type == "Kanbun")
          {
          }
          else if(type == "BopomofoExtended")
          {
          }
          else if(type == "EnclosedCJKLettersandMonths")
          {
          }
          else if(type == "CJKCompatibility")
          {
          }
          else if(type == "CJKUnifiedIdeographsExtensionA")
          {
          }
          else if(type == "CJKUnifiedIdeographs")
          {
          }
          else if(type == "YiSyllables")
          {
          }
          else if(type == "YiRadicals")
          {
          }
          else if(type == "HangulSyllables")
          {
          }
          else if(type == "HighSurrogates")
          {
          }
          else if(type == "HighPrivateUseSurrogates")
          {
          }
          else if(type == "LowSurrogates")
          {
          }
          else if(type == "PrivateUse")
          {
          }
          else if(type == "CJKCompatibilityIdeographs")
          {
          }
          else if(type == "AlphabeticPresentationForms")
          {
          }
          else if(type == "ArabicPresentationForms-A")
          {
          }
          else if(type == "CombiningHalfMarks")
          {
          }
          else if(type == "CJKCompatibilityForms")
          {
          }
          else if(type == "SmallFormVariants")
          {
          }
          else if(type == "ArabicPresentationForms-B")
          {
          }
          else if(type == "Specials")
          {
          }
          else if(type == "HalfwidthandFullwidthForms")
          {
          }
          else if(type == "Specials") /// WTF??? twice? (from to on two points?)
          {
          }
          else if(type == "OldItalic")
          {
          }
          else if(type == "Gothic")
          {
          }
          else if(type == "Deseret")
          {
          }
          else if(type == "ByzantineMusicalSymbols")
          {
          }
          else if(type == "MusicalSymbols")
          {
          }
          else if(type == "MathematicalAlphanumericSymbols")
          {
          }
          else if(type == "CJKUnifiedIdeographsExtensionB")
          {
          }
          else if(type == "CJKCompatibilityIdeographsSupplement")
          {
          }
          else if(type == "Tags")
          {
          }
          else if(type == "PrivateUse")
          {
          }
          else
          {
            /// TODO exception handling
            throw 1;
          }

          consumed += type.size();
        }
      }
      else
      {
        /// TODO exception handling
        throw 1;
      }


      if(inclusive)
      {
        nodes.push_back(new inclusive_regex_node_());
      }
      else
      {
        nodes.push_back(new exclusive_regex_node_());
      }
    }
    else if(regex[consumed] == '\\' || regex[consumed] == '|' || regex[consumed] == '.' || regex[consumed] == '-'
        || regex[consumed] == '^' || regex[consumed] == '?' || regex[consumed] == '*' || regex[consumed] == '+'
        || regex[consumed] == '{' || regex[consumed] == '}' || regex[consumed] == '(' || regex[consumed] == ')'
        || regex[consumed] == '[' || regex[consumed] == ']')
    {
      nodes.push_back(new literal_regex_node_(regex[consumed]));
    }
    else
    {
      /// TODO exception handling
      throw 1;
    }
  }
  else
  {
    nodes.push_back(new literal_regex_node_(regex[consumed]));
  }

  ++consumed;
}

// class regex_template
using rand_regex::regex_template;

regex_template::regex_template(const std::string& regex)
{
  std::size_t consumed = 0;
  std::unique_ptr<regex_node_> tmp;

  do
  {
    regexToRegexNodes(regex, consumed, nodes_);
  }
  while(consumed < regex.size());
}

std::string regex_template::generate() const
{
  std::stringstream ss;

  for(auto generator : nodes_)
  {
    generator->generate(ss);
  }

  return ss.str();
}
