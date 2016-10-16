#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>

namespace {

// input promises:
// unicode size() > 2
// unicode format: U+XX...X where X is replaced by hex value and ... means multiple hex values
unsigned long get_hex_part(std::string line, std::string& unicode)
{
  std::istringstream issTmp(line);
  std::string token;
  std::vector<std::string> tokens;
  while(std::getline(issTmp, token, '\t'))
  {
      tokens.push_back(token);
  }

  tokens[2].erase(std::remove_if(tokens[2].begin(), tokens[2].end(), [](char c){ 
                  return std::isspace(static_cast<unsigned char>(c));
               }), tokens[2].end());

  unicode = tokens[0];

  unsigned long value;
  std::istringstream iss("0x" + tokens[2]);
  iss >> std::hex >> value;

  return value;
}

} // namespace

int main()
{
  std::ifstream infile("in.txt");
  std::ofstream outfile("out.txt");
  int result = 0;
  std::string line;
  std::regex expression(R"(U\+[0-9A-Fa-f]+\t.*\t.*$)");

  if(std::getline(infile, line))
  {
    if(std::regex_match(line, expression))
    {
      std::string prev_line(line);
      std::string unicode;
      std::string prev_unicode;
      unsigned long prev_hex_part = get_hex_part(line, unicode);
      std::size_t cntr = 0;
      outfile << "(" << unicode << ") 0x" << std::hex << prev_hex_part;

      while(std::getline(infile, line))
      {
        if(std::regex_match(line, expression))
        {
          ++cntr;
          unsigned long hex_part = get_hex_part(line, unicode);

          if(prev_hex_part + 1 != hex_part)
          {
            if(cntr > 1)
            {
              outfile << " - (" << prev_unicode << ") 0x" << std::hex << prev_hex_part;
            }

            outfile << ", (" << unicode << ") 0x" << std::hex << hex_part;
            cntr = 0;
          }

          prev_line = line;
          prev_hex_part = hex_part;
          prev_unicode = unicode;
        }
        else
        {
          std::cerr << "Invalid input file format! Line: " << cntr << std::endl;
          result = 1;
          break;
        }
      }

      if(cntr != 0)
      {
        outfile << " - (" << prev_unicode << ") 0x" << std::hex << prev_hex_part;
      }
    }
    else
    {
      std::cerr << "Invalid input file format!" << std::endl;
      result = 1;
    }
  }

  return result;
}
