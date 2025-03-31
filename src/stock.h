#ifndef STOCK_H
#define STOCK_H

#include <string>

namespace adr
{
    class Stock
    {
    public:
        enum Id {
            jerrys_carrots,
            divans_gems,
            the_wood_and_fish_co,
            aatroxs_slaying_co,
            berts_knowledge,
            the_brewers,
            stonks_unlimited,
            shens_investment,
        };

    private:
        const std::string& m_name{};
        const Id m_id{};
        // Value in Adriencoin
        int m_value{};
        // The time for all of the stocks, in days
        static unsigned int day;

    public:
        static Stock& getStock(Id id); 
        // Increments the day by 1
        static void newDay();
        static unsigned int getDay();


    };
}

#endif
