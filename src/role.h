#include <dpp/role.h>
#include <dpp/snowflake.h>

namespace adr
{
  // i didnt want to make a seperate header file for this so its here
  enum FameRank
  {
    f_none,

    f_citizen,
    f_contributor,
    f_patron,
    f_attache,
    f_ambassador,

    f_MAX
  };

  const std::array<std::string_view, f_MAX> fameRankNames{
    "none",
    "citizen",
    "contributor",
    "patron",
    "attache",
    "ambassador",
  };

  enum RoleID
  {
    r_farmer,
    r_miner,
    r_lumberjack,
    r_fisherman,
    r_warrior,
    r_enchanter,
    r_brewer,

    r_agriculturalist,
    r_collier,
    r_logger,
    r_piscator,
    r_knight,
    r_thaumaturgist,
    r_alchemist,

    r_baron,
    r_duke,
    r_grandDuke,

    r_citizen,
    r_contributor,
    r_patron,
    r_attache,
    r_ambassador,

    r_MAX
  };

#ifndef DEBUG
  const std::array<dpp::snowflake, r_MAX> roleIDs 
  {
    1353491203366195292, // r_farmer
    1353491220521025577, // r_miner 
    1353491219266797660, // r_lumberjack
    1353491218549440532, // r_fisherman
    1353491216964124784, // r_warrior
    1353491216511012934, // r_enchanter
    1353491214934081568, // r_brewer

    1353491213826920572, // r_agriculturalist
    1353491213222674493, // r_collier
    1353491212342136883, // r_logger
    1353491211469590719, // r_piscator
    1353491210383396935, // r_knight
    1353491209284485225, // r_thaumaturgist
    1353491208768323665, // r_alchemist

    1360027149062508615, // r_baron 
    1372728902530633728, // r_duke 
    1372728958767861881, // r_grandDuke

    1373045003278090270, // r_citizen
    1373045047527870625, // r_contributor
    1373045086916579420, // r_patron
    1373045119229366423, // r_attache
    1373045149290074163, // r_ambassador
  };
#else
  // These are the UUIDs for the roles, not to be confused with the 
  // RoleID enum.
  const std::array<dpp::snowflake, r_MAX> roleIDs 
  {
    1341184396027232317, // r_farmer
    1341184413232402442, // r_miner
    1341184412238348440, // r_lumberjack
    1341184411265138728, // r_fisherman
    1341184410405306378, // r_warrior
    1341184409516380273, // r_enchanter
    1341184408207495188, // r_brewer

    1341184406668443731, // r_agriculturalist
    1341184405934444587, // r_collier 
    1341184404608913441, // r_logger 
    1341184403614732289, // r_piscator
    1341184402780065815, // r_knight
    1341184401995862136, // r_thaumaturgist
    1341184401190424596, // r_alchemist

    1372704052596576368, // r_baron
    1372704084582465566, // r_duke 
    1372704101611081779, // r_grandDuke

    1373042062542504066, // r_citizen
    1373042102656700596, // r_contributor
    1373042241844674630, // r_patron
    1373042281183056034, // r_attache
    1373042319523184661, // r_ambassador
  };
#endif
}
