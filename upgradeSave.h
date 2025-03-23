#pragma once

#include "job.h"
#include "inventory.h"

namespace adr {

    struct playerData
    {
        static constexpr uint32_t CURRENT_SAVE_VERSION{ 1 };

        uint32_t version{};
        adr::Job::Id job{};
        std::time_t lastWorked{ 0 };
        Inventory inv{};
    };

    playerData upgradeSave(playerData pd);
}
