#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <iostream>
#include <vector>
#include <chrono>
#include "daily.h"
#include "cache.h"
#include "botToken.h"
#include "stock.h"

void adr::daily::doDailyTasks(dpp::cluster& bot)
{
    std::cout << "doDailyTasks() called\n";

    doTitleMoney();
    doStocks();

    bot.direct_message_create(dpp::snowflake{ 488335709883727882 }, dpp::message{ "Daily Tasks Complete" }, [](const dpp::confirmation_callback_t& callback) {
        if (callback.is_error()) {
            std::cerr << "do daily tasks dm error: " << callback.get_error().human_readable << '\n';
        }
        else {
            std::cout << "do daily tasks dm sent\n";
        }
        });
}

void adr::daily::doTitleMoney()
{
    std::cout << "doTitleMoney() called\n";

    std::vector<dpp::snowflake> players{ adr::cache::cacheAll() };

    for (const dpp::snowflake& uuid : players) {
        adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

        switch (player.getTitle()) 
        {
        case adr::daily::t_baron:
            player.changeInv(adr::Item::adriencoin, 5);
            break;
        case adr::daily::t_duke:
            player.changeInv(adr::Item::adriencoin, 10);
            break;
        }
    }
}

void adr::daily::doStocks()
{
    std::cout << "doStocks() called\n";

    adr::Stock::newDay();

    adr::Stock::saveJSON();
}

uint64_t adr::daily::getTimeToMidnight()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto now_utc = time_point_cast<seconds>(now);
    auto now_est = now_utc - hours(4); // EST is UTC-4

    auto tomorrow_est = floor<days>(now_est) + days{1};
    auto midnight_est = time_point_cast<system_clock::duration>(tomorrow_est);
    uint64_t out{ static_cast<uint64_t>(duration_cast<seconds>(midnight_est - now_est).count()) };

    std::cout << "time until midnight EST: " << out << '\n';
    return out;
}
