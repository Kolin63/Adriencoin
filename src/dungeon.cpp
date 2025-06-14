#include "Random.h"
#include "dungeon.h"
#include "emoji.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "cache.h"
#include <dpp/appcommand.h>
#include <dpp/emoji.h>
#include <dpp/exception.h>
#include <dpp/message.h>
#include <sstream>
#include <string>
#include <variant>

adr::dungeon::win_info adr::dungeon::try_win(adr::Player& p, bool dungeon_potion) const
{
  // Get a random number between 0 and 100, inclusive
  const int roll{ Random::get<int>(0, 100) };

  // Only use a dungeon pot if the player wants to and they have one
  const bool use_dpot{ 
    dungeon_potion && (p.inv(i_dungeon_potion) > 0)
  };

  // This is where we can apply modifiers from mayors, items, 
  // or anything else
  // It is an int so we don't have to worry about overflow
  const int chance{
    100 - (

        win_chance 
        + (p.inv(i_spirit_sceptre) > 0) * 5
        + (p.inv(i_giant_sword) > 0)    * 5
        + (p.inv(i_hyperion) > 0)       * 10
        + (p.m_atr.wither_shield.val)   * 5
        + (p.m_atr.shadow_warp.val)     * 5
        + (p.m_atr.implosion.val)       * 5
        + (use_dpot)                    * 5

        )
  };

  // If the player used a dungeon pot, decrement it by one
  p.setInv(i_dungeon_potion, 
      (use_dpot) * -1 + p.inv(i_dungeon_potion));

  // Debug
  std::cout << p.uuid() << " dungeoning, chance is " << chance 
    << ", base chance is " << static_cast<short>(win_chance) 
    << ", rolled a " << roll << ", status is " << (chance >= roll) << '\n';

  // If the roll is greater than or equal to the chance (armor class), 
  // then the fight was won. 
  return { 
    roll >= chance,
         roll,
         chance,
  };
}

adr::dungeon::drop_info adr::dungeon::try_drop(adr::item_id i, bool kismet_feather) const
{
  // This is where we can apply modifiers from mayors, items, or anything
  // else
  // It is an int so we don't have to worry about overflow
  const int chance{ 
    100 - (

        item_chances[i].first 
        + (kismet_feather * 5)

        )
  };

  // Get a random number between 0 and 100, inclusive
  const int roll{ Random::get<int>(0, 100) };

  std::cout << "try_drop() for item " << i 
    << ", chance is " << chance << ", roll is " << roll << '\n';

  // If the roll is greater than or equal to the chance (armor class), 
  // then the item was dropped.
  return { 
    roll >= chance,
         roll,
         chance
  };
}

