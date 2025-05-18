#ifndef PLAYER_H
#define PLAYER_H

#include <dpp/dpp.h>
#include <array>
#include <ctime>
#include "item.h"
#include "job.h"
#include "inventory.h"
#include "daily.h"
#include "attributes.h"
#include "statistics.h"
#include "role.h"
#include "bank.h"

namespace adr {

    class Player {
    private:
        const dpp::snowflake m_uuid{};

        adr::Job::Id m_job{ adr::Job::MAX };
        std::time_t m_lastWorked{ 0 };
        static constexpr std::time_t workCooldownSeconds{ 60 * 60 * 6 }; // 6 hours

        std::time_t m_lastFought{ 0 };
        static constexpr std::time_t fightCooldownSeconds{ 60 * 60 * 1 }; // 1 hour

#ifdef DEBUG
    public: 
        enum godmode : std::uint8_t {
            g_none,
            g_items,
            g_no_cooldown,
            g_all,
        } m_godmode;
    private:
#endif

        // the player's inventory, where the index is the item ID and the value is the amount (int)
        inventory m_inv{};

    public:
        attributes m_atr{};

        Statistics m_stat{};

        FameRank m_fameRank{};

        Bank m_bank{};

        // the hardest dungeon the player has done
        // an integer to not have a circular dependency
        short m_high_dung{ -1 };
    private:

        adr::daily::Title m_title{ adr::daily::t_none };

    public:
        Player(const dpp::snowflake& uuid);
        Player(const dpp::snowflake& uuid, const inventory& inv);
        ~Player();

        const dpp::snowflake& uuid() const { return m_uuid; }

        void save() const;
        void load();
        void print() const;
        const dpp::embed viewEmbed() const;
        bool exists() const;
        static bool exists(const dpp::snowflake& uuid);

        adr::Job::Id job() const { return m_job; }
        void setJob(adr::Job::Id job);

        std::time_t lastWorked() const { return m_lastWorked; }
        void updateLastWorked();
        void setLastWorked(const std::time_t t) { m_lastWorked = t; }
        std::time_t nextWork() const;
        std::string nextWorkTimestamp() const;

        std::time_t lastFought() const { return m_lastFought; }
        void updateLastFought();
        void setLastFought(const std::time_t t) { m_lastFought = t; }
        std::time_t nextFight() const;
        std::string nextFightTimestamp() const;

        void setInv(const inventory& inv) { m_inv = inv; }
        void setInv(const adr::item& item) { m_inv[item.id] = item.amount; }
        void setInv(const adr::item_id& id, int amount) { m_inv[id] = amount; }
        void changeInv(const adr::item_id& id, int difference) { m_inv[id] += difference; }
        void changeInv(const inventory& difference);
        void subtractInv(const inventory& difference);
        bool canBuy(const inventory& cost);

        inventory inv() const { return m_inv; }
        int inv(adr::item_id id) const { return m_inv[id]; }

        adr::daily::Title getTitle() const { return m_title; }
        void setTitle(adr::daily::Title title) { m_title = title; }

        int& operator[](int index);
        const int& operator[](int index) const;

        void setRole(RoleID role, bool status);
    };
}

#endif
