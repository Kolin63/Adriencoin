#include "attributes.h"
#include "emoji.h"
#include <dpp/nlohmann/json.hpp>
#include <ios>
#include <sstream>
#include <string>
#include <string_view>
#include <iostream>

///
/// save()
/// @brief A helper function that saves an attribute
/// @param data A const reference to the JSON object containing only
/// the attributes. The object should be the entire player save, not
/// only the attributes object
/// @param atrib The attribute to save
///
template <typename T>
void save(nlohmann::json& data, const adr::attributes::atrib<T>& atrib)
{
    data["attributes"][atrib.nam] = atrib.val;
}

void adr::attributes::save_json(nlohmann::json& data) const
{
    std::cout << "adr::attributes::save_json() called!\n";

    // Save attributes here
    save(data, example);
    save(data, axample);

    std::cout << "adr::attributes::save_json() finished!\n";
}

///
/// try_load()
/// @brief A helper function that tries to load a JSON value, and handles
/// the error if the value was not found
/// @param data A const reference to the JSON object containing only
/// the attributes. The object should be the entire player save, not
/// only the attributes object
/// @param name The name of the variable in the JSON object
/// @param var The variable to store the value in
///
template <typename T>
void try_load(const nlohmann::json& data, adr::attributes::atrib<T>& atrib)
{
    // Try to get the value, with bounds checking
    try
    {
         atrib.val = data.at("attributes").at(atrib.nam);
    }
    // If there is an exception, catch it, zero-initialize the variable,
    // and send an error message.
    catch(const nlohmann::json::out_of_range& e)
    {
        atrib.val = atrib.def;

        std::cerr << "attributes try_load() error for " << atrib.nam << ": " 
            << e.what() << '\n';
    }
}

void adr::attributes::load_json(const nlohmann::json& data)
{
    std::cout << "adr::attributes::load_json() called!\n";

    // Load attributes here
    try_load(data, example);
    try_load(data, axample);

    std::cout << "adr::attributes::load_json() finished!\n";
}

///
/// condlist()
/// @brief A helper function that lists an attribute via a string stream if it
/// is not the default value.
/// @param ss The string stream to use
/// @param a The atribute struct
///
template <typename T>
void condlist(std::stringstream& ss, const adr::attributes::atrib<T>& a)
{
    if (a.val != a.def) {
        ss << adr::get_emoji(a.emo) << ' ' << a.nam << ": " << a.val << '\n';
    }
}

std::string adr::attributes::list() const
{
    // The string stream to put in all of the attribute names and values
    std::stringstream ss{};

    // Bool Alpha makes it print True and False rather than 1 and 0
    ss << std::boolalpha;

    // List attributes here
    condlist(ss, example);
    condlist(ss, axample);

    return ss.str();
}
