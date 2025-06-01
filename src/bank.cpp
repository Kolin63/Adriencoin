#include "bank.h"
#include "cache.h"
#include "inventory.h"
#include "item.h"
#include <dpp/message.h>
#include <dpp/nlohmann/json.hpp>
#include <dpp/appcommand.h>
#include <limits>
#include <string>

void adr::Bank::saveJSON(nlohmann::json& json) const {
  std::cout << "adr::Bank::saveJSON() called\n";

  json["bank"]["inv"] = m_inv;
  json["bank"]["isActive"] = m_isActive;

  std::cout << "adr::Bank::saveJSON() finished\n";
}

void adr::Bank::loadJSON(const nlohmann::json& json)
{
  std::cout << "adr::Bank::loadJSON() called\n";

  try {
    m_inv = json.at("bank").at("inv");
  }
  catch (nlohmann::json::out_of_range&) {
    for (std::size_t i{}; i < i_MAX; ++i) {
      try {
        m_inv[i] = json.at("bank").at("inv").at(i);
      }
      catch (nlohmann::json::out_of_range&) {
        m_inv[i] = 0;
      }
    }
  }

  try { m_isActive = json.at("bank").at("isActive"); }
  catch(nlohmann::json::out_of_range&) { m_isActive = false; }

  std::cout << "adr::Bank::loadJSON() finished\n";
}

dpp::embed adr::Bank::getEmbed() const {
  dpp::embed embed{};
  embed
    .set_title("Your Bank")
    .set_color(0x005eb8)
    .set_description(getNonZeroItems(m_inv))
    .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/chromebook.jpg");

  return embed;
}


void adr::Bank::addSlashCommands(
    dpp::cluster& bot,
    std::vector<dpp::slashcommand>& commandList)
{
  // Main command
  dpp::slashcommand bank{ "bank", "Access your Bank", bot.me.id };

  // Activate:
  bank.add_option(dpp::command_option{ 
      dpp::co_sub_command, 
      "activate", 
      "Activate your bank (see price in #bank)" });

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

  commandList.push_back(bank);
}

void adr::Bank::handleSlashCommand(
    dpp::cluster& bot,
    const dpp::slashcommand_t& event)
{
  // The subcommand action
  // Ex `/bank view`, the action is 'view'
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
    if (m_isActive) [[unlikely]] {
      event.reply(dpp::message{ "Your bank is already active!" }.set_flags(dpp::m_ephemeral));
      return;
    }

    player.changeInv(i_adriencoin, -1 * costAdriencoin);
    m_isActive = true;
    event.reply(dpp::message{ "Activated your bank!" });
    return;
  }

  if (!m_isActive) {
    event.reply(dpp::message{ "Your bank is not active (/bank activate)" }.set_flags(dpp::m_ephemeral));
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
    m_inv[item] += iamt;

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

    if (m_inv[item] < iamt) [[unlikely]] {
      event.reply(dpp::message{ "The bank doesn't have enough of that item!" }.set_flags(dpp::m_ephemeral));
      return;
    }

    if (iamt <= 0) [[unlikely]] {
      event.reply(dpp::message{ "The amount must be greater than 0" }.set_flags(dpp::m_ephemeral));
      return;
    }

    m_inv[item] -= iamt;
    player.changeInv(item, iamt);

    event.reply(dpp::message{ "Succesfully withdrew " + get_emoji(item) + ' ' + std::to_string(iamt) + ' ' + std::string{ item_names[item] } }.set_flags(dpp::m_ephemeral));
    return;
  }

  if (action == "view") {
    event.reply(dpp::message{ getEmbed() }.set_flags(dpp::m_ephemeral));
    return;
  }
}

void adr::Bank::doInterest() {
  m_inv[i_adriencoin] += static_cast<int>(m_inv[i_adriencoin] * 0.1);
}
