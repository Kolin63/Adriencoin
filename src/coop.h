#ifndef ADR_COOP_H
#define ADR_COOP_H

#include "bank.h"
#include <dpp/snowflake.h>

namespace adr
{
  class Coop : public Bank
  {
  protected:
    // cost in adriencoin to activate your bank account
    constexpr static int costAdriencoin{ 150 };

    // update the local bank based on the owners bank
    void updateLocal();

    Coop& getRemote() const;

  public:
    // the owner of the coop
    std::uint64_t m_ownerUUID{};

    // the players in the coop (including the owner)
    std::vector<std::uint64_t> m_players{};

    // outgoing invitations (represented as uuids)
    // these are not saved into json
    // these are for the coop bank, not the individual player
    std::vector<std::uint64_t> m_outgoingInvites{};

    // Turns true on the first time the player tries to delete it 
    // Turns false if the player cancels the deletion
    bool m_deletionMark{ false };

    // json should be a reference to the entire player object, not just
    // the bank object
    void saveJSON(nlohmann::json& json) const;

    // json should be a reference to the entire player object, not just
    // the bank object
    void loadJSON(const nlohmann::json& json, const dpp::snowflake& playerUUID);

    // returns an embed with a list of all non zero items in the bank
    dpp::embed getEmbed() const;

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
