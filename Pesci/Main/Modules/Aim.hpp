#pragma once
#include <iostream>
#include <vector>
#include "../Cuore/Entities/LocalPlayer.hpp"
#include "../Cuore/Entities/Players.hpp"
#include "../../Utilities/InputManager.hpp"
#include "../Cuore/Util/MyDisplay.hpp"
#include "../Cuore/Util/Structs.hpp"
#include "../Cuore/Util/Offsets.hpp"
#include "../Cuore/Util/QAngle.hpp"
#include "../Cuore/Util/Resolver.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

// Conversion
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )

struct Aimbot {
    MyDisplay* X11Display;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;
    Level* m_level;
    Player* CurrentTarget = nullptr;
    bool TargetSelected = true;
    QAngle RCSLastPunch;
    std::chrono::milliseconds LastAimTime;

    Aimbot(MyDisplay* X11Display, LocalPlayer* Myself, std::vector<Player*>* GamePlayers, Level* level) {
        this->X11Display = X11Display;
        this->Myself = Myself;
        this->Players = GamePlayers;
        this->m_level = level;
    }

    void Update() {
        if (!Modules::Aimbot::Enabled) { ReleaseTarget(); return; }
        if (!Myself->IsCombatReady()) { TargetSelected = false; return; }
        if (!InputManager::isKeyDown(Modules::Aimbot::aimHotkey)) { ReleaseTarget(); TargetSelected = false; CurrentTarget = nullptr; return;}
        if (Myself->IsHoldingGrenade) { ReleaseTarget(); return; }

        Player* Target = CurrentTarget;
        if (!IsValidTarget(Target)) {
            if(TargetSelected && !Modules::Aimbot::AllowTargetSwitch)
                return;

            Target = FindBestTarget();
            if (!IsValidTarget(Target)) {
                CurrentTarget = nullptr;
                return;
            }
            
            CurrentTarget = Target;
            CurrentTarget->IsLockedOn = true;
            TargetSelected = true;
        }
        StartAiming();
    }

    void StartAiming() {
        // Get Target Angle
        QAngle DesiredAngles = QAngle(0, 0);
        if (!GetAngle(CurrentTarget, DesiredAngles))
            return;

        // Calculate Increment
        Vector2D DesiredAnglesIncrement = Vector2D(CalculatePitchIncrement(DesiredAngles), CalculateYawIncrement(DesiredAngles));

        // Calculate Smooth
        float Extra = Modules::Aimbot::ExtraSmooth / CurrentTarget->DistanceToLocalPlayer;
        float TotalSmooth = Modules::Aimbot::Smooth + Extra;

        // No recoil calcs
        Vector2D punchAnglesDiff = Myself->PunchAnglesDifferent.Divide(Modules::Aimbot::Smooth).Multiply(Modules::Aimbot::Speed);
        double nrPitchIncrement = punchAnglesDiff.x;
        double nrYawIncrement = -punchAnglesDiff.y;

        // Aimbot calcs
        Vector2D aimbotDelta = DesiredAnglesIncrement.Divide(TotalSmooth).Multiply(Modules::Aimbot::Speed);
        double aimYawIncrement = aimbotDelta.y * -1;
        double aimPitchIncrement = aimbotDelta.x;

        // Combine
        double totalPitchIncrement = aimPitchIncrement + nrPitchIncrement;
        double totalYawIncrement = aimYawIncrement + nrYawIncrement;

        // Turn into integers
        int totalPitchIncrementInt = RoundHalfEven(AL1AF0(totalPitchIncrement));
        int totalYawIncrementInt = RoundHalfEven(AL1AF0(totalYawIncrement));

        // Move Mouse
        if (totalPitchIncrementInt == 0 && totalYawIncrementInt == 0) return;
        X11Display->MoveMouse(totalPitchIncrementInt, totalYawIncrementInt);
    }

    bool GetAngle(const Player* Target, QAngle& Angle) {
        const QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).NormalizeAngles();
        if (!CurrentAngle.isValid())
            return false;

        if (!GetAngleToTarget(Target, Angle))
            return false;

