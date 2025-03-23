#pragma warning(disable: 4251) // disables a silly warning from dpp

#include "upgradeSave.h"

void adr::upgradeSave(Inventory& inv, uint32_t& version)
{
    const uint32_t& CURRENT{ adr::CURRENT_SAVE_VERSION };

    switch (version) 
    {
    case 0:
        inv[adr::Item::stockofstonks] = 0;
        version = 1;
        [[fallthrough]];

    case CURRENT:
        return;
    default:
        std::cout << "!!! ERROR !!! Player Data Version " << version << " unaccounted for in upgradeSave()\n";
        return;
    }
}
