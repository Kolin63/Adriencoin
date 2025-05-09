#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <dpp/dpp.h>

namespace adr {
    namespace shop {
        dpp::message getMessage(const std::string name = "everything");
        dpp::message buy(
                const dpp::snowflake& uuid, 
                const std::string& productName, 
                const std::string& subprodName,
                const std::string& resultName = "", 
                int times = 1
                );
    }
}

#endif
