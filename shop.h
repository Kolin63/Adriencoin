#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <string_view>
#include <dpp/dpp.h>
#include "item.h"

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

        dpp::message getMessage(adr::shop::Id screen = adr::shop::resource);
    }
}

#endif
