#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <iostream>
#include <vector>
#include <chrono>
#include "daily.h"
#include "cache.h"
#include "botToken.h"

void adr::daily::doDailyTasks(dpp::cluster& bot)
{
    std::cout << "doDailyTasks() called\n";

    doTitleMoney();

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

uint64_t adr::daily::getTimeToMidnight()
{
    // thanks copilot!

    using namespace std::chrono;

    auto now = system_clock::now();
    auto tomorrow = floor<days>(now) + days{1};
    auto midnight = time_point_cast<system_clock::duration>(tomorrow);
    uint64_t out{ static_cast<uint64_t>(duration_cast<seconds>(midnight - now).count()) };

    std::cout << "time until midnight: " << out << '\n';
    return out;
}
