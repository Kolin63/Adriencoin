#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <array>
#include "job.h"

namespace adr
{
    class Stock
    {
    public:
        enum Id {
            jerrys_carrots,
            divans_gems,
            jacks_lumber,
            geralds_fish,
            aatroxs_slaying,
            berts_knowledge,
            the_brewers,
            stonks_unlimited,

            MAX
        };

    private:
        std::string m_name{};
        Id m_id{};
        // Value in Adriencoin
        int m_value{};
        int m_outstanding{};
        int m_unissued{};
        
        // The time for all of the stocks, in days
        static unsigned int day;
        // List of all stocks, gotten via getStock()
        static std::array<Stock, MAX> stocks;

    public:
        Stock() = default;
        Stock(const std::string& name, Id id, int value, int outstanding, int unissued)
            : m_name{ name }, m_id{ id }, m_value{ value }, m_outstanding{ outstanding }, m_unissued{ unissued }
        {
        };

        static Stock& getStock(Id id); 
        static void saveJSON();
        static void parseJSON();

        // Used when stocks are bought
        static void updateValue(Id id, int amount, int mux = 5);
        // Used when a player works
        static void updateValue(adr::Job::Id id);

        static void changeValue(Id id, int diff);
        static int getValue(Id id);

        // Increments the day by 1
        static void newDay();
        static unsigned int getDay();

        static void addSlashCommands(dpp::cluster& bot, std::vector<dpp::slashcommand>& commandList);
    };
}

#endif
