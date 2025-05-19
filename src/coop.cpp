#include "coop.h"
#include "cache.h"
#include "inventory.h"
#include <dpp/appcommand.h>
#include <dpp/invite.h>
#include <dpp/message.h>
#include <dpp/snowflake.h>
#include <unistd.h>

void adr::Coop::updateLocal() {
  const Coop& remote{ getRemote() };
  m_isActive = remote.m_isActive;
  m_inv = remote.m_inv;
  m_players = remote.m_players;
}

adr::Coop& adr::Coop::getRemote() const {
  adr::Player& owner{ adr::cache::getPlayerFromCache(m_ownerUUID) };
  return owner.m_coop;
}

void adr::Coop::saveJSON(nlohmann::json& json) const {
  std::cout << "adr::Coop::saveJSON() called\n";

  json["coop"]["inv"] = m_inv;
  json["coop"]["isActive"] = m_isActive;
  json["coop"]["owner"] = m_ownerUUID;
  json["coop"]["players"] = m_players;

  std::cout << "adr::Coop::saveJSON() finished\n";
}

void adr::Coop::loadJSON(const nlohmann::json& json, const dpp::snowflake& playerUUID)
{
  std::cout << "adr::Coop::loadJSON() called\n";

  try {
    m_inv = json.at("coop").at("inv");
    m_isActive = json.at("coop").at("isActive");
    m_ownerUUID = json.at("coop").at("owner");
    m_players = json.at("coop").at("players").get<std::vector<std::uint64_t>>();
  }
  catch (nlohmann::json::out_of_range&) {
    for (std::size_t i{}; i < i_MAX; ++i) {
      try {
        m_inv[i] = json.at("coop").at("inv").at(i);
      }
      catch (nlohmann::json::out_of_range&) {
        m_inv[i] = 0;
      }
    }
    m_isActive = false;
    m_ownerUUID = 0;
    m_players = {};
  }

  std::cout << "adr::Coop::loadJSON() finished\n";
}

dpp::embed adr::Coop::getEmbed() const {
  Coop& remote{ getRemote() };

  dpp::embed embed{};
  embed
    .set_title("Your Co-Op Bank")
    .set_color(0x00bb88)
    .set_description("Owner: " + cache::getElementFromCache(remote.m_ownerUUID).username + "\nPlayers:\n")
    .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/chromebook.jpg");

  for (const long unsigned int& i : remote.m_players) {
    embed.description += "* " + cache::getElementFromCache(i).username + '\n';
  }

  embed.description += "\nContents:\n" + getNonZeroItems(remote.m_inv);

  return embed;
}

void adr::Coop::addSlashCommands(
    dpp::cluster& bot,
    std::vector<dpp::slashcommand>& commandList)
{
  // Main command
  dpp::slashcommand bank{ "coop", "Access your Co-Op Bank", bot.me.id };

  // Activate:
  bank.add_option(dpp::command_option{ 
      dpp::co_sub_command, 
      "activate", 
      "Activate your co-op bank (see price in #bank)" });

  // Deposit:
  bank.add_option(dpp::command_option{
      dpp::co_sub_command,
      "deposit",
      "Desposit Items" }
      .add_option(dpp::command_option{
        dpp::co_string,
        "item",
        "The name of the item (see /lsitem for help)",
        true
        })
      .add_option(dpp::command_option{
        dpp::co_integer,
        "amount",
        "The amount of items to deposit",
        true
        })
      );

  // Withdraw:
  bank.add_option(dpp::command_option{
      dpp::co_sub_command,
      "withdraw",
      "Withdraw Items" }
      .add_option(dpp::command_option{
        dpp::co_string,
        "item",
        "The name of the item (see /lsitem for help)",
        true
        })
      .add_option(dpp::command_option{
        dpp::co_integer,
        "amount",
        "The amount of items to withdraw",
        true
        })
      );

  // View:
  bank.add_option(dpp::command_option{
      dpp::co_sub_command,
      "view",
      "View the contents of your bank" });

  // Invite:
  bank.add_option(dpp::command_option{
      dpp::co_sub_command,
      "invite",
      "Add a player to your Co-Op bank" }
      .add_option(dpp::command_option{
        dpp::co_user,
        "player",
        "The player to invite",
        true })
      );

  // Accept:
  bank.add_option(dpp::command_option{
      dpp::co_sub_command,
      "accept",
      "Accept and join a Co-Op bank to you were invited to" }
      .add_option(dpp::command_option{
        dpp::co_user,
        "player",
        "The player that invited you",
        true })
      );

  // Remove:
  bank.add_option(dpp::command_option{
      dpp::co_sub_command,
      "remove",
      "Remove a player from your Co-Op bank" }
      .add_option(dpp::command_option{
        dpp::co_user,
        "player",
        "The player that invited you",
        true })
      );

  // Transfer:
  bank.add_option(dpp::command_option{
      dpp::co_sub_command,
      "transfer",
      "Transfer ownership to another player" }
      .add_option(dpp::command_option{
        dpp::co_user,
        "player",
        "The player to transfer ownership to",
        true })
      );

  commandList.push_back(bank);
}

