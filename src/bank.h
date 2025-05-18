#ifndef ADR_BANK
#define ADR_BANK

#include <dpp/appcommand.h>
#include <dpp/nlohmann/json.hpp>
#include "inventory.h"
#include <string>

namespace adr
{
  class Bank
  {
  protected:
    inventory m_inv;

    bool m_isActive{ false };

    // cost in adriencoin to activate your bank account
    constexpr static int costAdriencoin{ 50 };

  public:
    // json should be a reference to the entire player object, not just
    // the bank object
    void saveJSON(nlohmann::json& json) const;

    // json should be a reference to the entire player object, not just
    // the bank object
    void loadJSON(const nlohmann::json& json);

    // returns an embed with a list of all non zero items in the bank
    dpp::embed getEmbed() const;

    bool isActive() const { return m_isActive; }

    static void addSlashCommands(
        dpp::cluster& bot,
        std::vector<dpp::slashcommand>& commandList);

    void handleSlashCommand(
        dpp::cluster& bot,
        const dpp::slashcommand_t& event);

    void doInterest();
  };
}

#endif
