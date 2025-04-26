#include "attributes.h"
#include "emoji.h"
#include <dpp/nlohmann/json.hpp>
#include <ios>
#include <sstream>
#include <string>
#include <string_view>
#include <iostream>

void adr::attributes::save_json(nlohmann::json& data) const
{
    std::cout << "adr::attributes::save_json() called!\n";

    // Save attributes here

    std::cout << "adr::attributes::save_json() finished!\n";
}

///
/// try_load()
/// @brief A helper function that tries to load a JSON value, and handles
/// the error if the value was not found
/// @param data A const reference to the JSON object containing only
/// the attributes. The object should not be the entire player save,
/// only the attributes object
/// @param name The name of the variable in the JSON object
/// @param var The variable to store the value in
///
template <typename T>
void try_load(const nlohmann::json& data, std::string_view name, T& var)
{
    // Try to get the value, with bounds checking
    try
    {
        var = data.at("attributes").at(name);
    }
    // If there is an exception, catch it, zero-initialize the variable,
    // and send an error message.
    catch(const nlohmann::json::out_of_range& e)
    {
        var = T{};

        std::cerr << "attributes try_load() error for " << name << ": " 
            << e.what() << '\n';
    }
}

void adr::attributes::load_json(const nlohmann::json& data)
{
    std::cout << "adr::attributes::load_json() called!\n";

    // Load attributes here

    std::cout << "adr::attributes::load_json() called!\n";
}

///
/// condlist()
/// @brief A helper function that lists an attribute via a string stream if it
/// is not the default value.
/// @param ss The string stream to use
/// @param var The variable that is being listed
/// @param def The default value, which will hide the varaiable if they are
/// equal.
/// @param name The name of the variable, to be displayed in the list
/// @param emoji The emoji id for the emoji to show in the list
///
template <typename T>
void condlist(
        std::stringstream& ss, 
        const T& var, 
        const T& def, 
        std::string_view name, 
        adr::emoji_id emoji)
{
    if (var == def) {
        ss << adr::get_emoji(emoji) << ' ' << name << ": " << var << '\n';
    }
}

std::string adr::attributes::list() const
{
    // The string stream to put in all of the attribute names and values
    std::stringstream ss{};

    // Bool Alpha makes it print True and False rather than 1 and 0
    ss << std::boolalpha;

    return ss.str();
}