void adr::Coop::handleSlashCommand(
    dpp::cluster& bot,
    const dpp::slashcommand_t& event)
{
  updateLocal();

  // The subcommand action
  // Ex `/coop view`, the action is 'view'
  const std::string_view action{ 
    event.command.get_command_interaction().options[0].name 
  };

  adr::Player& player{ 
    cache::getPlayerFromCache(event.command.usr.id) 
  };

  if (action == "activate") {
    if (player.inv(i_adriencoin) < costAdriencoin) [[unlikely]] {
      event.reply(dpp::message{ "You can't afford that! (" + get_emoji(e_adriencoin) + ' ' + std::to_string(costAdriencoin) + " Adriencoin)" }.set_flags(dpp::m_ephemeral));
      return;
    }
    if (m_ownerUUID == player.uuid()) [[unlikely]] {
      event.reply(dpp::message{ "You already own a Co-Op bank!" }.set_flags(dpp::m_ephemeral));
      return;
    }
    if (m_ownerUUID != 0) [[unlikely]] {
      event.reply(dpp::message{ "You are already in a Co-Op bank!" }.set_flags(dpp::m_ephemeral));
      return;
    }
    if (m_isActive) [[unlikely]] {
      event.reply(dpp::message{ "Your bank is already active!" }.set_flags(dpp::m_ephemeral));
      return;
    }

    player.changeInv(i_adriencoin, -1 * costAdriencoin);
    m_isActive = true;
    m_ownerUUID = player.uuid();
    m_players = { player.uuid() };
    event.reply(dpp::message{ "Activated your Co-Op bank!" });
    return;
  }

  Coop& remote{ getRemote() };

  if (!remote.m_isActive && action != "accept") {
    event.reply(dpp::message{ "Your coop bank is not active (/coop activate)" }.set_flags(dpp::m_ephemeral));
    return;
  }

  if (action == "deposit") {
    const item_id item{ get_item_id(std::get<std::string>(event.get_parameter("item"))) };
    const std::int64_t amt{ std::get<std::int64_t>(event.get_parameter("amount")) };

    if (amt > std::numeric_limits<int>::max()) [[unlikely]] {
      event.reply(dpp::message{ "Are you trying to overflow the integer limit to bug the bot?" }.set_flags(dpp::m_ephemeral));
      return;
    }

    const int iamt{ static_cast<int>(amt) };

    if (player.inv(item) < iamt) [[unlikely]] {
      event.reply(dpp::message{ "You don't have enough of that item!" }.set_flags(dpp::m_ephemeral));
      return;
    }

    if (iamt <= 0) [[unlikely]] {
      event.reply(dpp::message{ "The amount must be greater than 0" }.set_flags(dpp::m_ephemeral));
      return;
    }

    player.changeInv(item, -1 * iamt);
    remote.m_inv[item] += iamt;

    event.reply(dpp::message{ "Succesfully deposited " + get_emoji(item) + ' ' + std::to_string(iamt) + ' ' + std::string{ item_names[item] } }.set_flags(dpp::m_ephemeral));
    return;
  }

  if (action == "withdraw") {
    const item_id item{ get_item_id(std::get<std::string>(event.get_parameter("item"))) };
    const std::int64_t amt{ std::get<std::int64_t>(event.get_parameter("amount")) };

    if (amt > std::numeric_limits<int>::max()) [[unlikely]] {
      event.reply(dpp::message{ "Are you trying to overflow the integer limit to bug the bot?" }.set_flags(dpp::m_ephemeral));
      return;
    }

    const int iamt{ static_cast<int>(amt) };

    if (remote.m_inv[item] < iamt) [[unlikely]] {
      event.reply(dpp::message{ "The bank doesn't have enough of that item!" }.set_flags(dpp::m_ephemeral));
      return;
    }

    if (iamt <= 0) [[unlikely]] {
      event.reply(dpp::message{ "The amount must be greater than 0" }.set_flags(dpp::m_ephemeral));
      return;
    }

    remote.m_inv[item] -= iamt;
    player.changeInv(item, iamt);

    event.reply(dpp::message{ "Succesfully withdrew " + get_emoji(item) + ' ' + std::to_string(iamt) + ' ' + std::string{ item_names[item] } }.set_flags(dpp::m_ephemeral));
    return;
  }

  if (action == "view") {
    event.reply(dpp::message{ remote.getEmbed() }.set_flags(dpp::m_ephemeral));
    return;
  }

  if (action == "invite") {
    if (player.uuid() != remote.m_ownerUUID) [[likely]] {
      event.reply(dpp::message{ "Only owners can perform that action" }.set_flags(dpp::m_ephemeral));
      return;
    }

    dpp::snowflake inviteeUUID{ std::get<dpp::snowflake>(event.get_parameter("player")) };
    playerCacheElement& pce{ cache::getElementFromCache(inviteeUUID) };
    playerCacheElement& ownerPCE{ cache::getElementFromCache(remote.m_ownerUUID) };

    remote.m_outgoingInvites.push_back(inviteeUUID);

    event.reply(dpp::message{ "Succesfully invited " + pce.username + " to the Co-Op.\n" + pce.username + " can join with `/coop accept " + ownerPCE.username + "`." });
    return;
  }

  if (action == "accept") {
    if (m_ownerUUID == player.uuid()) [[unlikely]] {
      event.reply(dpp::message{ "You already own a Co-Op bank!" }.set_flags(dpp::m_ephemeral));
      return;
    }
    if (m_ownerUUID != 0) [[unlikely]] {
      event.reply(dpp::message{ "You are already in a Co-Op bank!" }.set_flags(dpp::m_ephemeral));
      return;
    }
    if (m_isActive) [[unlikely]] {
      event.reply(dpp::message{ "Your bank is already active!" }.set_flags(dpp::m_ephemeral));
      return;
    }

    dpp::snowflake inviterUUID{ std::get<dpp::snowflake>(event.get_parameter("player")) };
    Player& inviterPlayer{ cache::getPlayerFromCache(inviterUUID) };
    Coop& inviterCoop{ inviterPlayer.m_coop.getRemote() };
    std::vector<std::uint64_t>& list{ inviterCoop.m_outgoingInvites };

    for (std::size_t i{}; i < list.size(); ++i) {
      if (list[i] != player.uuid()) [[likely]] continue;
      m_ownerUUID = inviterCoop.m_ownerUUID;
      inviterCoop.m_players.push_back(player.uuid());
      list.erase(list.begin() + i);
      updateLocal();
      event.reply(dpp::message{ "Succesfully joined their Co-Op" }.set_flags(dpp::m_ephemeral));
      return;
    }
    event.reply(dpp::message{ "You don't have an invite in that Co-Op!" }.set_flags(dpp::m_ephemeral));
    return;
  }

  if (action == "remove") {
    if (player.uuid() != remote.m_ownerUUID) [[likely]] {
      event.reply(dpp::message{ "Only owners can perform that action" }.set_flags(dpp::m_ephemeral));
      return;
    }

    dpp::snowflake rmUUID{ std::get<dpp::snowflake>(event.get_parameter("player")) };
    std::vector<std::uint64_t>& list{ player.m_coop.getRemote().m_players };
    
    for (std::size_t i{}; i < list.size(); ++i) {
      if (list[i] != rmUUID) [[likely]] continue;
      list.erase(list.begin() + i);
      updateLocal();
      event.reply(dpp::message{ "Succesfully removed that player" }.set_flags(dpp::m_ephemeral));
      return;
    }
    event.reply(dpp::message{ "That player is not in your Co-Op" }.set_flags(dpp::m_ephemeral));
  }

  if (action == "transfer") {
    if (player.uuid() != remote.m_ownerUUID) [[likely]] {
      event.reply(dpp::message{ "Only owners can perform that action" }.set_flags(dpp::m_ephemeral));
      return;
    }

    dpp::snowflake transUUID{ std::get<dpp::snowflake>(event.get_parameter("player")) };
    Player& transPlayer{ cache::getPlayerFromCache(transUUID) };
    std::vector<std::uint64_t>& list{ player.m_coop.getRemote().m_players };

    for (std::size_t i{}; i < list.size(); ++i) {
      if (list[i] != transUUID) [[likely]] continue;
      remote.m_ownerUUID = transUUID;
      transPlayer.m_coop.updateLocal();
      updateLocal();
      event.reply(dpp::message{ "Succesfully transferred ownership to that player" }.set_flags(dpp::m_ephemeral));
      return;
    }
    event.reply(dpp::message{ "That player is not in your Co-Op" }.set_flags(dpp::m_ephemeral));
  }
}

void adr::Coop::doInterest() {
  Coop& remote{ getRemote() };
  remote.m_inv[i_adriencoin] += static_cast<int>(remote.m_inv[i_adriencoin] * 0.1);
}
