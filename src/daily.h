#ifndef DAILY_H
#define DAILY_H

#include <ctime>
#include <array>
#include <dpp/dpp.h>

namespace adr
{
    namespace daily
    {
        enum Title : std::uint8_t {
            t_none,
            t_baron,
            t_duke,
            t_grand_duke,

            t_MAX
        };

        const std::array<std::string, t_MAX> titleNames{ "None", "Baron", "Duke", "Grand Duke" };

        void doDailyTasks(dpp::cluster& bot);

        // Returns the time until Midnight EST, in seconds
        // Note: It is actually 11:55 PM EST, to give padding
        uint64_t getTimeToMidnight();
    }
}

#endif
