#include <dpp/nlohmann/json.hpp>
#include "inventory.h"
#include <string>

namespace adr
{
  class Bank
  {
  protected:
    inventory m_inv;

  public:
    // json should be a reference to the entire player object, not just
    // the bank object
    void saveJSON(nlohmann::json& json);

    // json should be a reference to the entire player object, not just
    // the bank object
    void loadJSON(const nlohmann::json& json);

    // for when the player puts items from their inventory into the 
    // banks inventory
    bool deposit(item_id item, uint32_t amt, inventory& playerInv);

    // for when the player gets items from the banks inventory and puts them 
    // in their inventory
    bool withdraw(item_id item, uint32_t amt, inventory& playerInv);

    // returns a list of all non zero items in the bank
    std::string list();
  };
}
