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

        static Id getId(const std::string& str);
        Id getId() const { return m_id; };
        static Stock& getStock(Id id); 
        static Stock& getStock(const std::string& str);
        static void saveJSON();
        static void parseJSON();

        // Used when a player works
        static void updateValue(adr::Job::Id id);

        // Changes the stock's value by diff
        void changeValue(int diff) { m_value += diff; };
        // Changes the stock's value by the percent
        // Note: 1.00 = 100%, 0.50 = 50%, etc
        void changeValue(double percent) { m_value = static_cast<int>(m_value * percent); }
        int getValue() const { return m_value; };
        // To be called when a stock is bought or sold
        // Diff: the difference in stock outstanding
        // > Positive means stock was bought
        // > Negative means stock was sold
        void changeOutstanding(int diff);
        int getOutstanding() const { return m_outstanding; };
        int getUnissued() const { return m_unissued; };

        // Increments the day by 1
        static void newDay();
        static unsigned int getDay();

        static void addSlashCommands(dpp::cluster& bot, std::vector<dpp::slashcommand>& commandList);
    };
}

#endif
