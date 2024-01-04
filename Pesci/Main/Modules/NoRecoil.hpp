#pragma once
#include <iostream>
#include <vector>

#include "../../Main/Modules/Modules.hpp"
#include "../../Cuore/Entities/LocalPlayer.hpp"
#include "../../Cuore/Util/MyDisplay.hpp"
#include "../../Cuore/Util/Structs.hpp"

struct NoRecoil {
    MyDisplay* display;
    LocalPlayer* localPlayer;
    Vector2D previous_weaponPunchAngles;

    NoRecoil(MyDisplay* display, LocalPlayer* localPlayer) {
        this->display = display;
        this->localPlayer = localPlayer;
    }

    void controlWeapon() {
        if (!Modules::NoRecoil::Enabled)return;
        if (!localPlayer->IsCombatReady()) return;
        if (!localPlayer->IsInAttack) return;
        Vector2D punchAnglesDiff = localPlayer->PunchAnglesDifferent;
        if (punchAnglesDiff.IsZeroVector()) return;
        int pitch = (punchAnglesDiff.x > 0)
            ? roundHalfEven(punchAnglesDiff.x * Modules::NoRecoil::PitchPower)
            : 0;
        int yaw = roundHalfEven(-punchAnglesDiff.y * Modules::NoRecoil::YawPower);
        display->MoveMouse(pitch, yaw);
    }

    int roundHalfEven(float x) {
        return (x >= 0.0)
            ? static_cast<int>(std::round(x))
            : static_cast<int>(std::round(-x)) * -1;
    }
};