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
        d_bonzo,
        d_scarf,
        d_the_professor,
        d_thorn,
        d_livid,
        d_sadan,
        d_necron,

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

    /*
    dungeon(
            dungeon_id _id,
            std::string_view _name,
            int _price,
            uint8_t _win_chance, 
            const std::array<std::pair<uint8_t, int>, i_MAX>& _item_chances,
            std::string_view _thumbnail_url
    ) 
    */

    const std::array<dungeon, d_MAX> dungeons{{
        /*
        { 
            d_foo, "Foo", int_cost, int_win_chance, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 0, 0 }, // i_adriresource,
                { 0, 0 }, // i_null,
                { 0, 0 }, // i_stockofstonks,

                { 0, 0 }, // i_jerrys_carrots,
                { 0, 0 }, // i_divans_gems,
                { 0, 0 }, // i_jacks_lumber,
                { 0, 0 }, // i_geralds_fish,
                { 0, 0 }, // i_aatroxs_slaying,
                { 0, 0 }, // i_berts_knowledge,
                { 0, 0 }, // i_the_brewers,
                { 0, 0 }, // i_stonks_unlimited,

                { 0, 0 }, // i_bonzo_mask,
                { 0, 0 }, // i_loving_scarf,
                { 0, 0 }, // i_spirit_sceptre,
                { 0, 0 }, // i_livid_dagger,
                { 0, 0 }, // i_giant_sword,
                { 0, 0 }, // i_wither_shield,
                { 0, 0 }, // i_shadow_warp,
                { 0, 0 }, // i_implosion,
                { 0, 0 }, // i_hyperion,
            }}, 
            "https://media.tenor.com/NsJeBDRCh_MAAAAM/puffer-fish.gif"
        },
        */

        { 
            d_bonzo, "Bonzo", 10, 100, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 100, 5 }, // i_adriresource,
                { 0, 0 }, // i_null,
                { 0, 0 }, // i_stockofstonks,

                { 0, 0 }, // i_jerrys_carrots,
                { 0, 0 }, // i_divans_gems,
                { 0, 0 }, // i_jacks_lumber,
                { 0, 0 }, // i_geralds_fish,
                { 0, 0 }, // i_aatroxs_slaying,
                { 0, 0 }, // i_berts_knowledge,
                { 0, 0 }, // i_the_brewers,
                { 0, 0 }, // i_stonks_unlimited,

                { 10, 1 }, // i_bonzo_mask,
            }}, 
            "https://wiki.hypixel.net/images/7/7f/SkyBlock_npcs_bonzo_undead.png"
        },
        {
            d_scarf, "Scarf", 20, 80, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 100, 1 }, // i_adriresource,
                { 0, 0 }, // i_null,
                { 0, 0 }, // i_stockofstonks,

                { 0, 0 }, // i_jerrys_carrots,
                { 0, 0 }, // i_divans_gems,
                { 0, 0 }, // i_jacks_lumber,
                { 0, 0 }, // i_geralds_fish,
                { 0, 0 }, // i_aatroxs_slaying,
                { 0, 0 }, // i_berts_knowledge,
                { 0, 0 }, // i_the_brewers,
                { 0, 0 }, // i_stonks_unlimited,

                { 0, 0 }, // i_bonzo_mask,
                { 10, 1 }, // i_loving_scarf,
            }}, 
            "https://wiki.hypixel.net/images/a/a0/SkyBlock_entities_scarf.png"
        },
        {
            d_the_professor, "The Professor", 30, 60, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 10, 300 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 10, 20 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 100, 15 }, // i_adriresource,
            }}, 
            "https://wiki.hypixel.net/images/9/99/SkyBlock_npcs_professor.png"
        },
        { 
            d_thorn, "Thorn", 40, 40, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 100, 20 }, // i_adriresource,
                { 0, 0 }, // i_null,
                { 0, 0 }, // i_stockofstonks,

                { 0, 0 }, // i_jerrys_carrots,
                { 0, 0 }, // i_divans_gems,
                { 0, 0 }, // i_jacks_lumber,
                { 0, 0 }, // i_geralds_fish,
                { 0, 0 }, // i_aatroxs_slaying,
                { 0, 0 }, // i_berts_knowledge,
                { 0, 0 }, // i_the_brewers,
                { 0, 0 }, // i_stonks_unlimited,

                { 0, 0 }, // i_bonzo_mask,
                { 0, 0 }, // i_loving_scarf,
                { 10, 1 }, // i_spirit_sceptre,
            }}, 
            "https://wiki.hypixel.net/images/4/4f/Minecraft_entities_ghast.gif"
        },
        { 
            d_livid, "Livid", 50, 30, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 100, 25 }, // i_adriresource,
                { 0, 0 }, // i_null,
                { 0, 0 }, // i_stockofstonks,

                { 0, 0 }, // i_jerrys_carrots,
                { 0, 0 }, // i_divans_gems,
                { 0, 0 }, // i_jacks_lumber,
                { 0, 0 }, // i_geralds_fish,
                { 0, 0 }, // i_aatroxs_slaying,
                { 0, 0 }, // i_berts_knowledge,
                { 0, 0 }, // i_the_brewers,
                { 0, 0 }, // i_stonks_unlimited,

                { 0, 0 }, // i_bonzo_mask,
                { 0, 0 }, // i_loving_scarf,
                { 0, 0 }, // i_spirit_sceptre,
                { 10, 1 }, // i_livid_dagger,
            }}, 
            "https://wiki.hypixel.net/images/3/34/SkyBlock_npcs_livid.png"
        },
        { 
            d_sadan, "Sadan", 50, 30, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 100, 30 }, // i_adriresource,
                { 0, 0 }, // i_null,
                { 0, 0 }, // i_stockofstonks,

                { 0, 0 }, // i_jerrys_carrots,
                { 0, 0 }, // i_divans_gems,
                { 0, 0 }, // i_jacks_lumber,
                { 0, 0 }, // i_geralds_fish,
                { 0, 0 }, // i_aatroxs_slaying,
                { 0, 0 }, // i_berts_knowledge,
                { 0, 0 }, // i_the_brewers,
                { 0, 0 }, // i_stonks_unlimited,

                { 0, 0 }, // i_bonzo_mask,
                { 0, 0 }, // i_loving_scarf,
                { 0, 0 }, // i_spirit_sceptre,
                { 0, 0 }, // i_livid_dagger,
                { 10, 1 }, // i_giant_sword,
            }}, 
            "https://wiki.hypixel.net/images/1/17/SkyBlock_npcs_sadan.png"
        },
        { 
            d_necron, "Necron", 70, 0, 
            {{
            //  { chance, price }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 20, 1 }, // i_gaydriencoin,
                { 100, 50 }, // i_adriresource,
                { 0, 0 }, // i_null,
                { 0, 0 }, // i_stockofstonks,

                { 0, 0 }, // i_jerrys_carrots,
                { 0, 0 }, // i_divans_gems,
                { 0, 0 }, // i_jacks_lumber,
                { 0, 0 }, // i_geralds_fish,
                { 0, 0 }, // i_aatroxs_slaying,
                { 0, 0 }, // i_berts_knowledge,
                { 0, 0 }, // i_the_brewers,
                { 0, 0 }, // i_stonks_unlimited,

                { 0, 0 }, // i_bonzo_mask,
                { 0, 0 }, // i_loving_scarf,
                { 0, 0 }, // i_spirit_sceptre,
                { 0, 0 }, // i_livid_dagger,
                { 0, 0 }, // i_giant_sword,
                { 10, 1 }, // i_wither_shield,
                { 10, 1 }, // i_shadow_warp,
                { 10, 1 }, // i_implosion,
                { 5, 1 }, // i_hyperion,
            }}, 
            "https://wiki.hypixel.net/images/a/a4/Minecraft_entities_wither.png"
        },
    }};
}

#endif
