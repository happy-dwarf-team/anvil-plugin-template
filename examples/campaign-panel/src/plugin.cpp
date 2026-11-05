// The player's country in a window, with no game addresses: no symbols.toml, so
// nothing to port when the game updates. It reads the campaign through
// anvil-game, hears the month and the wars, and hands rows to the overlay.
#include <anvil/anvil.hpp>
#include <anvil/anvil_game.h>
#include <anvil/anvil_panel.h>

#include <format>
#include <iterator>
#include <string>

namespace {

struct CampaignPanel : anvil::Plugin {
    const AnvilGameWorldService* world = nullptr;
    const AnvilPanelService* panels = nullptr;
    AnvilPanel* panel = nullptr;
    std::string last_war = "none";

    // In a handler of an anvil-game event, or in a click: the only moments the
    // world answers.
    void refresh() {
        AnvilGameTag me{};
        AnvilGameCountry country{};
        country.size = sizeof(country);
        if (world->player(&me) != ANVIL_OK || world->country(me.text, &country) != ANVIL_OK) return;
        uint32_t wars = 0;
        world->war_ids(nullptr, 0, &wars);

        const std::string treasury = std::format("{:.1f}", country.treasury / 1000.0);
        const std::string income = std::format("{:.1f}", country.monthly_income / 1000.0);
        const std::string stability = std::format("{:+}", country.stability / 1000);
        const std::string provinces = std::to_string(country.province_count);
        const std::string running = std::to_string(wars);
        const AnvilPanelRow rows[] = {
            {ANVIL_PANEL_TEXT, 0, "Country", me.text, 0},
            {ANVIL_PANEL_TEXT, 0, "Treasury", treasury.c_str(), 0},
            {ANVIL_PANEL_TEXT, 0, "Income", income.c_str(), 0},
            {ANVIL_PANEL_TEXT, 0, "Stability", stability.c_str(), 0},
            {ANVIL_PANEL_TEXT, 0, "Provinces", provinces.c_str(), 0},
            {ANVIL_PANEL_SEPARATOR, 0, nullptr, nullptr, 0},
            {ANVIL_PANEL_TEXT, 0, "Wars in the world", running.c_str(), 0},
            {ANVIL_PANEL_TEXT, 0, "Latest", last_war.c_str(), 0},
            {ANVIL_PANEL_BUTTON, 0, "Refresh", nullptr, 1},
        };
        panels->set_rows(panel, rows, static_cast<uint32_t>(std::size(rows)), sizeof(AnvilPanelRow));
    }

    void load(anvil::Host& host) override {
        world = host.service<AnvilGameWorldService>(ANVIL_GAME_WORLD_SERVICE, ANVIL_GAME_WORLD_SERVICE_VERSION);
        panels = host.service<AnvilPanelService>(ANVIL_PANEL_SERVICE, ANVIL_PANEL_SERVICE_VERSION);
        if (world == nullptr || panels == nullptr) {
            host.warn("needs anvil-game.world and anvil-overlay.panel: Anvil 1.11 or newer");
            return;
        }
        if (panels->create("My country", &panel) != ANVIL_OK) return;
        // A click runs after an outer frame, so it may read the world.
        panels->on_click(panel, [](void* self, uint64_t) { static_cast<CampaignPanel*>(self)->refresh(); }, this);
        panels->show(panel, 1);

        host.subscribe(ANVIL_GAME_EVENT_SESSION, [this](std::string_view, const void* payload) {
            if (static_cast<const AnvilGameSession*>(payload)->change == ANVIL_GAME_SESSION_STARTED) refresh();
        });
        host.subscribe(ANVIL_GAME_EVENT_MONTH, [this](std::string_view, const void*) { refresh(); });
        host.subscribe(ANVIL_GAME_EVENT_WAR, [this](std::string_view, const void* payload) {
            const auto* war = static_cast<const AnvilGameWarChange*>(payload);
            last_war = std::format("{} {}", war->change == ANVIL_GAME_WAR_STARTED ? "began:" : "ended:", war->name);
            refresh();
        });
    }
};

}  // namespace

ANVIL_PLUGIN(CampaignPanel)
