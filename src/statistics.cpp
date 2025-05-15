#include "statistics.h"
#include <iostream>

void save(nlohmann::json& data, const adr::Statistics::Stat& stat)
{
  data["stats"][stat.name] = stat.value;
}

void try_load(const nlohmann::json& data, adr::Statistics::Stat& stat)
{
  // Try to get the value, with bounds checking
  try
  {
    stat.value = data.at("stats").at(stat.name);
  }
  // If there is an exception, catch it, zero-initialize the variable,
  // and send an error message.
  catch(const nlohmann::json::out_of_range& e)
  {
    stat.value = stat.defaultValue;

    std::cerr << "attributes try_load() error for " << stat.name << ": " 
      << e.what() << '\n';
  }
}

void condlist(std::stringstream& ss, const adr::Statistics::Stat& stat)
{
    if (stat.value != stat.defaultValue) {
        ss << adr::get_emoji(stat.emoji) << ' ' << stat.name << ": " 
          << stat.value << '\n';
    }
}

// ============================================================================
//
// To add new attributes, put them in the following three functions:
//
// ============================================================================

void adr::Statistics::saveJSON(nlohmann::json& data) const
{
    std::cout << "adr::Statistics::saveJSON() called!\n";

    // Save attributes here
    // save(data, example);
    save(data, timesWorked);
    save(data, timesInStore);
    save(data, bonzo);
    save(data, scarf);
    save(data, theProfessor);
    save(data, thorn);
    save(data, livid);
    save(data, sadan);
    save(data, necron);

    std::cout << "adr::Statistics::saveJSON() finished!\n";
}

void adr::Statistics::loadJSON(const nlohmann::json& data)
{
    std::cout << "adr::Statistics::loadJSON() called!\n";

    // Load attributes here
    // try_load(data, example);
    try_load(data, timesWorked);
    try_load(data, timesInStore);
    try_load(data, bonzo);
    try_load(data, scarf);
    try_load(data, theProfessor);
    try_load(data, thorn);
    try_load(data, livid);
    try_load(data, sadan);
    try_load(data, necron);

    std::cout << "adr::Statistics::loadJSON() finished!\n";
}

std::string adr::Statistics::list() const
{
    // The string stream to put in all of the attribute names and values
    std::stringstream ss{};

    // Bool Alpha prints True / False, not 1 / 0
    ss << std::boolalpha;

    // List attributes here
    // condlist(ss, example);
    condlist(ss, timesWorked);
    condlist(ss, timesInStore);
    condlist(ss, bonzo);
    condlist(ss, scarf);
    condlist(ss, theProfessor);
    condlist(ss, thorn);
    condlist(ss, livid);
    condlist(ss, sadan);
    condlist(ss, necron);

    return ss.str();
}
