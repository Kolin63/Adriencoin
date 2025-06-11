#include "mayor.h"
#include <fstream> 
#include <iostream>
#include <dpp/nlohmann/json.hpp>

void adr::Mayor::loadJSON() {
  using json = nlohmann::json;

  std::ifstream fs{ "./../data/mayors.json" };  

  json data{};

  try {
    data = json::parse(fs);
  }
  catch (const json::parse_error& e) {
    std::cerr << "adr::Mayor::loadJSON parse error: " << e.what() << '\n';
    return;
  }
}
