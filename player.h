#ifndef PLAYER_H
#define PLAYER_H

#include <dpp/dpp.h>
#include <array>
#include "item.h"
#include "job.h"

using Inventory = std::array<int, adr::Item::MAX>;
static constexpr int CURRENT_SAVE_VERSION{ 1 };

namespace adr {

    class Player {
    private:
        // the savedata version, so that the data can be converted if needed
        short m_version{};
        const dpp::snowflake m_uuid{};
        adr::Job::Id m_job{ adr::Job::MAX };
        // the player's inventory, where the index is the item ID and the value is the amount (int)
        Inventory m_inv{};

    public:
        Player(const dpp::snowflake& uuid);
        Player(const dpp::snowflake& uuid, const Inventory& inv);
        ~Player();

        void save() const;
        void load();
        void print() const;
        bool exists() const;
        static bool exists(const dpp::snowflake& uuid);

        adr::Job::Id job() const { return m_job; }
        void setJob(adr::Job::Id job) { m_job = job; }

        int& operator[](int index);
        const int& operator[](int index) const;
    };
}

#endif