        return true;
    }

    bool GetAngleToTarget(const Player* Target, QAngle& Angle) const {
        const Vector3D TargetPosition = Target->GetBonePosition(Modules::Aimbot::Hitbox);
        const Vector3D TargetVelocity = Target->AbsoluteVelocity;
        const Vector3D CameraPosition = Myself->CameraPosition;
        const QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).NormalizeAngles();
        
        if (Myself->WeaponProjectileSpeed > 1.0f) {
            if(Modules::Aimbot::PredictBulletDrop && Modules::Aimbot::PredictMovement) {
                return Resolver::CalculateAimRotationNew(CameraPosition, TargetPosition, TargetVelocity, Myself->WeaponProjectileSpeed, Myself->WeaponProjectileScale, 255, Angle);
            }
            else if (Modules::Aimbot::PredictBulletDrop) {
                return Resolver::CalculateAimRotationNew(CameraPosition, TargetPosition, Vector3D(0, 0, 0), Myself->WeaponProjectileSpeed, Myself->WeaponProjectileScale, 255, Angle);
            }
            else if (Modules::Aimbot::PredictMovement) {
                return Resolver::CalculateAimRotation(CameraPosition, TargetPosition, TargetVelocity, Myself->WeaponProjectileSpeed, Angle);
            }
        }
        Angle = Resolver::CalculateAngle(CameraPosition, TargetPosition);
        return true;   
    }

    bool IsValidTarget(Player* target) {
        if (target == nullptr ||
            !target->IsValid() || 
            !target->IsCombatReady() || 
            !target->IsVisible || 
            !target->IsHostile || 
            target->Distance2DToLocalPlayer < Conversion::ToGameUnits(Modules::Aimbot::MinDistance) || 
            target->Distance2DToLocalPlayer > Conversion::ToGameUnits(Modules::Aimbot::MaxDistance))
            return false;
        return true;
    }

    float CalculatePitchIncrement(QAngle AimbotDesiredAngles) {
        float wayA = AimbotDesiredAngles.x - Myself->ViewAngles.x;
        float wayB = 180 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }

    float CalculateYawIncrement(QAngle AimbotDesiredAngles) {
        float wayA = AimbotDesiredAngles.y - Myself->ViewAngles.y;
        float wayB = 360 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (fabs(wayA) < fabs(wayB))
            return wayA;
        return wayB;
    }

    Player* FindBestTarget() {
        float NearestDistance = 9999;
        Player* BestTarget = nullptr;
        Vector3D CameraPosition = Myself->CameraPosition;
        QAngle CurrentAngle = QAngle(Myself->ViewAngles.x, Myself->ViewAngles.y).NormalizeAngles();
        for (int i = 0; i < Players->size(); i++) {
            Player* p = Players->at(i);
            if (!IsValidTarget(p)) continue;

            Vector3D TargetPos = p->LocalOrigin;
            if (CameraPosition.Distance(TargetPos) <= 0.0001f)
                continue;

            QAngle TargetAngle = Resolver::CalculateAngle(CameraPosition, TargetPos);
            if (!TargetAngle.isValid())
                continue;

            double DistanceFromCrosshair = CurrentAngle.distanceTo(TargetAngle);
            if (DistanceFromCrosshair > Modules::Aimbot::FOV)
                continue;

            if (DistanceFromCrosshair < NearestDistance) {
                BestTarget = p;
                NearestDistance = DistanceFromCrosshair;
            }
        }
        return BestTarget;
    }

    void ReleaseTarget() {
        if (CurrentTarget != nullptr && CurrentTarget->IsValid())
            CurrentTarget->IsLockedOn = false;
        
        TargetSelected = false;
        CurrentTarget = nullptr;
    }

    int RoundHalfEven(float x) {
        return (x >= 0.0)
            ? static_cast<int>(std::round(x))
            : static_cast<int>(std::round(-x)) * -1;
    }

    float AL1AF0(float num) {
        if (num > 0) return std::max(num, 1.0f);
        return std::min(num, -1.0f);
    }
};