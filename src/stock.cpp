#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <filesystem>
#include <fstream>
#include <utility>
#include <dpp/nlohmann/json.hpp>
#include "stock.h"
#include "util.h"
#include "Random.h"

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

dpp::message adr::Stock::getEmbed(std::string name)
{
    dpp::message msg{};

    if (name == "everything") {
        dpp::embed embed{};
        embed.set_title("Stocks")
            .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/paper.png")
            .set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/distantphone-wide.jpg")
            .set_description(dpp::utility::timestamp(std::time(0), dpp::utility::tf_long_date) + "\n");

        for (const adr::Stock& stock : adr::Stock::stocks) {
            embed.set_description(embed.description + "## " + stock.getName() + '\n' + getEmbed(stock.getName()).embeds[0].description + '\n');
        }

        embed.set_description(embed.description + "\n" + getEmbed("compact").content);

        msg.add_embed(embed);
        return msg;
    }
    else if (name == "compact") {
        msg.set_content("Day " + std::to_string(adr::Stock::getDay()) + " " + dpp::utility::timestamp(std::time(0), dpp::utility::tf_long_date) + "\n");

        for (const adr::Stock& stock : adr::Stock::stocks) {
            int diff{ stock.getValue() - stock.getHistory(1) };

            char c{};
            std::string emoji{};
            if (diff > 0) {
                c = '+';
                emoji = dpp::emoji{ "green_circle" }.get_mention();
            }
            else if (diff < 0) {
                c = '-';
                emoji = dpp::emoji{ "red_circle" }.get_mention();
            }
            else {
                c = '=';
                emoji = dpp::emoji{ "white_small_square" }.get_mention();
            }

            msg.set_content(msg.content + emoji + ' ' + stock.getName() + ": " 
                + "**" + std::to_string(stock.getValue()) + "**"
                + (c == '=' ? "\n" : " (" + std::string{ c } + std::to_string(std::abs(diff)) + " : " + std::to_string(stock.getHistory(1)) + " -> " + std::to_string(stock.getValue()) + ")\n"));
        }

        return msg;
    }
    else if (name == "graph") {
        // TODO
        return dpp::message{ "Coming soon" };
    }

    const adr::Stock::Id id{ adr::Stock::getId(name) };
    const adr::Stock& stock{ adr::Stock::getStock(id) };

    const int valueDiff{ stock.getValue() - stock.getHistory(1) };

    dpp::embed embed{};
    embed
        .set_title(name)
        .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/paper.png")
        .set_color(0xeeeeee)
        .set_description("**Value: " + std::to_string(stock.getValue()) + "** (" + (valueDiff >= 0 ? '+' : '-') + std::to_string(std::abs(valueDiff)) + ")\n"
            + "__Outstanding__: " + std::to_string(stock.getOutstanding()) + '\n'
            + "__Unissued__: " + std::to_string(stock.getUnissued()) + '\n'
            + "__Authorized__: " + std::to_string(stock.getOutstanding() + stock.getUnissued()) + '\n');

    msg.add_embed(embed);
    return msg;
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
        stock["history"] = obj.m_history;
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

        std::array<int, adr::Stock::historyLength> history{};
        try {
            for (std::size_t hi{}; hi < stock.at("history").size(); ++hi) {
                history[hi] = stock.at("history").at(hi);
            }
        }
        catch ([[maybe_unused]] const json::out_of_range& e) {
            history[0] = stock["value"].get<int>();
        }

        adr::Stock::stocks[i] = { 
            stock["name"].get<std::string>(), 
            static_cast<adr::Stock::Id>(i),
            stock["value"].get<int>(),
            stock["outstanding"].get<int>(),
            stock["unissued"].get<int>(),
            history
        };
    }

    fs.close();

    std::cout << "Done parsing stocks JSON.\n";
}

void adr::Stock::updateValue(adr::Job::Id id)
{
    // Job Id % Tier One Job Size essentially turns tier 2 jobs into their tier one counterparts
    // With tier one job numbers, we can get the corresponding stock
    adr::Stock::Id sid{ static_cast<adr::Stock::Id>(id % adr::Job::tierOneJobsSize) };

    constexpr int base{ 1 };
    constexpr int increase{ 2 };

    // Tier 1 Job - base adriencoin increase
    // Tier 2 Job - base + increase adriencoin
    // https://www.desmos.com/calculator/e06yhf1wsb
    const int value{ static_cast<int>(id) / static_cast<int>(adr::Job::tierOneJobsSize) * increase + base };

    // Change the value to the new value
    adr::Stock::stocks[sid].m_value = adr::Stock::stocks[sid].m_value + value;
}

void adr::Stock::changeOutstanding(int diff)
{
    m_outstanding += diff;
    m_unissued -= diff;
}

void adr::Stock::newDay()
{
    ++adr::Stock::day;

    std::cout << "Stock day now: " << adr::Stock::day << '\n';

    // Randomly change the values of every stock and edit the history
    // In this case, the percentages are in integer form because of limitations of Random::get()
    constexpr int lowerPercent{ 5 };
    constexpr int upperPercent{ 150 };

    for (adr::Stock& stock : adr::Stock::stocks) {
        // They are divided by 100 here to compensate for the change --->                     vvvvv
        stock.changeValue(static_cast<double>(Random::get<int>(lowerPercent, upperPercent)) / 100.0);

        // Shift the history
        adr::arrayShiftRight<int, adr::Stock::historyLength>(stock.m_history);
        stock.m_history[0] = stock.m_value;
    }
}

unsigned int adr::Stock::getDay()
{
    return adr::Stock::day;
}

void adr::Stock::addSlashCommands(dpp::cluster& bot, std::vector<dpp::slashcommand>& commandList)
{
    parseJSON();

    dpp::command_option stockopt{ dpp::co_string, "stock", "Which stock", true };

    for (const adr::Stock& i : adr::Stock::stocks) {
        stockopt.add_choice(dpp::command_option_choice{ i.m_name, i.m_name });
    }

    dpp::command_option amtopt{ dpp::co_integer, "amount", "How many stocks to buy", false };

    dpp::slashcommand cmd{ "stock", "Buy or sell a stock", bot.me.id };
    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "buy", "Buy a stock" }.add_option(stockopt).add_option(amtopt));
    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "sell", "Sell a stock" }.add_option(stockopt).add_option(amtopt));

    stockopt.add_choice(dpp::command_option_choice{ "everything", "everything" });
    stockopt.add_choice(dpp::command_option_choice{ "compact", "compact" });
    stockopt.add_choice(dpp::command_option_choice{ "graph", "graph" });
    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "view", "View a stock" }.add_option(stockopt));

    commandList.push_back(cmd);
}
