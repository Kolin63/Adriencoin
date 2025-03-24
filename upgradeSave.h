#ifndef UPGRADE_SAVE_H
#define UPGRADE_SAVE_H

#include "job.h"
#include "inventory.h"

namespace adr {

    static constexpr uint32_t CURRENT_SAVE_VERSION{ 1 };

    void upgradeSave(Inventory& inv, uint32_t& version);
}

#endif
