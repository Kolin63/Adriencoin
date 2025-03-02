#ifndef PLAYER_H
#define PLAYER_H

#include <dpp/dpp.h>
#include <array>
#include <ctime>
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
        adr::Job::Id m_tempJob{ adr::Job::MAX }; // used for when the player is selecting a job
        std::time_t m_lastWorked{ 0 };
        static constexpr std::time_t workCooldownSeconds{ 60 * 60 * 6 }; // 6 hours

        // the player's inventory, where the index is the item ID and the value is the amount (int)
        Inventory m_inv{};

    public:
        Player(const dpp::snowflake& uuid);
        Player(const dpp::snowflake& uuid, const Inventory& inv);
        ~Player();

        void save() const;
        void load();
        void print() const;
        const dpp::embed viewEmbed(dpp::cluster& bot) const;
        bool exists() const;
        static bool exists(const dpp::snowflake& uuid);

        adr::Job::Id job() const { return m_job; }
        void setJob(adr::Job::Id job) { m_job = job; }
        adr::Job::Id tempJob() const { return m_tempJob; }
        void setTempJob(adr::Job::Id job) { m_tempJob = job; }

        std::time_t lastWorked() const { return m_lastWorked; }
        void updateLastWorked() { m_lastWorked = std::time(0); }
        void setLastWorked(const std::time_t t) { m_lastWorked = t; }
        std::time_t nextWork() const { return m_lastWorked - std::time(0) + workCooldownSeconds; } // relative time, not since epoch
        std::string nextWorkTimestamp() const { return dpp::utility::timestamp(std::time(0) + nextWork(), dpp::utility::tf_relative_time); }

        void setInv(const Inventory& inv) { m_inv = inv; }
        void setInv(const adr::Item& item) { m_inv[item.id] = item.amount; }
        void setInv(const adr::Item::Id& id, int amount) { m_inv[id] = amount; }
        void changeInv(const adr::Item::Id& id, int difference) { m_inv[id] += difference; }

        Inventory inv() const { return m_inv; }
        int inv(adr::Item::Id id) const { return m_inv[id]; }

        int& operator[](int index);
        const int& operator[](int index) const;
    };
}

#endif
