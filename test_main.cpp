#include <iostream>
#include <string>
#include <regex>
#include "unicode/blocks.hpp" // TODO this should probbably be moved to random_regex_string as base? or not as ascii could also be supported...
#include "random_regex_string.hpp"

namespace {
  void test_regex_ECMAScript(const std::string& regex)
  {
    using rand_regex::regex_template;

    regex_template generator(regex);
    std::cout << "------------------------ parsed -------------------------\n";
    std::stringstream ss;
    generator.generate(ss);

    std::string result(ss.str());
    std::regex rx(regex);

    std::cout << (std::regex_match(result, rx) ? "match: '" :
      "+++++++++++++++++++++++++++ERROR: '") << regex << "'  '" << result << "'\n";
  }
}

int main()
{
  test_regex_ECMAScript("a");
  test_regex_ECMAScript("a|f");
  test_regex_ECMAScript("a|f|g");
  test_regex_ECMAScript("gbc.*");
  test_regex_ECMAScript("gbc.*****************"); // oddly enough this id valid even though it means the same as *
  test_regex_ECMAScript("gbc+++++++++++(cd)"); // oddly enough this is valid even though it means the same as ++
  test_regex_ECMAScript("gbc.*+abc");
  test_regex_ECMAScript("(abc)*+abc");
  test_regex_ECMAScript("a|f|gbc.*+abc");
  test_regex_ECMAScript("(ab)|(cde)|(fg(hi|(jk)))a");
  test_regex_ECMAScript("(ab)*|(cde)|(fg(hi|(jk)))a");
  test_regex_ECMAScript("ab?c?");
  test_regex_ECMAScript("a??b");
  test_regex_ECMAScript("a???b"); // oddly enough this is valid even though it means the same as ??
  test_regex_ECMAScript("ab\\?c\\\\\\?");
  test_regex_ECMAScript("(ab){2}(cd){3,5}");
  test_regex_ECMAScript("(ab){2}(cd){3,5}e{2,}");
  test_regex_ECMAScript("[abd-x---0-9--]*");
  test_regex_ECMAScript("[a-c][abd][a-bcd-e0-8][---][--][-a-]");
  test_regex_ECMAScript("[0-9]{3}-\\s[0-9]{2}-[0-9]{4}");
  test_regex_ECMAScript("\\^*");
  test_regex_ECMAScript("[\\^]*");
  test_regex_ECMAScript("[\\n-\\^]*");
  test_regex_ECMAScript("\\f\\{");
//  test_regex_ECMAScript("[^a-c]*");
  // TODO add negative cases... TODO also check if they realy are invalid
  // []
  // [z-a]

//  test_regex_ECMAScript("upper\\p{Lu}case");
}
