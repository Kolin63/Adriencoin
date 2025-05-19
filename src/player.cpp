#include <dpp/nlohmann/json.hpp>
#include <dpp/cluster.h>
#include <filesystem>
#include "player.h"
#include "item.h"
#include "job.h"
#include "cache.h"
#include "util.h"
#include "botToken.h"

adr::Player::Player(const dpp::snowflake& uuid) 
    : m_uuid{ uuid }
{
    std::cout << "Player constructor called for " << uuid << '\n';
    if (exists()) {
      std::cout << uuid << " savedata does exist. Loading!\n";
        load();
    }
    else {
        std::cout << uuid << " savedata does not exist. Creating new one.\n";
        save();
    }
}

adr::Player::Player(const dpp::snowflake& uuid, const inventory& inv) 
    : m_uuid{ uuid }
{
    if (!exists()) {
        m_inv = inv;
        std::cout << uuid << " savedata does not exist. Creating new one.\n";
    }
    save();
}

adr::Player::~Player()
{
    save();
}

void adr::Player::save() const
{
    using json = nlohmann::json;
    std::cout << "Saving player " << m_uuid << '\n';

    std::filesystem::create_directory("../playerdata");
    const std::string filename{ "./../playerdata/" + std::to_string(m_uuid) + ".json" };

    json data;

    data["uuid"] = static_cast<uint64_t>(m_uuid);
    data["job"] = m_job;
    data["lastWorked"] = m_lastWorked;
    data["inv"] = m_inv;
    data["title"] = m_title;
    data["fame"] = m_fameRank;
    data["highdung"] = m_high_dung;

    m_atr.save_json(data);
    m_stat.saveJSON(data);
    m_bank.saveJSON(data);
    m_coop.saveJSON(data);

    std::ofstream fs(filename);
    fs << std::setw(4) << data << std::endl;

    fs.close();
}

void adr::Player::load()
{
    using json = nlohmann::json;
    std::cout << "Loading player " << m_uuid << '\n';
    std::string filename{ "./../playerdata/" + std::to_string(m_uuid) + ".json" };

    if (!exists()) {
        std::cerr << "Error: " << filename << " does not exist.\n";
        return;
    }

    std::ifstream fs{ filename };
    json data;

    try {
        data = json::parse(fs);
    }
    catch (const json::parse_error& e) {
        std::cerr << "adr::Player::load() json parse error: " << e.what() << '\n';
        fs.close();
        return;
    }

    if (data["uuid"] != static_cast<uint64_t>(m_uuid)) {
        std::cerr << "Error: " << filename << ' ' << data["uuid"] << " does not match " << m_uuid << '\n';
        fs.close();
        return;
    }

    m_job = data["job"];
    m_lastWorked = data["lastWorked"];

    try {
        m_inv = data["inv"];
    }
    catch (...) {
        // If the JSON's inventory is outdated, we will need 
        // to put it into the user's inventory one at a time
        // and leave the new items as zeros
        std::cout << m_uuid << "'s save possibly outdated. attempting to fix." << '\n';
        m_inv = {};
        for (std::size_t i{}; i < data["inv"].size(); ++i) {
            try {
                m_inv[i] = data["inv"][i];
            }
            catch (...) {
                std::cout << m_uuid << " inventory item " << i << " is being"
                    << " updated\n";
                m_inv[i] = 0;
            }
        }
        std::cout << "Succesfully updated inventory of " << m_uuid << '\n';
    }

    try {
        m_high_dung = data["highdung"];
    } 
    catch (...) {
        m_high_dung = -1;
    }

    m_title = data.value("title", adr::daily::t_none);
    m_fameRank = data.value("fame", f_none);

    m_atr.load_json(data);
    m_stat.loadJSON(data);
    m_bank.loadJSON(data);
    m_coop.loadJSON(data, m_uuid);

    fs.close();
}

void adr::Player::print() const
{
    std::cout << m_uuid << "'s Inventory:\n"
        << ((m_job != adr::Job::MAX) ? adr::Job::jobs[m_job].name : "no job")
        << " last worked: " << m_lastWorked << '\n'
        << "next work: " << nextWork() << '\n'
        << "current time: " << std::time(0) << '\n';

    for (std::size_t i{}; i < m_inv.size(); ++i) {
        std::cout << adr::item_names[i] << " (" << i << "):\t" << m_inv[i] << '\n';
    }
    std::cout << '\n';
}

