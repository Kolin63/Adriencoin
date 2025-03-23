#pragma warning(disable: 4251) // disables a silly warning from dpp

#include "upgradeSave.h"

adr::playerData adr::upgradeSave(playerData pd)
{
    const uint32_t& CURRENT{ adr::playerData::CURRENT_SAVE_VERSION };

    switch (pd.version) 
    {
    case CURRENT:
        return pd;
    default:
        std::cout << "!!! ERROR !!! Player Data Version " << pd.version << " unaccounted for in upgradeSave()\n";
        return pd;
    }
}
