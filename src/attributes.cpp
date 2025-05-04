#include "attributes.h"
#include "emoji.h"
#include <dpp/nlohmann/json.hpp>
#include <ios>
#include <sstream>
#include <string>
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

// void condlist(std::stringstream& ss, const adr::attributes::atrib<bool>& a)
// {
//     if (a.val != a.def) {
//         ss << std::boolalpha;
//         ss << adr::get_emoji(a.emo) << ' ' << a.nam << ": " << a.val << '\n';
//         ss << std::noboolalpha;
//    }
// }

// ============================================================================
//
// To add new attributes, put them in the following three functions:
//
// ============================================================================

void adr::attributes::save_json(nlohmann::json& data) const
{
    std::cout << "adr::attributes::save_json() called!\n";

    // Save attributes here
    // save(data, example);
    save(data, bonzo_love);
    save(data, bonzo_can_use);
    save(data, wither_shield);
    save(data, shadow_warp);
    save(data, implosion);

    std::cout << "adr::attributes::save_json() finished!\n";
}

void adr::attributes::load_json(const nlohmann::json& data)
{
    std::cout << "adr::attributes::load_json() called!\n";

    // Load attributes here
    // try_load(data, example);
    try_load(data, bonzo_love);
    try_load(data, bonzo_can_use);
    try_load(data, wither_shield);
    try_load(data, shadow_warp);
    try_load(data, implosion);

    std::cout << "adr::attributes::load_json() finished!\n";
}

std::string adr::attributes::list() const
{
    // The string stream to put in all of the attribute names and values
    std::stringstream ss{};

    // Bool Alpha prints True / False, not 1 / 0
    ss << std::boolalpha;

    // List attributes here
    // condlist(ss, example);
    condlist(ss, bonzo_love);
    condlist(ss, bonzo_can_use);
    condlist(ss, wither_shield);
    condlist(ss, shadow_warp);
    condlist(ss, implosion);

    return ss.str();
}
