#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <string_view>
#include <dpp/dpp.h>
#include "item.h"
#include "player.h"

namespace adr {
    namespace shop {
        enum Id {
            gaydrien,
            adriresource,
            job,
            resource,
            everything,

            MAX
        };

        extern const std::array<std::string, MAX> names;
        adr::shop::Id getId(const std::string_view str);
        extern const std::array<adr::Item::Id, MAX> itemIcons;
        extern const std::array<std::string, MAX> jokes;
       
        const std::array<dpp::embed, adr::shop::MAX> makeEmbedsList();
        extern const std::array<dpp::embed, adr::shop::MAX> embeds;

        dpp::message getMessage(adr::shop::Id screen = adr::shop::everything);

        namespace product {
            enum Id {
                gaytocoin,
                adriresourcetrade,
                tierupgrade,
                jobchange,
                resourceexchange,

                MAX
            };

            extern const std::array<std::string, MAX> names;
        };

        void buy(adr::Player& player, const std::string& productName, const dpp::slashcommand_t& event);
    }
}

#endif