adr::dungeon::fight_info adr::dungeon::fight(
    const dpp::snowflake& uuid,
    bool dungeon_potion,
    bool kismet_feather
    ) const
{
  // The player that is fighting
  adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

  // If the boss fight was lost, return null
  const win_info win_info{ try_win(player, dungeon_potion) };
  if (!std::get<bool>(win_info)) return {
    {}, {}, win_info
  };

  // The temporary Inventory we will be adding to
  inventory inv{};

  // The temporary drop info list
  std::array<drop_info, i_MAX> drop_infos{};

  // Otherwise, we can roll for item drops
  for (std::size_t i{}; i < adr::i_MAX; ++i) 
  {
    // The item we are rolling for
    const adr::item_id item{ static_cast<adr::item_id>(i) };

    const drop_info drop_info{ try_drop(item, kismet_feather) };

    drop_infos[i] = drop_info;

    // If we fail the drop, continue
    if (!std::get<bool>(drop_info)) continue;

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
  return {
    drop_infos,
      inv,
      win_info
  };
}

dpp::embed adr::dungeon::get_embed() const
{
  // Make the embed
  dpp::embed embed{};

  // Set the title and thumbnail
  embed
    .set_title(name)
    .set_thumbnail(thumbnail_url)
    .set_color(0xf3b5b8);

  // Make a String Stream, which is easier than concatenating strings
  std::stringstream ss{};

  // Put in the Floor, Cost, and the header for Drops
  ss << desc << "\n\nFloor " << id + 1 << "\n"
    << "Win Chance: " << static_cast<int>(win_chance) << "%\n\n"
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

dpp::message adr::dungeon::buy(
    const dpp::snowflake& uuid,
    bool dungeon_potion,
    bool bonzo_mask,
    bool kismet_feather,
    bool dungeon_pass
    ) const
{
  // The player that is buying
  adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

  // Check that they can afford it
  if (!dungeon_pass && player.inv(adr::i_adriencoin) < price) {
    return dpp::message{ "You can't afford that!" }
    .set_flags(dpp::m_ephemeral);
  }
  if (dungeon_pass && player.inv(adr::i_dungeon_pass) <= 0) {
    return dpp::message{ "You don't have a dungeon pass!" }
    .set_flags(dpp::m_ephemeral);
  }
  if (dungeon_potion && player.inv(i_dungeon_potion) <= 0) {
    return dpp::message{ "You don't have any dungeon potions!" }
    .set_flags(dpp::m_ephemeral);
  }
  if (bonzo_mask && player.inv(i_bonzo_mask) <= 0) {
    return dpp::message{ "You don't have any bonzo masks!" }
    .set_flags(dpp::m_ephemeral);
  }
  if (kismet_feather && player.inv(i_kismet_feather) <= 0) {
    return dpp::message{ "You don't have any kismet feathers!" }
    .set_flags(dpp::m_ephemeral);
  }

  // Check for other requirements
  if (player.m_high_dung + 1 < id) { 
    return dpp::message{ "You must beat the previous dungeon!" }
    .set_flags(dpp::m_ephemeral);
  }
  if (id == d_sadan && (player.inv(i_livid_dagger) <= 0)) {
    return dpp::message{ "You need a livid dagger!" }
    .set_flags(dpp::m_ephemeral);
  }
  if (id == d_necron && (player.inv(i_giant_sword) <= 0)) {
    return dpp::message{ "You need a Giant Sword!" }
    .set_flags(dpp::m_ephemeral);
  }
  if (id == d_necron && (player.inv(i_spirit_sceptre) <= 0)) {
    return dpp::message{ "You need a Spirit Sceptre!" }
    .set_flags(dpp::m_ephemeral);
  }

  // Check that they are not on cooldown
  if (player.nextFight() >= 0 && !bonzo_mask) {
    return dpp::message{ 
      "You can fight next " + player.nextFightTimestamp() 
    }.set_flags(dpp::m_ephemeral);
  }

  // Bonzo Mask
  if (bonzo_mask && !player.m_atr.bonzo_can_use.val) {
    return dpp::message{ "You can't use your bonzo mask right now!" }
    .set_flags(dpp::m_ephemeral);
  }

  bool bonzo_used{ false };
  if (player.m_atr.bonzo_can_use.val && player.nextFight() >= 0) {
    player.m_atr.bonzo_can_use.val = false;
    bonzo_used = true;

    if (!player.m_atr.bonzo_love.val) {
      player.changeInv(i_bonzo_mask, -1);
    }
  }
  else if (player.m_atr.bonzo_can_use.val && player.nextFight() < 0) {
    player.m_atr.bonzo_can_use.val = false;
  }

  // If they can, subtract the price from their inventory 
  player.changeInv(adr::i_adriencoin,  !dungeon_pass * -1 * price);
  player.changeInv(adr::i_dungeon_pass, dungeon_pass * -1);

  // And update their fight cooldown
  player.updateLastFought();

  // Fight the boss
  const fight_info fight_results{ fight(uuid, dungeon_potion, kismet_feather) };
  const win_info& win_info{ std::get<adr::dungeon::win_info>(fight_results) };
  const std::array<drop_info, i_MAX>& drop_infos{ std::get<std::array<drop_info, i_MAX>>(fight_results) };

  // Make the embed that we will be using for the message
  dpp::embed embed{};
  embed.set_thumbnail(thumbnail_url);

  // Make the String Stream that we will be using to put text in the embed
  std::stringstream ss{};

  // If the player lost the fight
  if (!std::get<bool>(win_info)) {
    ss << lose_msg << "\n\n" 
      << dpp::emoji{ "x" }.get_mention() << ' '
      << name << " (Floor " << id + 1 << ")\n"
      << dpp::emoji{ "game_die" }.get_mention()
      << " You rolled a " << std::get<1>(win_info)
      << ", but needed a " << std::get<2>(win_info) << "\n\n"
      << "**Costed:** " << 
      (dungeon_pass ? " 1 Dungon Pass " + get_emoji(adr::e_paper) 
       : std::to_string(price) + ' ' + get_emoji(adr::e_adriencoin));

    // Bonzo Mask can be used if:
    // * the player has a bonzo mask
    // * the player has failed the last dungeon they did
    // * the player did not use a bonzo mask on their last dungeon
    // * the player is on dungeon cooldown
    player.m_atr.bonzo_can_use.val = 
      player.inv(i_bonzo_mask) > 0
      && !bonzo_used
      && player.nextFight() > 0;

    ss << "\n\n";

    if (player.m_atr.bonzo_can_use.val) {
      ss << get_emoji(e_bonzo_mask) << " Bonzo Mask Available\n";
    }

    if (kismet_feather) {
      ss << get_emoji(e_kismet_feather) << " Kismet Feather not used\n";
    }

    // Set embed Title, Color, Description
    embed.set_title("Dungeon Lost!")
      .set_color(0xEE0000)
      .set_description(ss.str());

    // Return to the caller to be sent to the player
    return dpp::message{}.add_embed(embed);
  }

  // If the player won the fight

  ss << win_msg << "\n\n" 
    << dpp::emoji{ "white_check_mark" }.get_mention() << ' '
    << name << " (Floor " << id + 1 << ")\n"
    << dpp::emoji{ "game_die" }.get_mention()
    << " You rolled a " << std::get<1>(win_info)
    << ", and needed a " << std::get<2>(win_info) << "!\n\n"
    << "**Costed:** " << 
    (dungeon_pass ? " 1 Dungon Pass " + get_emoji(adr::e_paper) 
     : std::to_string(price) + ' ' + get_emoji(adr::e_adriencoin));

  // Put the item drops into an inventory
  const inventory& inv{ std::get<inventory>(fight_results) };

  // Update highest dungeon
  if (id > player.m_high_dung)
    player.m_high_dung = id;

  // Set title and color
  embed.set_title("Dungeon Won!")
    .set_color(0x00EE00);

  // Header for Item Drops
  ss << "\n\n**Item Drops:**\n";

  // Add item drops to the embed
  for (std::size_t i{}; i < adr::i_MAX; ++i) 
  {
    // Shortcut
    const int amt{ item_chances[i].second };

    // Only show drops that actually exist
    //
    // Even if we didn't drop any for this fight, we should still show
    // that we didn't drop any
    // 
    // However, this will filter out any drops that never drop from
    // the boss.
    if (amt <= 0) continue;

    // Shortcuts
    const item_id item{ static_cast<item_id>(i) };

    const std::string_view bold{ inv[i] > 0 ? "**" : "*" };

    // Add item emoji, name, and amount
    ss << bold << get_emoji(item) << ' ' << adr::item_names[i] << ": " 
      << inv[i] << bold 
      << " Rolled: " << std::get<1>(drop_infos[i])
      << " Needed: " << std::get<2>(drop_infos[i])
      << '\n';
  }

  ss << '\n';

  // Handle kismet feather stuff
  // Note: the actual chance increase is done in try_drop()
  if (kismet_feather) {
    player.changeInv(i_kismet_feather, -1);
    ss << get_emoji(e_kismet_feather) << " Kismet Feather Used\n";
  }

  // Set embed description
  embed.set_description(ss.str());

  // stats
  switch (id) {
    case d_bonzo: ++player.m_stat.bonzo.value; break;
    case d_scarf: ++player.m_stat.scarf.value; break;
    case d_the_professor: ++player.m_stat.theProfessor.value; break;
    case d_thorn: ++player.m_stat.thorn.value; break;
    case d_livid: ++player.m_stat.livid.value; break;
    case d_sadan: ++player.m_stat.sadan.value; break;
    case d_necron: ++player.m_stat.necron.value; break;
    default: break;
  }

  // Return to the caller to be sent to the player 
  return dpp::message{}.add_embed(embed);
}

void adr::dungeon::add_slash_commands(
    dpp::cluster& bot, 
    std::vector<dpp::slashcommand>& command_list) 
{
  // The main slashcommand (`/dungeon`)
  dpp::slashcommand sc{ "dungeon", "Dungeons", bot.me.id };

  // The command option that includes the names of all bosses
  dpp::command_option bosses{ dpp::co_string, "boss", "Which Boss", true };

  // Put the boss names into the command option
  for (adr::dungeon d : adr::dungeons)
  {
    std::string str{ d.name };
    bosses.add_choice(dpp::command_option_choice{ str, str });
  }

  // `/dungeon fight`
  sc.add_option(
      dpp::command_option{ dpp::co_sub_command, "fight", "Fight a Boss" }
      .add_option(bosses)
      .add_option(dpp::command_option{ dpp::co_boolean, "dungeon_potion", "Use a Dungeon Potion", false })
      .add_option(dpp::command_option{ dpp::co_boolean, "bonzo_mask", "Use a Bonzo Mask", false })
      .add_option(dpp::command_option{ dpp::co_boolean, "kismet_feather", "Use a Kismet Feather", false })
      .add_option(dpp::command_option{ dpp::co_boolean, "dungeon_pass", "Use a Dungeon Pass", false })
      );

  // add 'all' option to boss list for view command
  bosses.add_choice(dpp::command_option_choice{ "all", "all" });

  // `/dungeon view`
  sc.add_option(
      dpp::command_option{ dpp::co_sub_command, "view", "View a Boss" }
      .add_option(bosses)
      );

  // Add `/dungeon` to the command list
  command_list.push_back(sc);

  // Logging
  std::cout << "Done making dungeon slash commands\n";
}

void adr::dungeon::handle_slash_command(
    dpp::cluster& bot, 
    const dpp::slashcommand_t& event)
{
  // The subcommand action
  // Ex `/dungeon view`, the action is 'view'
  const std::string_view action{ 
    event.command.get_command_interaction().options[0].name 
  };

  // Name of the boss in a string
  const std::string_view name{ 
    std::get<std::string>(event.get_parameter("boss")) 
  };

  // dungeon view is only for colin because its big
  if (action == "view" && event.command.usr.id != 488335709883727882) {
    event.reply(
        dpp::message{ "stop doing that just look at #dungeons" }
        .set_flags(dpp::m_ephemeral)
        );
    return;
  }

  // Handle viewing all bosses
  if (name == "all" && action == "view") {
    // The message we will return
    dpp::message msg{};

    // Add every embed to the message
    for (const adr::dungeon& d : adr::dungeons) {
      msg.add_embed(d.get_embed());
    }

    // Reply and early return
    event.reply(msg);
    return;
  }

  // Boss ID
  const adr::dungeon_id id{ adr::get_dungeon_id(name) };

  // Check if the Boss Name was invalid
  if (id == adr::d_MAX) {
    event.reply(dpp::message{ "That is an invalid name!" }
        .set_flags(dpp::m_ephemeral));
  }

  // Dungeon class from the ID
  const adr::dungeon& dung{ adr::dungeons[id] };

  // Actual handling of the commands below

  if (action == "view") {
    event.reply(dung.get_embed());
    return;
  }

  if (action == "fight") {
    bool dungeon_pot{ false };
    bool bonzo_mask{ false };
    bool kismet_feather{ false };
    bool dungeon_pass{ false };

    try { dungeon_pot = std::get<bool>(event.get_parameter("dungeon_potion")); }
    catch (const std::bad_variant_access&) {};
    try { bonzo_mask = std::get<bool>(event.get_parameter("bonzo_mask")); }
    catch (const std::bad_variant_access&) {};
    try { kismet_feather = std::get<bool>(event.get_parameter("kismet_feather")); }
    catch (const std::bad_variant_access&) {};
    try { dungeon_pass = std::get<bool>(event.get_parameter("dungeon_pass")); }
    catch (const std::bad_variant_access&) {};

    event.reply(dung.buy(event.command.usr.id, dungeon_pot, bonzo_mask, kismet_feather, dungeon_pass));
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
