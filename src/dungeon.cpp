#include "Random.h"
#include "dungeon.h"
#include "emoji.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "cache.h"
#include <dpp/appcommand.h>
#include <dpp/message.h>
#include <optional>
#include <sstream>

bool adr::dungeon::try_win() const
{
    // Get a random number between 0 and 100, inclusive
    const uint8_t roll{ Random::get<uint8_t>(0, 100) };

    // If the win chance is less than or equal to the roll, 
    // then the fight was won. 
    return win_chance <= roll;
}

bool adr::dungeon::try_drop(adr::item_id i) const
{
    // Reference for easy typing
    const uint8_t& chance{ item_chances[i].first };

    // If the item never drops or always drops, we don't need
    // to waste time generating a random number
    if (chance == 0 || chance == 100)
        return chance;

    // Get a random number between 0 and 100, inclusive
    const uint8_t roll{ Random::get<uint8_t>(0, 100) };

    // If the drop chance is less than or equal to the roll, 
    // then the item was dropped.
    return chance <= roll;
}

std::optional<inventory> adr::dungeon::fight(const dpp::snowflake& uuid) const
{
    // If the boss fight was lost, return null
    if (!try_win()) return std::nullopt;

    // The player that is fighting
    adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

    // The temporary Inventory we will be adding to
    inventory inv{};

    // Otherwise, we can roll for item drops
    for (std::size_t i{}; i < adr::i_MAX; ++i) 
    {
        // The item we are rolling for
        const adr::item_id item{ static_cast<adr::item_id>(i) };

        // If we fail the drop, continue
        if (!try_drop(item)) continue;

        // Otherwise, add it to the player inventory
        
        // Get the amount
        const int amt{ item_chances[item].second };

        // Change the Player's Inventory 
        player.changeInv(item, amt);

        // Change the temporary inventory
        inv[item] += amt;
    }

    // Return the temporary inventory so that the caller can tell the
    // player what items they got
    return inv;
}

dpp::embed adr::dungeon::get_embed() const
{
    // Make the embed
    dpp::embed embed{};

    // Set the title and thumbnail
    embed
        .set_title(name)
        .set_thumbnail(thumbnail_url);

    // Make a String Stream, which is easier than concatenating strings
    std::stringstream ss{};

    // Put in the Floor, Cost, and the header for Drops
    ss << "Floor " << id << "\n"
        << "Win Chance: " << static_cast<int>(win_chance) << "\n\n"
        << "**Costs: **" << price << ' ' << adr::get_emoji(e_adriencoin) << '\n'
        << "**Drops:**\n";

    // Put in the Potential Drops
    for (std::size_t i{}; i < adr::i_MAX; ++i)
    {
        // Shortcut
        const int amt{ item_chances[i].second };

        // Only show drops that actually exist
        if (amt <= 0) continue;

        // Shortcuts
        const int chance{ item_chances[i].first };
        const item_id item{ static_cast<item_id>(i) };

        // Put in the emoji, name, amount, and chance
        ss << get_emoji(item) << ' ' << item_names[i] << ": "
            << amt << " (" << chance << "%)\n";
    }

    // Set the embed's description to the string stream
    embed.set_description(ss.str());

    // Return the embed to be sent to the user
    return embed;
}

int adr::dungeon::get_price() const
{
    return price;
}

dpp::message adr::dungeon::buy(const dpp::snowflake& uuid) const
{
    adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

    if (player.inv(adr::i_adriencoin) < price) {
        return dpp::message{ "You can't afford that!" }
        .set_flags(dpp::m_ephemeral);
    }

    player.changeInv(adr::i_adriencoin, price);

    const std::optional<inventory> fight_results{ fight(uuid) };

    dpp::embed embed{};

    std::stringstream ss{};

    ss << name << " (Floor " << id << ")\n\n"
        << "**Costs:** " << price << ' ' << get_emoji(adr::e_adriencoin);

    if (!fight_results.has_value()) {
        // TODO: Make it have an option to use Bonzo Mask

        embed.set_title("Dungeon Lost!")
            .set_thumbnail(thumbnail_url)
            .set_color(0xEE0000)
            .set_description(ss.str());

        return dpp::message{}.add_embed(embed);
    }

    const inventory& inv{ fight_results.value() };

    embed.set_title("Dungeon Won!")
        .set_thumbnail(thumbnail_url)
        .set_color(0x00EE00);

    ss << "\n\n**Item Drops:**\n";

    for (std::size_t i{}; i < adr::i_MAX; ++i) 
    {
        // Shortcut
        const int amt{ item_chances[i].second };

        // Only show drops that actually exist
        if (amt <= 0) continue;

        // Shortcuts
        const item_id item{ static_cast<item_id>(i) };

        ss << get_emoji(item) << ": " << inv[i] << '\n';
    }

    embed.set_description(ss.str());
    return dpp::message{}.add_embed(embed);
}

void adr::dungeon::add_slash_commands(
        dpp::cluster& bot, 
        std::vector<dpp::slashcommand>& command_list) 
{
    dpp::slashcommand sc{ "dungeon", "Dungeons", bot.me.id };

    dpp::command_option bosses{ dpp::co_string, "boss", "Which Boss", true };

    for (adr::dungeon d : adr::dungeons)
    {
        std::string str{ d.name };
        bosses.add_choice(dpp::command_option_choice{ str, str });
    }

    sc.add_option(
            dpp::command_option{ dpp::co_sub_command, "view", "View a Boss" }
            .add_option(bosses)
    );

    sc.add_option(
            dpp::command_option{ dpp::co_sub_command, "fight", "Fight a Boss" }
            .add_option(bosses)
    );

    command_list.push_back(sc);

    std::cout << "Done making dungeon slash commands\n";
}

void adr::dungeon::handle_slash_command(
        dpp::cluster& bot, 
        const dpp::slashcommand_t& event)
{
    const std::string_view action{ 
        event.command.get_command_interaction().options[0].name 
    };

    const std::string_view name{ 
        std::get<std::string>(event.get_parameter("boss")) 
    };

    const adr::dungeon_id id{ adr::get_dungeon_id(name) };

    if (id == adr::d_MAX) {
        event.reply(dpp::message{ "That is an invalid name!" }
        .set_flags(dpp::m_ephemeral));
    }

    const adr::dungeon& dung{ adr::dungeons[id] };

    if (action == "view") {
        event.reply(dung.get_embed());
        return;
    }

    if (action == "fight") {
        event.reply(dung.buy(event.command.usr.id));
        return;
    }
}

adr::dungeon_id adr::get_dungeon_id(std::string_view name)
{
    for (const adr::dungeon& d : adr::dungeons)
    {
        if (d.name == name) return d.id;
    }

    return adr::d_MAX;
};
