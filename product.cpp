#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <filesystem>
#include <cassert>
#include <future>
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

// Return false if error, true otherwise
bool adr::Product::parseJson()
{
    using json = nlohmann::json;

    std::filesystem::path filepath{ "data/shop.json" };
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

    /* Example structure:
    {
        "products": 
        [
            {
                "name": "gaydrien",
                "desc": "",
                "joke": "funny joke",
                "picURL": "https....",
                "color": 0xFFFFFF,
                "cost": 
                [
                    ... 
                ],
                "resultType": "all",
                "result":
                [
                    ...
                ]
            }
        ]
    }
    */

    adr::Product::products.reserve(5);

    // Puts the JSON data into the vector for future access
    for (auto i : data["products"]) {
        std::cout << i["name"] << '\n';
        adr::Product::products.push_back({ i["name"], i["desc"], i["joke"], i["picURL"], i["color"], 
            i["cost"], getResultType(i["resultType"]), i["result"] });
    }

    return true;
}

void adr::Product::addSlashCommands(dpp::cluster& bot, std::vector<dpp::slashcommand>& commandList)
{
    parseJson();


    dpp::slashcommand buy{ "buy", "Buy something", bot.me.id };
    for (const adr::Product& product : adr::Product::products) {
        dpp::command_option subcmd{ dpp::co_sub_command, product.name, product.joke };

        if (product.resultType != adr::Product::r_all && product.resultType != adr::Product::r_none) {
            dpp::command_option result{ dpp::co_string, "result", "result", true };

            for (std::size_t i{}; i < product.result.size(); ++i) {
                if (product.result[i] > 0)
                    result.add_choice(dpp::command_option_choice{ adr::Item::names[i], adr::Item::names[i] });
            }

            subcmd.add_option(result);

            subcmd.add_option(dpp::command_option{ dpp::co_integer, "times", "the amount of times to buy this product", false });
        }

        buy.add_option(subcmd);
    }

    commandList.push_back(buy);
}

