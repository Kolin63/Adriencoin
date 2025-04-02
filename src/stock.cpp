#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <filesystem>
#include <fstream>
#include <utility>
#include <dpp/nlohmann/json.hpp>
#include "stock.h"

std::filesystem::path path{ "data/stock.json" };

namespace adr
{
    unsigned int adr::Stock::day{ 0 };
    std::array<adr::Stock, adr::Stock::MAX> Stock::stocks{};
}

adr::Stock::Id adr::Stock::getId(const std::string& str)
{
    for (const adr::Stock& stock : adr::Stock::stocks) {
        if (stock.m_name == str) {
            return stock.m_id;
        }
    }

    std::cerr << "getId(string) Error: stock named " << str << " not found, returning jerry's carrots\n";
    return adr::Stock::jerrys_carrots;
}

adr::Stock& adr::Stock::getStock(Id id)
{
    return adr::Stock::stocks[id];
}

adr::Stock& adr::Stock::getStock(const std::string& str)
{
    for (adr::Stock& stock : adr::Stock::stocks) {
        if (stock.m_name == str) {
            return stock;
        }
    }

    std::cerr << "getStock(string) Error: stock named " << str << " not found, returning jerry's carrots\n";
    return adr::Stock::stocks[0];
}

void adr::Stock::saveJSON()
{
    std::cout << "Saving stocks JSON...\n";

    using json = nlohmann::json;

    json data;
    data["day"] = adr::Stock::day;

    for (std::size_t i{}; i < adr::Stock::stocks.size(); ++i) {
        json& stock{ data["stocks"][i] };
        const Stock& obj{ adr::Stock::stocks[i] };
        stock["name"] = obj.m_name;
        stock["value"] = obj.m_value;
        stock["outstanding"] = obj.m_outstanding;
        stock["unissued"] = obj.m_unissued;
    }

    std::ofstream fs{ path };
    fs << std::setw(4) << data << std::endl;
    fs.close();

    std::cout << "Done saving stocks JSON!\n";
}

void adr::Stock::parseJSON()
{
    using json = nlohmann::json;

    std::ifstream fs{ path };
    const json data{ json::parse(fs) };

    adr::Stock::day = data["day"];

    for (std::size_t i{}; i < adr::Stock::stocks.size(); ++i) {
        const json& stock{ data["stocks"][i] };
        adr::Stock::stocks[i] = { 
            stock["name"].get<std::string>(), 
            static_cast<adr::Stock::Id>(i),
            stock["value"].get<int>(),
            stock["outstanding"].get<int>(),
            stock["unissued"].get<int>()
        };
    }

    fs.close();

    std::cout << "Done parsing stocks JSON.\n";
}

void adr::Stock::updateValue(Id id, int amount, int mux)
{
    // The new value is changed by the amount bought * mux (default 5)
    // Cannot be less than 0
    adr::Stock::stocks[id].m_value = std::max(
        0,
        adr::Stock::stocks[id].m_value + (amount * mux)
    );
}

void adr::Stock::updateValue(adr::Job::Id id)
{
    // Job Id % Tier One Job Size essentially turns tier 2 jobs into their tier one counterparts
    // With tier one job numbers, we can get the corresponding stock
    adr::Stock::Id sid{ static_cast<adr::Stock::Id>(id % adr::Job::tierOneJobsSize) };

    // Increase the value by 2
    adr::Stock::updateValue(sid, 1, 2);
}

void adr::Stock::changeValue(Id id, int diff)
{
    adr::Stock::stocks[id].m_value += diff;
}

int adr::Stock::getValue(Id id)
{
    return adr::Stock::stocks[id].m_value;
}

void adr::Stock::newDay()
{
    ++adr::Stock::day;
}

unsigned int adr::Stock::getDay()
{
    return adr::Stock::day;
}

void adr::Stock::addSlashCommands(dpp::cluster& bot, std::vector<dpp::slashcommand>& commandList)
{
    parseJSON();

    dpp::slashcommand cmd{ "stock", "Buy or sell a stock", bot.me.id };
    cmd.add_option(dpp::command_option{ dpp::co_string, "action", "Buy or sell?", true }
        .add_choice(dpp::command_option_choice{ "buy", "buy" })
        .add_choice(dpp::command_option_choice{ "sell", "sell" }));

    dpp::command_option stockopt{ dpp::co_string, "stock", "Which stock to buy or sell", true };

    for (const adr::Stock& i : adr::Stock::stocks) {
        stockopt.add_choice(dpp::command_option_choice{ i.m_name, i.m_name });
    }

    cmd.add_option(stockopt);

    cmd.add_option(dpp::command_option{ dpp::co_integer, "amount", "How many stocks to buy", false });

    commandList.push_back(cmd);
}
