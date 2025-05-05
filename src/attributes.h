#ifndef KOLIN_ADRIENCOIN_ATTRIBUTES_H
#define KOLIN_ADRIENCOIN_ATTRIBUTES_H

#include "emoji.h"
#include <bitset>
#include <dpp/nlohmann/json.hpp>
#include <string>

namespace adr
{
    ///
    /// attributes
    /// @brief A struct containing attributes for a player, usually related 
    /// to the states of items.
    ///
    struct attributes
    {
        ///
        /// save_json()
        /// @brief Save attributes to a JSON player save file
        /// @param data A const reference to the JSON object containing only
        /// the attributes. The object should be the entire player save, not
        /// only the attributes object
        ///
        void save_json(nlohmann::json& data) const;

        ///
        /// load_json()
        /// @brief Load attributes from a JSON player save file
        /// @param data A const reference to the JSON object containing only
        /// the attributes. The object should be the entire player save, not
        /// only the attributes object
        ///
        void load_json(const nlohmann::json& data);

        ///
        /// list()
        /// @return A human readable list of all attributes that are not
        /// their default values.
        ///
        std::string list() const;

        ///
        /// atrib
        /// @brief A struct that contains the necessary information for an
        /// attribute
        /// @param def The default value
        /// @param nam The attribute name
        /// @param emo The attribute emoji
        /// @param val The value, should not be initialized by the user
        ///
        template <typename T>
        struct atrib
        {
            const T def{};
            const std::string_view nam{ "NULL" };
            const adr::emoji_id emo{ adr::e_fish };
            T val{ def };
        };

        // ====================================================================
        //
        // Below are the actual attributes
        //
        // ====================================================================

        // atrib<bool> example{ false, "example", e_stockofstonks };

        // True if the loving scarf has been used
        atrib<bool> bonzo_love{ false, "bonzo_love", e_bonzo_mask };

        // True if:
        // * the player has a bonzo mask
        // * the player has failed the last dungeon they did
        // * the player did not use a bonzo mask on their last dungeon
        // * the player is on dungeon cooldown
        atrib<bool> bonzo_can_use{ false, "bonzo_can_use", e_bonzo_mask };

        atrib<bool> wither_shield{ false, "wither_shield", e_wither_shield };
        atrib<bool> shadow_warp{ false, "shadow_warp", e_shadow_warp };
        atrib<bool> implosion{ false, "implosion", e_implosion };
    };
}

#endif
