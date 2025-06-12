#ifndef ADRIENCOIN_MAYOR_H_
#define ADRIENCOIN_MAYOR_H_

#include <string>
#include <cstdint>
#include <vector>

namespace adr 
{
  class Mayor 
  {
    public:
      struct MayorData 
      {
        std::string  name;
        std::string  image;
        std::string  desc;
        std::uint8_t perkAmt;
        std::uint8_t perkMax;
        bool         isSpecial;

        template<typename T>
          struct Perk 
          {
            T value;
            std::uint8_t rank; // 1, 2, 3, etc. 1 = common. 3 = rare.
          };

        struct Perks 
        {
          Perk<int>  carrotPerWork;
          Perk<int>  gemstonePerWork;
          Perk<int>  woodPerWork;
          Perk<int>  fishPerWork;
          Perk<int>  rottenfleshPerWork;
          Perk<int>  bookPerWork;
          Perk<int>  potionPerWork;
          Perk<int>  adriencoinPerWork;
          Perk<int>  adriresourcePerWork;

          Perk<int>  dailyDungeonPasses;
          Perk<int>  dungeonPriceMultiplier;
          Perk<int>  dungeonAdriresourceIncrease;
          Perk<int>  dungeonWinChanceIncrease;
          Perk<int>  dungeonDropChanceIncrease;

          Perk<int>  resourceExchangePrice;
          Perk<int>  allShopAdriencoinMultiplier; // only changes adriencoin prices
          Perk<int>  stockStabilityCap;
          Perk<int>  titleMoneyMultiplier;
          Perk<int>  workYieldMultiplier;
          Perk<int>  workCooldownChangeMinutes; // should be negative to decrease

          Perk<bool> bankInterestOnElection;
          Perk<bool> dailyGiveaway;
          Perk<int>  bribe;
          Perk<bool> darkerAuctions;

          // workPresents:
          // every player gets presents whenever they work. presents can be 
          // gifted to other players, and the player that gets gifted gets free 
          // items.
          Perk<int>  workPresents;

          // animeGirls:
          // players can use the mayor command to get a picture of an anime girl,
          // no cooldown. there is a very rare chance that they get the Marina
          // fanart, and if they do they win 30,000 fish and 100 adriencoin.
          Perk<bool> animeGirls;

          // activates all perks of another mayor once every day
          // on election, a list is shuffled with aatrox, cole, diana, finnegan,
          // marina, and paul. the perks are then cycled through that list.
          Perk<bool> activateOtherPerks; 

          // insurance:
          // when insurance is enabled, everybody has a chance to get hurt when 
          // working. if they didn't buy insurance, they won't get any profit. 
          // if they do have insurance, nothing will be different. 
          // insurance also acts as an extra bonzo mask, so dungeons can be 
          // retried, except for free
          Perk<bool> insurance;

        } perks;

        /* 
           - Aatrox: 
           - 300 rottenflesh per work 
           - one daily dungeon pass 
           - dungeon costs halved
           - Cole: 
           - 25 gemstone per work
           - Diana: 
           - bank interest is done on diana's election
           - Diaz: 
           - resource exchange price changed from 10 to 7 
           - stock stability will be capped at 80% rather than 50%
           - title money increased by 50%
           - Finnegan: 
           - 100 carrot per work 
           - Foxy:
           - enable presents, 2 per work
           - Daily giveaway
           - Marina:
           - 150 fish per work 
           - enable anime girls
           - Paul:
           - every dungeon boss drops 10 more adriresource
           - dungeon win chance increased by 10%
           - dungeon drop chance increased by 10%
           - Jerry: 
           - activate all perks of another mayor every day 
           - decrease work cooldown by 90 minutes 
           - enable presents, 4 per work
           - Derpy:
           - all shop prices have their adriencoin doubled
           - working produces double the adriencoin and resources 
           - dungeon win chance is decreased by 15% 
           - Scorpius: 
           - 250 adriencoin bribe if you vote for scorpius 
           - darker auctions. one more round and special items. 
           - Adrien: 
           - you can buy insurance every time adrien is elected for 50 coin 
           - every time you work, you have a chance to get hurt 
           - if you get hurt without insurance, you make nothing 
           - if you get hurt with insurance, you get the normal amount of profit
           - insurance also acts as a second bonzo mask for dungeons, except for 
           free. it also stacks with bonzo mask. 
           */
      } mayorData;

    private:
      // Increments once a week, and is reset to 0 at specialMayorReset
      // when counter == reset, that means it is special
      static std::uint8_t specialMayorCounter;
      constexpr static std::uint8_t specialMayorReset{ 4 };

      static std::size_t currentMayor;
      static std::string currentMayorName;

      static std::vector<MayorData> mayors;

      static bool isElection;
      static std::array<std::size_t, 5> candidates;
      static std::array<std::uint8_t, 5> votes;

#ifndef DEBUG 
      constexpr static std::uint64_t channelID{ 1354481700633317488 };
      constexpr static std::uint64_t pingID{ 1379976141145903145 };
#else 
      constexpr static std::uint64_t channelID{ 1379976702599626863 };
      constexpr static std::uint64_t pingID{ 1379976749072257104 };
#endif

    public:

      Mayor(const MayorData& md) : mayorData{ md } {};

      static const MayorData& getCurrent() { return mayors[currentMayor]; }

      const MayorData::Perks& getFeats() const { return mayorData.perks; }

      static void startElection();

      static void loadJSON();

  };
}

#endif
