#include <dpp/appcommand.h>
#include <dpp/message.h>
#include <string>
#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <filesystem>
#include <fstream>
#include <cmath>
#include <dpp/nlohmann/json.hpp>
#include "stock.h"
#include "util.h"
#include "Random.h"
#include "graph/graph.h"

std::filesystem::path path{ "./../data/stock.json" };

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

dpp::message adr::Stock::getGraph(std::string name, std::int64_t graphHistoryLength)
{
    // Need to fix the bugs
    return dpp::message{ "Coming Soon\nhttps://tenor.com/view/turtle-friends-funny-dog-slow-gif-13962148" }.set_flags(dpp::m_ephemeral);

    kolin::graph::dataset data;

    if (graphHistoryLength == 0) graphHistoryLength = 10;
    if (graphHistoryLength > adr::Stock::day + 1) graphHistoryLength = adr::Stock::day + 1;

    constexpr int width{ 10 };
    constexpr int height{ 28 };

    data.reserve(graphHistoryLength);

    int minY{ adr::Stock::getStock(name).getValue() };
    int maxY{ adr::Stock::getStock(name).getValue() };

    for (std::int64_t i{}; i < graphHistoryLength; ++i) {
        kolin::graph::point p;

        p.first = adr::Stock::day - i;
        p.second = adr::Stock::getStock(name).getHistory(i);

        data.push_back(p);

        if (p.second > maxY) maxY = p.second;
        if (p.second < minY) minY = p.second;
    }

    // We don't want max and min to be equal,
    // otherwise it would have a height of 0
    if (maxY == minY) {
        --minY;
        ++maxY;
    }

    // X Interval is Graph History Length / Width
    // If the GHL is bigger than the Width, the interval should be bigger
    int xInt{ static_cast<int>(std::floor(static_cast<double>(graphHistoryLength) / width)) };
    int yInt{ static_cast<int>(std::floor(static_cast<double>(maxY - minY) / height)) };

    if (xInt <= 0) xInt = 0;
    if (yInt <= 0) yInt = 0;

    std::cout << "Getting a stock graph of " << name << " length of " << graphHistoryLength
        << " width/height: " << width << '/' << height << " x/y interval: " << xInt << '/' << yInt 
        << " min / max: " << minY << '/' << maxY << " day: " << adr::Stock::day << '\n';

    try {
        const std::string str{ kolin::graph{ width, height, data }.make_body(xInt, yInt, adr::Stock::day + 1 - graphHistoryLength, minY) };

        std::cout << str << "\n\n";

        return dpp::message{ "```\n" + str + "```"};
    }
    catch (const std::exception& e) {
        std::cerr << "adr::Stock::getGraph() error: " << e.what() << '\n';

        return dpp::message{ "Sorry, an error occurred" }.set_flags(dpp::m_ephemeral);
    }
}

static char getDiffChar(int diff)
{
    if (diff > 0) return '+';
    else if (diff < 0) return '-';
    else return '=';
}

static std::string getDiffEmoji(int diff)
{
    if (diff > 0)
        return dpp::emoji{ "green_circle" }.get_mention();
    else if (diff < 0)
        return dpp::emoji{ "red_circle" }.get_mention();
    else 
        return dpp::emoji{ "white_small_square" }.get_mention();
}

