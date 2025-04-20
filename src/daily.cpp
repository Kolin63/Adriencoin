#include "item.h"
#include <iostream>
#include <vector>
#include "daily.h"
#include "cache.h"
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
            player.changeInv(adr::i_adriencoin, 5);
            break;
        case adr::daily::t_duke:
            player.changeInv(adr::i_adriencoin, 10);
            break;
        default:
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
    // How much padding it should give in seconds
    constexpr std::uint64_t paddingSeconds{ 60 * 5 };

    // Difference between UTC and preferred time zone in seconds
    // Example: UTC is 4 hours ahead of EST, so the value should be positive 4 hours
    constexpr std::int64_t utcDiffSeconds{ 60 * 60 * 4 };

    // Get current time
    const std::time_t now = std::time(nullptr);

    // 60 seconds * 60 minutes * 24 hours = 86400
    constexpr std::uint64_t secInDay{ 60 * 60 * 24 };

    // Seconds that have passed today
    const std::uint64_t todaySeconds{ now % secInDay };

    // Time to Midnight in Seconds
    const std::uint64_t timeToMidnight{ secInDay - todaySeconds - paddingSeconds + utcDiffSeconds };

    std::cout << "time until midnight EST: " << timeToMidnight << '\n';
    return timeToMidnight;
}
