#pragma once
#include <iostream>
#include <vector>
#include <set>

#include "../Cuore/Entities/LocalPlayer.hpp"
#include "../Cuore/Entities/Players.hpp"

#include "../Cuore/Util/MyDisplay.hpp"
#include "../Cuore/Util/Structs.hpp"
#include "../Cuore/Util/Offsets.hpp"
#include "../Cuore/Util/QAngle.hpp"
#include "../Cuore/Util/Resolver.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


struct Triggerbot {
    MyDisplay* X11Display;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;

    const float TB_MAX_RANGE_ZOOMED = Conversion::ToGameUnits(Modules::TriggerBot::TriggerbotRange);
    const float TB_MAX_RANGE_HIPFRE = Conversion::ToGameUnits(Modules::TriggerBot::HipFireRange);

    Triggerbot(MyDisplay* X11Display, LocalPlayer* Myself, std::vector<Player*>* GamePlayers) {
        this->X11Display = X11Display;
        this->Myself = Myself;
        this->Players = GamePlayers;
    }

     void Update() {
        if (!Modules::TriggerBot::Enabled) return;
        if (!Myself->IsCombatReady()) return;

        if (Modules::TriggerBot::WeaponList.find(Myself->WeaponIndex) == Modules::TriggerBot::WeaponList.end()) return;

        const float RANGE_MAX = (Myself->IsZooming) ? TB_MAX_RANGE_ZOOMED : TB_MAX_RANGE_HIPFRE;

        for (int i = 0; i < Players->size(); i++) {
            Player* player = Players->at(i);
            if (!player->IsCombatReady()) continue;
            if (!player->IsHostile) continue;
            if (!player->IsAimedAt) continue;
            if (player->DistanceToLocalPlayer <= RANGE_MAX) {
                X11Display->MouseClickLeft();
                break;
            }
        }
    }
};