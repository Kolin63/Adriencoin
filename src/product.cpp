#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <filesystem>
#include <cassert>
#include <dpp/nlohmann/json.hpp>
#include "product.h"

// Returns an enum based on a string, and returns adr::Product::r_none if the string is invalid
adr::Product::ResultType adr::Product::getResultType(const std::string& str) {
    if (str == "all") return adr::Product::r_all;
    if (str == "one") return adr::Product::r_one;
    if (str == "customAll") return adr::Product::r_customAll;
    if (str == "customOne") return adr::Product::r_customOne;

    return adr::Product::r_none;
}

// Take a JSON object, "cost":{} or "result":{}, and return an Inventory or a String Vector of custom results
std::tuple<Inventory, std::vector<std::string>> jsonToInv(const nlohmann::json json) {
    Inventory inv{};
    std::vector<std::string> custom(0);

    // For every item that exists 
    for (std::size_t i{}; i < adr::Item::names.size(); ++i) {
        // If the JSON has this item, and it is an int
        try {
            if (json.at(adr::Item::names[i]).is_number_integer()) {
                // Set the passed Inventory's value to the JSON's value
                inv[i] = json[adr::Item::names[i]];
            }
        }
        catch ([[maybe_unused]] const nlohmann::json::out_of_range& e) {
        }
    }

    try {
        if (json.at("custom").is_array()) {
            for (std::string str : json.at("custom")) {
                custom.push_back(str);
            }
        }
    }
    catch ([[maybe_unused]] const nlohmann::json::out_of_range& e) {
    }
    
    return { inv, custom };
}

// Return false if error, true otherwise
bool adr::Product::parseJson()
{
    std::cout << "adr::Product::parseJson() called\n";

    using json = nlohmann::json;

    std::filesystem::path filepath{ "./../data/shop.json" };
    std::ifstream fs{ filepath };
    json data;

    try {
        data = json::parse(fs);
    }
    catch (const json::parse_error& e) {
        std::cerr << "adr::Product::parseJson() error: " << e.what() << '\n';
        fs.close();
        return false;
    }

    adr::Product::products.reserve(5);

    // Puts the JSON data into the vector for future access
    for (auto i : data["products"]) {
        bool noTimes{ false };
        try {
            if (i.at("noTimes").is_boolean()) {
                noTimes = i.at("noTimes");
            }
        }
        catch ([[maybe_unused]] const nlohmann::json::out_of_range& e) {}

        adr::Product::products.push_back({ i["name"], i["display"], i["desc"], i["joke"], i["picURL"], i["color"],
            std::get<Inventory>(jsonToInv(i["cost"])), getResultType(i["resultType"]), std::get<Inventory>(jsonToInv(i["result"])), 
            std::get<std::vector<std::string>>(jsonToInv(i["result"])), noTimes });
    }

    std::cout << "adr::Product::parseJson() finished\n";

    return true;
}

void adr::Product::addSlashCommands(dpp::cluster& bot, std::vector<dpp::slashcommand>& commandList)
{
    parseJson();


    dpp::slashcommand buy{ "buy", "Buy something", bot.me.id };
    for (const adr::Product& product : adr::Product::products) {
        dpp::command_option subcmd{ dpp::co_sub_command, product.name, product.joke };

        if (product.resultType != adr::Product::r_all && product.resultType != adr::Product::r_customAll && product.resultType != adr::Product::r_none) {
            dpp::command_option result{ dpp::co_string, "result", "result", true };

            for (std::size_t i{}; i < product.result.size(); ++i) {
                if (product.result[i] > 0)
                    result.add_choice(dpp::command_option_choice{ adr::Item::names[i], adr::Item::names[i] });
            }

            for (const std::string& str : product.customResult) {
                result.add_choice(dpp::command_option_choice{ str, str });
            }

            subcmd.add_option(result);
        }

        if (product.resultType != adr::Product::r_none && !product.noTimes)
            subcmd.add_option(dpp::command_option{ dpp::co_integer, "times", "the amount of times to buy this product", false });

        buy.add_option(subcmd);
    }

    commandList.push_back(buy);
}

