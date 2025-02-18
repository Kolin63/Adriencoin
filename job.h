#ifndef JOB_H
#define JOB_H

#include <string>
#include <array>
#include "item.h"

namespace adr {

    struct Job {
        enum Id
        {
            farmer,
            miner,
            lumberjack,
            fisherman,
            warrior,
            enchanter,
            brewer,

            agriculturalist,
            collier,
            logger,
            piscator,
            knight,
            thaumaturgist,
            alchemist,

            MAX
        };

        static const std::array<adr::Job, 14> jobs;

        Id id{};
        std::string name{};
        std::string action{};
        adr::Item item{};
        int adriencoin{};
        uint32_t color{};
    };

    void doJob([[maybe_unused]] const dpp::cluster& bot, const dpp::slashcommand_t& event);
}

#endif