dpp::message adr::Stock::getEmbed(std::string name)
{
    dpp::message msg{};

    if (name == "compact") {
        dpp::embed embed{};
        embed
            .set_title("Stocks")
            .set_description("Day " + std::to_string(adr::Stock::getDay()) + " / " + dpp::utility::timestamp(std::time(0), dpp::utility::tf_long_date) + '\n')
            .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/paper.png")
            .set_color(0xeeeeee);

        for (const adr::Stock& stock : adr::Stock::stocks) {
            int diff{ stock.getValue() - stock.getHistory(1) };

            const char c{ getDiffChar(diff) };
            const std::string emoji{ getDiffEmoji(diff) };

            embed.set_description(embed.description
                + emoji + " `" + stock.getTicker() + "`: "
                + "**" + std::to_string(stock.getValue()) + "**"
                + (c == '=' ? ", " : " (" + std::string{ c } + std::to_string(std::abs(diff)) + "), ")
                + std::to_string(stock.m_stability) + "%, "
                + std::to_string(stock.getOutstanding()) + '/' + std::to_string(stock.getUnissued()) + '\n');
        }

        msg.add_embed(embed);
        return msg;
    }


    // If it is not the compact view:

    const adr::Stock::Id id{ adr::Stock::getId(name) };
    const adr::Stock& stock{ adr::Stock::getStock(id) };

    const int diff{ stock.getValue() - stock.getHistory(1) };
    const char c{ getDiffChar(diff) };
    const std::string emoji{ getDiffEmoji(diff) };

    dpp::embed embed{};
    embed
        .set_title(name + " (" + stock.getTicker() + ')')
        .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/paper.png")
        .set_color(0xeeeeee)
        .set_description(emoji + " **Value: " + std::to_string(stock.getValue()) + "** (" + c + std::to_string(std::abs(diff)) + ")\n"
            + "__Stabililty__: " + std::to_string(stock.m_stability) + "%\n"
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
        stock["ticker"] = obj.m_ticker;
        stock["value"] = obj.m_value;
        stock["stability"] = obj.m_stability;
        stock["outstanding"] = obj.m_outstanding;
        stock["unissued"] = obj.m_unissued;
        for (std::size_t d{}; d < adr::Stock::day; ++d) {
            stock["history"][d] = obj.getHistory(d);
        }
    }

    std::ofstream fs{ path };
    fs << data;
    fs.close();

    std::cout << "Done saving stocks JSON!\n";
}

void adr::Stock::parseJSON()
{
    std::cout << "adr::Stock::parseJSON() called" << std::endl;

    using json = nlohmann::json;

    std::ifstream fs{ path };
    json data{};

    try {
        data = json::parse(fs); 
    }
    catch (const json::parse_error& e) {
        std::cerr << "adr::Stock::parseJSON() error: " << e.what() << '\n';
        return;
    }

    adr::Stock::day = data["day"];

    for (std::size_t i{}; i < adr::Stock::stocks.size(); ++i) {
        const json& stock{ data["stocks"][i] };

        std::array<int, adr::Stock::historyLength> history{};
        try {
            for (std::size_t hi{}; hi < stock.at("history").size(); ++hi) {
                history[hi] = stock.at("history").at(hi);
            }
        }
        catch (const json::out_of_range&) {
            history[0] = stock["value"].get<int>();
        }

        adr::Stock::stocks[i] = { 
            stock["name"].get<std::string>(), 
            stock["ticker"].get<std::string>(),
            static_cast<adr::Stock::Id>(i),
            stock["value"].get<int>(),
            stock["stability"].get<std::int8_t>(),
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

    constexpr std::int8_t base{ 10 };
    constexpr std::int8_t increase{ 5 };

    // Tier 1 Job - base adriencoin increase
    // Tier 2 Job - base + increase adriencoin
    // https://www.desmos.com/calculator/e06yhf1wsb
    const int change{ static_cast<int>(id) / static_cast<int>(adr::Job::tierOneJobsSize) * increase + base };

    // Change the stability to the new stability
    adr::Stock::stocks[sid].m_stability += static_cast<std::int8_t>(change);
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
    std::cout << "adr::Stock::addSlashCommands() called\n";

    parseJSON();

    dpp::command_option stockopt{ dpp::co_string, "stock", "Which stock", true };

    for (const adr::Stock& i : adr::Stock::stocks) {
        stockopt.add_choice(dpp::command_option_choice{ i.m_name, i.m_name });
    }

    dpp::command_option amtopt{ dpp::co_integer, "amount", "How many stocks to buy", false };

    dpp::slashcommand cmd{ "stock", "Buy or sell a stock", bot.me.id };
    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "buy", "Buy a stock" }.add_option(stockopt).add_option(amtopt));
    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "sell", "Sell a stock" }.add_option(stockopt).add_option(amtopt));

    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "view", "View a stock" }.add_option(stockopt));

    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "compact", "Compact view of all stocks" });
    
    cmd.add_option(dpp::command_option{ dpp::co_sub_command, "graph", "View a stock graph" }
    .add_option(stockopt)
    .add_option(dpp::command_option{ dpp::co_integer, "history", "How far back you want to look", false }));

    commandList.push_back(cmd);

    std::cout << "adr::Stock::addSlashCommands() finished\n";
}
