#ifndef DAILY_H
#define DAILY_H

#include <ctime>
#include <dpp/dpp.h>

namespace adr
{
    namespace daily
    {
        enum Title {
            t_none,
            t_baron,
            t_duke,
        };

        void doDailyTasks(dpp::timer t = {});
        void doTitleMoney();

        uint64_t getTimeToMidnight();
    }
}

#endif
