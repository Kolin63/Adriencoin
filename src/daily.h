#ifndef DAILY_H
#define DAILY_H

#include <ctime>
#include <array>
#include <dpp/dpp.h>

namespace adr
{
    namespace daily
    {
        enum Title {
            t_none,
            t_baron,
            t_duke,

            t_MAX
        };

        const std::array<std::string, t_MAX> titleNames{ "None", "Baron", "Duke" };

        void doDailyTasks(dpp::cluster& bot);
        void doTitleMoney();
        void doStocks();

        uint64_t getTimeToMidnight();
    }
}

#endif
