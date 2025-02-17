#ifndef JOB_H
#define JOB_H

#include <string>
#include <array>
#include "item.h"

namespace adr {
    struct Job {
        std::string name{};
        std::string action{};
        adr::Item item{};
        uint32_t color{};
    };

    extern const std::array<adr::Job, 14> jobs;

    void doJob([[maybe_unused]] const dpp::cluster& bot, const dpp::slashcommand_t& event);
}

#endif
