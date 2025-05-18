#include "bank.h"
#include "inventory.h"
#include <dpp/nlohmann/json.hpp>

void adr::Bank::saveJSON(nlohmann::json& json) {
  std::cout << "adr::Bank::saveJSON() called\n";

  json["bank"]["inv"] = m_inv;

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

  std::cout << "adr::Bank::loadJSON() finished\n";
}

bool adr::Bank::deposit(item_id item, uint32_t amt, inventory& playerInv) {
  if (playerInv[item] < amt) return false;
  playerInv[item] -= amt;
  m_inv[item] += amt;
  return true;
}

bool adr::Bank::withdraw(item_id item, uint32_t amt, inventory& playerInv) {
  if (m_inv[item] < amt) return false;
  m_inv[item] -= amt;
  playerInv[item] += amt;
  return true;
}

std::string adr::Bank::list() {
  return getNonZeroItems(m_inv);
}
