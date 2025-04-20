#ifndef JOB_H
#define JOB_H

#include <string>
#include <string_view>
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
        static constexpr std::size_t tierOneJobsSize{ adr::Job::brewer + 1 };

        static Id getId(std::string_view jobName);

        Id id{};
        std::string name{};
        std::string action{};
        adr::item item{};
        int adriencoin{};
        uint32_t color{};
    };
}

#endif
