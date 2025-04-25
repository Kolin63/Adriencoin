#ifndef KOLIN_DUNGEON_H
#define KOLIN_DUNGEON_H

#include <array>
#include <dpp/appcommand.h>
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
        d_foo,

        d_MAX
    };

    ///
    /// dungeon
    /// @brief A class containing the information for a dungeon boss 
    ///
    class dungeon
    {
    public:

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

    private:
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

        ///
        /// add_slash_commands()
        /// @brief A function that adds all slash commands related to
        /// dungeons
        /// @param bot The Discord Bot
        /// @param command_list The list of commands that will be added to
        ///
        static void add_slash_commands(
                dpp::cluster& bot, 
                std::vector<dpp::slashcommand>& command_list
        );

        ///
        /// handle_slash_command()
        /// @brief To be called on a slash command to handle it
        ///
        static void handle_slash_command(
                dpp::cluster& bot,
                const dpp::slashcommand_t& event
        );
    };

    ///
    /// get_dungeon_id()
    /// @return The Dungeon ID for the given name.
    /// Returns d_MAX if it is invalid
    ///
    dungeon_id get_dungeon_id(std::string_view name);

    const std::array<dungeon, d_MAX> dungeons{{
        { d_foo, "Foo", 10, 50, {
            {
                { 100, 10 },
                { 0, 0 },
                { 0, 0 },
                { 0, 0 },
                { 25, 500 },
                { 0, 0 },
                { 0, 0 },
                { 0, 0 },
                { 0, 0 },
                { 100, 20 },
                { 10, 1 },
            }
        }, "https://media.npr.org/assets/img/2014/08/07/monkey-selfie_custom-6624e8356a07d872997e801d7a04aa8cdc8fbaac.jpg?s=1100&c=50&f=jpeg"}
    }};
}

#endif
