#ifndef KOLIN_DUNGEON_H
#define KOLIN_DUNGEON_H

#include <array>
#include <optional>
#include <dpp/message.h>
#include "item.h"
#include "inventory.h"

namespace adr
{
    ///
    /// dungeon_id
    /// @brief An enum containing all dungeon ID's
    /// @see dungeons
    ///
    enum dungeon_id
    {
        d_MAX
    };

    ///
    /// dungeon
    /// @brief A class containing the information for a dungeon boss 
    ///
    class dungeon
    {
    private:

        ///
        /// id
        /// @brief The id of the boss
        ///
        const dungeon_id id;

        ///
        /// name
        /// @brief The name of the boss
        ///
        const std::string_view name;

        ///
        /// price
        /// @brief The price of the boss, in Adriencoin
        ///
        const int price;

        ///
        /// win_chance
        /// @brief The chance for the player to beat this boss.
        /// The chance can be anywhere between 0 and 100, inclusive
        ///
        const uint8_t win_chance;

        ///
        /// item_chances
        /// @brief An array where the index is the Item ID, the 
        /// first of the pair is the chance for that item to drop,
        /// and the second of the pair is the amount of the item.
        /// The chance can be anywhere between 0 and 100, inclusive
        ///
        const std::array<std::pair<uint8_t, int>, i_MAX> item_chances;

        ///
        /// thumbnail_url
        /// @brief The url to the thumbnail that will be used in the embed
        ///
        const std::string_view thumbnail_url;

        /// 
        /// try_win()
        /// @brief Attempt to win a boss fight
        /// @return True if succesful, false otherwise
        /// 
        bool try_win() const;

        ///
        /// try_drop()
        /// @brief Attempt to get a drop from a boss fight
        /// @param i Which item to try for
        /// @return True if succesful, false otherwise
        ///
        bool try_drop(item_id i) const;

        ///
        /// fight()
        /// @brief Handles boss fight RNG and item distribution
        /// @param uuid The player that is fighting
        /// @return std::optional<Inventory> that will be null if the
        /// fight was unsuccesful, and contain the item drops otherwise
        ///
        std::optional<inventory> fight(const dpp::snowflake& uuid) const;

    public:

        ///
        /// dungeon()
        /// @brief A constructor for a dungeon boss
        /// @param _win_chance The base chance for this boss to be beaten
        /// @param item_chances An array where the index is the Item ID and the 
        /// uin8_t is the chance for that item to drop
        /// @see win_chance
        /// @see item_chances
        ///
        dungeon(

                dungeon_id _id,
                std::string_view _name,
                int _price,
                uint8_t _win_chance, 
                const std::array<std::pair<uint8_t, int>, i_MAX>& _item_chances,
                std::string_view _thumbnail_url
        ) 
                : id(_id)
                , name(_name)
                , price(_price)
                , win_chance(_win_chance)
                , item_chances(_item_chances) 
                , thumbnail_url(_thumbnail_url)
        {
        }

        ///
        /// get_embed()
        /// @return A Discord embed with information on the boss
        ///
        dpp::embed get_embed() const;

        ///
        /// get_price()
        /// @return The price of the boss in Adriencoin
        ///
        int get_price() const;

        /// 
        /// buy()
        /// @brief The main interface for fighting a boss. 
        /// Handles the cost of the boss, RNG, and item distribution 
        /// @param uuid The player that is fighting
        /// @return A Discord message with a human-readable status
        /// of the fight
        /// 
        dpp::message buy(const dpp::snowflake& uuid) const;
    };

    const std::array<dungeon, d_MAX> dungeons;
}

#endif
