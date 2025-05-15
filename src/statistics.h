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
      static constexpr int defaultValue{};
      const std::string_view name;
      const adr::emoji_id emoji{ adr::e_fish };
      int value{ defaultValue };
    };

    // ====================================================================
    //
    // Below are the actual attributes
    //
    // ====================================================================

    // Stat example{ 0, "exampe", adr::e_fish };
  };
}

#endif