const dpp::embed adr::Player::viewEmbed() const
{
    const adr::playerCacheElement& pce{ adr::cache::getElementFromCache(m_uuid) };
    dpp::embed embed{};
    embed.set_title(pce.username + "'s Inventory")
        .set_thumbnail(pce.avatarURL)
        .set_color(0x0088FF)
        .set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/skew-wide.jpg");

    std::string desc{
        '*' + std::to_string(m_uuid) + "*\n\n" 
        + "**Job:** " + ((m_job == adr::Job::MAX) ? "none" : adr::Job::jobs[m_job].name)
        + (getTitle() == adr::daily::t_none ? "" : "\nTitle: " + adr::daily::titleNames[m_title])
        + (m_fameRank == f_none ? "" : "\nFame Rank: " + std::string{ fameRankNames[m_fameRank] })
        + (m_bank.isActive() ? "\nBank: Active" : "")
        + (m_coop.isActive() ? ("\nCo-Op: " + std::string{ (m_coop.m_ownerUUID == m_uuid ? "Owning" : "Participating") }) : "")
        + "\nLast Worked: " + dpp::utility::timestamp(m_lastWorked, dpp::utility::tf_short_datetime)
        + "\nCan Work Next " + nextWorkTimestamp()
        + "\n\n**Inventory:**\n"
    };

    desc += adr::getNonZeroItems(m_inv);

    desc += "\n**Attributes:**\n" + m_atr.list();
    desc += "\n**Statistics:**\n" + m_stat.list();

    embed.set_description(desc);

    return embed;
}

bool adr::Player::exists() const
{
    return std::filesystem::exists("./../playerdata/" + std::to_string(m_uuid) + ".json");
}

bool adr::Player::exists(const dpp::snowflake& uuid)
{
    return std::filesystem::exists("./../playerdata/" + std::to_string(uuid) + ".json");
}

void adr::Player::updateLastWorked() 
{ 
    m_lastWorked = std::time(nullptr); 
    std::cout << m_uuid << " updateLastWorked(), m_lastWorked = " << m_lastWorked 
        << " next work: " << nextWork() << '\n'
        << "current: " << std::time(0) << '\n';
}

std::time_t adr::Player::nextWork() const 
{ 
    // On Debug Mode, if the player has godmode, no cooldown
#ifdef DEBUG 
    if (m_godmode == Player::g_all || m_godmode == Player::g_no_cooldown) 
        return -500;
#endif

    constexpr int ambassadorEffect{ 60 * 60 * 3 }; // 3 hours

    // relative time, not since epoch
    return m_lastWorked - std::time(nullptr) + workCooldownSeconds
      - ((m_fameRank == f_ambassador) * ambassadorEffect); 
} 

std::string adr::Player::nextWorkTimestamp() const
{ 
    std::time_t t{ std::time(nullptr) + nextWork() };
    return dpp::utility::timestamp(t, dpp::utility::tf_relative_time) + " (" + dpp::utility::timestamp(t, dpp::utility::tf_short_time) + ")";
}

void adr::Player::updateLastFought() 
{ 
    m_lastFought = std::time(nullptr); 
    std::cout << m_uuid << " updateLastFought(), m_lastFought = " << m_lastFought 
        << " next fight: " << nextFight() << '\n'
        << "current: " << std::time(0) << '\n';
}

std::time_t adr::Player::nextFight() const 
{ 
    // On Debug Mode, if the player has godmode, no cooldown
#ifdef DEBUG 
    if (m_godmode == Player::g_all || m_godmode == Player::g_no_cooldown) 
        return -500;
#endif

    // relative time, not since epoch
    return m_lastFought - std::time(nullptr) + fightCooldownSeconds; 
} 

std::string adr::Player::nextFightTimestamp() const
{ 
    std::time_t t{ std::time(nullptr) + nextFight() };
    return dpp::utility::timestamp(t, dpp::utility::tf_relative_time) + " (" + dpp::utility::timestamp(t, dpp::utility::tf_short_time) + ")";
}

void adr::Player::changeInv(const inventory& difference)
{
    for (std::size_t i{}; i < m_inv.size(); ++i) {
        m_inv[i] += difference[i];
    }
}

void adr::Player::subtractInv(const inventory& difference)
{
    for (std::size_t i{}; i < m_inv.size(); ++i) {
        m_inv[i] -= difference[i];
    }
}

bool adr::Player::canBuy(const inventory& cost)
{
    for (std::size_t i{}; i < m_inv.size(); ++i) {
        if (m_inv[i] < cost[i]) return false;
    }
    return true;
}

int& adr::Player::operator[](int index)
{
    return m_inv[index];
}

const int& adr::Player::operator[](int index) const
{
    return m_inv[index];
}

void adr::Player::setRole(RoleID role, bool status)
{
  if (status) [[likely]] {
    bot.guild_member_add_role(
        guildID,
        m_uuid,
        roleIDs[role]);
  }
  else {
    bot.guild_member_delete_role(
        guildID, 
        m_uuid, 
        roleIDs[role]);
  }

  std::cout << m_uuid << " role " << static_cast<int>(role) << " = " << status << '\n';
}

void adr::Player::setJob(adr::Job::Id job) {
  m_job = job;
  if (m_job != Job::MAX) // to prevent overflow
    setRole(static_cast<RoleID>(job), true);
}
