#ifndef KOLIN_DUNGEON_H
#define KOLIN_DUNGEON_H

#include <array>
#include <dpp/appcommand.h>
#include <optional>
#include <dpp/message.h>
#include <string_view>
#include "item.h"
#include "inventory.h"
#include "player.h"

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

        ///
        /// description
        /// @brief The description for the boss. It can include lore,
        /// requirements, or drop information
        ///
        const std::string_view desc;

        ///
        /// win_msg
        /// @brief The message that displays when beating the boss
        ///
        const std::string_view win_msg;

        ///
        /// lose_msg
        /// @brief The message that displays when losing
        ///
        const std::string_view lose_msg;

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

        using win_info = std::tuple<bool, int, int>;

        /// 
        /// try_win()
        /// @brief Attempt to win a boss fight
        /// @param p The player that is attempting. It is used for checking 
        /// their inventory.
        /// @return A tuple containing if it was a win, what the player
        /// rolled, and what the player would've had to roll
        /// 
        win_info try_win(adr::Player& p, bool dungeon_potion) const;

        using drop_info = std::tuple<bool, int, int>;

        ///
        /// try_drop()
        /// @brief Attempt to get a drop from a boss fight
        /// @param i Which item to try for
        /// @return A tuple containing if it was dropped, what the player 
        /// rolled, and what the player would've had to roll
        ///
        drop_info try_drop(item_id i, bool kismet_feather) const;

        using fight_info = std::tuple<std::array<drop_info, i_MAX>, inventory, win_info>;

        ///
        /// fight()
        /// @brief Handles boss fight RNG and item distribution
        /// @param uuid The player that is fighting
        /// @return A tuple containing win_info, an array of drop_info,
        /// and an inventory containing the item drops
        ///
        fight_info fight(
            const dpp::snowflake& uuid,
            bool dungeon_potion,
            bool kismet_feather
        ) const;

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
                std::string_view _desc,
                std::string_view _win_msg,
                std::string_view _lose_msg,
                const std::array<std::pair<uint8_t, int>, i_MAX>& _item_chances,
                std::string_view _thumbnail_url
        ) 
                : id(_id)
                , name(_name)
                , desc(_desc)
                , win_msg(_win_msg)
                , lose_msg(_lose_msg)
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
        dpp::message buy(
                const dpp::snowflake& uuid,
                bool dungeon_potion,
                bool bonzo_mask,
                bool kismet_feather,
                bool dungeon_pass
        ) const;

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
            "Description",
            "Win msg",
            "Lose msg"
            {{
            //  { chance, amount }
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

                { 0, 0 }, // i_dungeon_potion,
                { 0, 0 }, // i_kismet_feather,
            }}, 
            "https://media.tenor.com/NsJeBDRCh_MAAAAM/puffer-fish.gif"
        },
        */

        { 
            d_bonzo, "Bonzo", 10, 100, 
            "> Gratz for making it this far, but I'm basically unbeatable."
            "\n> I don't even need to fight, this is the life!"
            "\n> I can summon lots of Undead."
            "\n> Check this out!"
            "\n\nBonzo drops the **Bonzo Mask**, which allows you to "
            "reattempt a failed dungeon (you still have to pay)",
            "> Alright, maybe I'm just weak after all.."
            "\n> But my masters are a lot stronger."
            "\n> Just you wait...",
            "this doesnt exist!",
            {{
            //  { chance, amount }
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
            "> This is where the journey ends for you, Adventurers."
            "\n> The last few who tried fighting me are now in those Crypts."
            "\n> If you can beat my Undeads, I'll personally grant you the privilege to replace them."
            "\n> ARISE, MY CREATIONS!"
            "\n> This should be interesting."
            "\n\nScarf drops the **Loving Scarf**, which makes a Bonzo Mask "
            "infinitely reusable when applied with `/use`",
            "> Whatever..."
            "\n> You'll never beat my teacher.."
            "\n> His technique.. is too advanced..",
            "> I always knew I would win...I'm a genius."
            "\n> But don't worry I'll add you to my collection."
            "\n> In a way, you'll live forever...",
            {{
            //  { chance, amount }
                { 0, 0 }, // i_carrot,
                { 0, 0 }, // i_gemstone,
                { 0, 0 }, // i_wood,
                { 0, 0 }, // i_fish,
                { 0, 0 }, // i_rottenflesh,
                { 0, 0 }, // i_book,
                { 0, 0 }, // i_potion,
                { 0, 0 }, // i_adriencoin,
                { 0, 0 }, // i_gaydriencoin,
                { 100, 10 }, // i_adriresource,
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
            "> I was burdened with terrible news recently..."
            "\n> My most talented student..."
            "\n> I'll show you real power!",
            "> What?! My Guardian power is unbeatable!"
            "\n> How could you... I needed more power..."
            "\n> I can't let my Master hear about this.. he'll kill m-",
            "> Now that you have fallen, my student has been avenged."
            "\n> Picking a fight with me and my experiments will only lead to your demise!"
            "\n> I give you an F-!",
            {{
            //  { chance, amount }
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
            "> Welcome Adventurers! I am Thorn, the Spirit! And host of the Vegan Trials!"
            "\n> My Arena is the most entertaining place in all the Catacombs!"
            "\n> Today you'll be our spectacle."
            "\n> Dance! Dance with my Spirit animals!"
            "\n> And may you perish in a delightful way!"
            "\n\nThorn drops the **Spirit Sceptre**, which increases win rate by 5%",
            "> This is it...where shall I go now?"
            "\n> All those memories will be lost in time, like tears in rain..."
            "\n> Congratulations humans, you may pass.",
            "> Wonderful performance humans, truly beautiful."
            "\n> How one dies shows how one lived. And it was your masterpiece."
            "\n> What spirit shall you become? Yes! A rabbit!"
            "\n> What spirit shall you become? Yes! A chihuahua!"
            "\n> What spirit shall you become? Yes! A sheep!"
            "\n> What spirit shall you become? Yes! A chicken!",
            {{
            //  { chance, amount }
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
            "> Welcome, you arrive right on time. I am Livid, the Master of Shadows."
            "\n> This Orb you see, is Thorn, or what is left of him."
            "\n> In a way, I have to thank you for finally getting rid of him."
            "\n> I can now turn those Spirits into shadows of myself, identical to their creator."
            "\n> I respect you for making it to here, but I'll be your undoing."
            "\n\nLivid drops the **Livid Dagger**, which is required to defeat Sadan",
            "> Impossible! How did you figure out which one I was?!"
            "\n> If you think this is the end, you are so wrong!"
            "\n> My shadows are everywhere, THEY WILL FIND YOU!!",
            "> Oh my shadows! Perfect in every way!"
            "\n> Stronger than my subjects and more loyal...I am invincible..."
            "\n> Well fought humans, no longer you will disrupt the silence of the Catacombs",
            {{
            //  { chance, amount }
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
                { 0, 0 }, // i_giant_sword,
                { 0, 0 }, // i_wither_shield,
                { 0, 0 }, // i_shadow_warp,
                { 0, 0 }, // i_implosion,
                { 0, 0 }, // i_hyperion,

                { 0, 0 }, // i_dungeon_potion,
                { 20, 1 }, // i_kismet_feather,
            }}, 
            "https://wiki.hypixel.net/images/3/34/SkyBlock_npcs_livid.png"
        },
        { 
            d_sadan, "Sadan", 50, 30, 
            "> So you made it all the way here...and you wish to defy me? Sadan?!"
            "\n> The audacity! I have been the ruler of these floors for a hundred years!"
            "\n> I am the bridge between this realm and the world below! You shall not pass!"
            "\n\nA Livid Dagger is required to fight Sadan"
            "\nSadan drops the **Giant Sword**, which is required for Necron "
            "and increases win rate by 5%",
            "> NOOOOOOOOO!!! THIS IS IMPOSSIBLE!!"
            "\n> FATHER, FORGIVE ME!!!",
            "> It was inevitable. You are fighting forces beyond your imagination."
            "\n> No adventurer can see what lays beyond this floor."
            "\n> Rest in peace.",
            {{
            //  { chance, amount }
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
                { 0, 0 }, // i_wither_shield,
                { 0, 0 }, // i_shadow_warp,
                { 0, 0 }, // i_implosion,
                { 0, 0 }, // i_hyperion,

                { 0, 0 }, // i_dungeon_potion,
                { 25, 1 }, // i_kismet_feather,
            }}, 
            "https://wiki.hypixel.net/images/1/17/SkyBlock_npcs_sadan.png"
        },
        { 
            d_necron, "Necron", 70, 0, 
            "> Finally, I heard so much about you. The Eye likes you very much."
            "\n> You went further than any human before, congratulations."
            "\n> I'm afraid, your journey ends now."
            "\n> My master and I spent centuries building this factory...and this army."
            "\n> I won't allow you to destroy it all now."
            "\n> Goodbye."
            "\n\nBoth the Spirit Sceptre and the Giant Sword are required to fight Necron"
            "\nNecron drops the **Hyperion**, as well as three scrolls to improve it"
            "\nAn unscrolled Hyperion increases win rate by 10%, "
            "and each scroll applies gives an additional 5%",
            "> All this, for nothing..."
            "\n> I understand your words now, my master."
            "\n> The Catacombs... are no more.",
            "> FINALLY! This took way too long."
            "\n> Now that you're a Ghost, can you help me clean up?",
            {{
            //  { chance, amount }
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

                { 0, 0 }, // i_dungeon_potion,
                { 30, 1 }, // i_kismet_feather,
            }}, 
            "https://wiki.hypixel.net/images/a/a4/Minecraft_entities_wither.png"
        },
    }};
}

#endif
