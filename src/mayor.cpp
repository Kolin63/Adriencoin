#include "mayor.h"
#include "Random.h"
#include "botToken.h"
#include <algorithm>
#include <fstream> 
#include <iostream>
#include <dpp/nlohmann/json.hpp>

void adr::Mayor::loadJSON() {
  using json = nlohmann::json;

  std::ifstream fs{ "./../data/mayors.json" };  

  json data{};

  try {
    data = json::parse(fs);
  }
  catch (const json::parse_error& e) {
    std::cerr << "adr::Mayor::loadJSON parse error: " << e.what() << '\n';
    return;
  }

  json buf;

  try {
    buf = data.at("mayors");
  } 
  catch (const json::out_of_range& e) {
    std::cerr << "adr::Mayor::loadJSON error getting mayor list: " << e.what()
      << '\n';
    return;
  }

  try {
    currentMayorName = buf.at("current");
  }
  catch (const json::out_of_range& e) {
    std::cerr << "adr::Mayor::loadJSON() could not get currentMayorName: "
      << e.what() << '\n';
    currentMayorName = "null";
  }
  currentMayor = 0;

  try {
    votes = buf.at("votes");
  }
  catch (const json::out_of_range& e) {
    std::cerr << "adr::Mayor::loadJSON() could not get votes: "
      << e.what() << '\n';
  }

  try {
    candidates = buf.at("candidates");
  }
  catch (const json::out_of_range& e) {
    std::cerr << "adr::Mayor::loadJSON() could not get candidates: "
      << e.what() << '\n';
  }

  try {
    votes = buf.at("votes");
  }
  catch (const json::out_of_range& e) {
    std::cerr << "adr::Mayor::loadJSON() could not get votes: "
      << e.what() << '\n';
  }

  mayors.clear();
  mayors.reserve(buf.size());

  for (std::size_t i{}; i < buf.size(); ++i) {
    mayors.push_back(MayorData{
        buf["mayors"][i]["name"],
        buf["mayors"][i]["image"],
        buf["mayors"][i]["desc"],
        buf["mayors"][i]["perkAmt"],
        buf["mayors"][i]["perkMax"],
        buf["mayors"][i]["isSpecial"],

#define ADR_MAYORDATA_MAKE_PERK(name) { buf["mayors"][i]["perks"][#name][0], buf["mayors"][i]["perks"][#name][1] },

        MayorData::Perks{
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(carrotPerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(gemstonePerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(woodPerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(fishPerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(rottenfleshPerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(bookPerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(potionPerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(adriencoinPerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(adriresourcePerWork)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(dailyDungeonPasses)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(dungeonPriceMultiplier)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(dungeonAdriresourceIncrease)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(dungeonWinChanceIncrease)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(dungeonDropChanceIncrease)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(resourceExchangePrice)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(allShopAdriencoinMultiplier)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(stockStabilityCap)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(titleMoneyMultiplier)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(workYieldMultiplier)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(workCooldownChangeMinutes)
MayorData::Perk<bool>ADR_MAYORDATA_MAKE_PERK(bankInterestOnElection)
MayorData::Perk<bool>ADR_MAYORDATA_MAKE_PERK(dailyGiveaway)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(bribe)
MayorData::Perk<bool>ADR_MAYORDATA_MAKE_PERK(darkerAuctions)
MayorData::Perk<int> ADR_MAYORDATA_MAKE_PERK(workPresents)
MayorData::Perk<bool>ADR_MAYORDATA_MAKE_PERK(animeGirls)
MayorData::Perk<bool>ADR_MAYORDATA_MAKE_PERK(activateOtherPerks)
MayorData::Perk<bool>ADR_MAYORDATA_MAKE_PERK(insurance)
        // { buf["mayors"][i]["perks"]["carrotPerWork"][0],               buf["mayors"][i]["perks"]["carrotPerWork"][1] },
        // { buf["mayors"][i]["perks"]["gemstonePerWork"][0],             buf["mayors"][i]["perks"]["gemstonePerWork"][1] },
        // { buf["mayors"][i]["perks"]["woodPerWork"][0],                 buf["mayors"][i]["perks"]["woodPerWork"][1] },
        // { buf["mayors"][i]["perks"]["fishPerWork"][0],                 buf["mayors"][i]["perks"]["fishPerWork"][1] },
        // { buf["mayors"][i]["perks"]["rottenfleshPerWork"][0],          buf["mayors"][i]["perks"]["rottenfleshPerWork"][1] },
        // { buf["mayors"][i]["perks"]["bookPerWork"][0],                 buf["mayors"][i]["perks"]["bookPerWork"][1] },
        // { buf["mayors"][i]["perks"]["potionPerWork"][0],               buf["mayors"][i]["perks"]["potionPerWork"][1] },
        // { buf["mayors"][i]["perks"]["adriencoinPerWork"][0],           buf["mayors"][i]["perks"]["adriencoinPerWork"][1] },
        // { buf["mayors"][i]["perks"]["adriresourcePerWork"][0],         buf["mayors"][i]["perks"]["adriresourcePerWork"][1] },
        // { buf["mayors"][i]["perks"]["dailyDungeonPasses"][0],          buf["mayors"][i]["perks"]["dailyDungeonPasses"][1] },
        // { buf["mayors"][i]["perks"]["dungeonPriceMultiplier"][0],      buf["mayors"][i]["perks"]["dungeonPriceMultiplier"][1] },
        // { buf["mayors"][i]["perks"]["dungeonAdriresourceIncrease"][0], buf["mayors"][i]["perks"]["dungeonAdriresourceIncrease"][1] },
        // { buf["mayors"][i]["perks"]["dungeonWinChanceIncrease"][0],    buf["mayors"][i]["perks"]["dungeonWinChanceIncrease"][1] },
        // { buf["mayors"][i]["perks"]["dungeonDropChanceIncrease"][0],   buf["mayors"][i]["perks"]["dungeonDropChanceIncrease"][1] },
        // { buf["mayors"][i]["perks"]["resourceExchangePrice"][0],       buf["mayors"][i]["perks"]["resourceExchangePrice"][1] },
        // { buf["mayors"][i]["perks"]["allShopAdriencoinMultiplier"][0], buf["mayors"][i]["perks"]["allShopAdriencoinMultiplier"][1] },        
        // { buf["mayors"][i]["perks"]["stockStabilityCap"][0],           buf["mayors"][i]["perks"]["stockStabilityCap"][1] },
        // { buf["mayors"][i]["perks"]["titleMoneyMultiplier"][0],        buf["mayors"][i]["perks"]["titleMoneyMultiplier"][1] },
        // { buf["mayors"][i]["perks"]["workYieldMultiplier"][0],         buf["mayors"][i]["perks"]["workYieldMultiplier"][1] },
        // { buf["mayors"][i]["perks"]["workCooldownChangeMinutes"][0],   buf["mayors"][i]["perks"]["workCooldownChangeMinutes"][1] },
        // { buf["mayors"][i]["perks"]["bankInterestOnElection"][0],      buf["mayors"][i]["perks"]["bankInterestOnElection"][1] },
        // { buf["mayors"][i]["perks"]["dailyGiveaway"][0],               buf["mayors"][i]["perks"]["dailyGiveaway"][1] },
        // { buf["mayors"][i]["perks"]["bribe"][0],                       buf["mayors"][i]["perks"]["bribe"][1] },
        // { buf["mayors"][i]["perks"]["darkerAuctions"][0],              buf["mayors"][i]["perks"]["darkerAuctions"][1] },
        // { buf["mayors"][i]["perks"]["workPresents"][0],                buf["mayors"][i]["perks"]["workPresents"][1] },
        // { buf["mayors"][i]["perks"]["animeGirls"][0],                  buf["mayors"][i]["perks"]["animeGirls"][1] },
        // { buf["mayors"][i]["perks"]["activateOtherPerks"][0],          buf["mayors"][i]["perks"]["activateOtherPerks"][1] }, 
        // { buf["mayors"][i]["perks"]["insurance"][0],                   buf["mayors"][i]["perks"]["insurance"][1] }
        }
    });

    // translates to:
    // if the name is equal to the saved name, then set current index to i
    currentMayor += (currentMayorName == buf["mayors"][i]["perks"].get<std::string>()) * i;
  }
}

void adr::Mayor::startElection()
{
  std::cout << "adr::Mayor::startElection() called\n";

  loadJSON();
  
  /* choose candidates

#1	All candidates (except last year's winners and special mayors) are added 
    to a possible candidates list.
#2	Each candidate now has a 50% chance to gain an extra perk if they did not 
    have 3 yet. (mayors reset to one perk after being elected)
#3	The possible candidates list is shuffled, and the first 5 are chosen.
#4	If this year requires a special candidate the 5th possible candidate is 
    removed and replaced with the correct special candidate. This happens 
    every specialMayorReset elections.
 */

  std::vector<std::size_t> poscand;
  poscand.reserve(mayors.size());

  for (std::size_t i{}; i < mayors.size(); ++i) {
    // dont add special or last winner
    if (!mayors[i].isSpecial && mayors[i].name != currentMayorName) [[likely]] 
    {
      // If perks not maxed, then 50% chance to get new perk. branchless!
      mayors[i].perkAmt += (mayors[i].perkAmt != mayors[i].perkMax 
          && Random::get(0, 1) == 1);

      poscand.push_back(i);
      std::cout << "possible candidate: " << mayors[i].name << " with " 
        << mayors[i].perkAmt << " perks\n";
    }
  }

  std::shuffle(poscand.begin(), poscand.end(), Random::mt);

  // handle specials
  if (specialMayorCounter == specialMayorReset) [[unlikely]] {
    // count how many special mayors there are
    std::vector<std::size_t> specials;
    specials.reserve(mayors.size());

    for (std::size_t i{}; i < mayors.size(); ++i) 
      if (mayors[i].isSpecial) [[unlikely]] specials.push_back(i);

    specials.reserve(specials.size());

    // choose a random special from the list
    std::size_t chosenSpecial{ specials[Random::get(0, specials.size())] };

    // replace the fifth candidate with the special 
    poscand[4] = chosenSpecial;
  }

  // set variables to prepare for voting
  std::string msg{};

  std::cout << "chosen candidates are:\n";
  for (std::size_t i{}; i < 5; ++i) {
    std::cout << '\t' << mayors[poscand[i]].name << ' ' 
      << mayors[poscand[i]].perkAmt << " perks\n";

    candidates[i] = poscand[i];

    msg += mayors[poscand[i]].name + '\n';
  }

  votes = {};
  isElection = true;

  bot.message_create(dpp::message{ 
      channelID,
      msg
      }, [](const dpp::confirmation_callback_t& e){
      if (e.is_error())
      std::cerr << "adr::Mayor::startElection() error: " 
      << e.get_error().human_readable << '\n';
      else
      std::cout << "Mayor election message sent\n";
      });

  std::cout << "adr::Mayor::startElection() finished\n";
}
