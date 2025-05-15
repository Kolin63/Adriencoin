#ifndef KOLIN_ADRIENCOIN_STATISTICS_H
#define KOLIN_ADRIENCOIN_STATISTICS_H

#include "emoji.h"
#include <dpp/nlohmann/json.hpp>

namespace adr 
{
  struct Statistics
  {
    // forward declaration
    struct Stat;

    void saveJSON(nlohmann::json& data) const;

    void loadJSON(const nlohmann::json& data);

    std::string list() const;

    struct Stat
    {
      const int defaultValue{};
      const std::string_view name;
      const adr::emoji_id emoji{ adr::e_fish };
      int value{ defaultValue };
    };

    // ====================================================================
    //
    // Below are the actual attributes
    //
    // ====================================================================

    // Stat example{ 0, "example", adr::e_fish };
    Stat timesWorked{ 0, "timesWorked", adr::e_carrot };
    Stat timesInStore{ 0, "timesInStore", adr::e_gemstone };
    Stat bonzo{ 0, "bonzo", adr::e_bonzo_mask };
    Stat scarf{ 0, "scarf", adr::e_loving_scarf };
    Stat theProfessor{ 0, "theProfessor", adr::e_fish };
    Stat thorn{ 0, "thorn", adr::e_spirit_sceptre };
    Stat livid{ 0, "livid", adr::e_livid_dagger };
    Stat sadan{ 0, "sadan", adr::e_giant_sword };
    Stat necron{ 0, "necron", adr::e_hyperion };
  };
}

#endif
