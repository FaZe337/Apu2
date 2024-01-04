#pragma once

#include "../../Memory/Mem.hpp"
#include "../Util/Structs.hpp"
#include "../Util/Offsets.hpp"

struct LocalPlayer {
    long BasePointer;

    bool IsDead;
    bool IsInAttack;
    bool IsKnocked;
    bool IsZooming;

    int Team;
    Vector3D LocalOrigin;
    Vector3D CameraPosition;

    Vector2D ViewAngles;
    Vector2D PunchAngles;
    Vector2D PunchAnglesPrevious;
    Vector2D PunchAnglesDifferent;


    int WeaponIndex;
    float WeaponProjectileSpeed;
    float WeaponProjectileScale;
    bool IsHoldingGrenade;
    long highlightSettingsPtr;
    float ZoomFOV;
    float TargetZoomFOV;

    float ViewYaw;

    void ResetPointer() {
        BasePointer = 0;
    }
    
    void Read() {
        BasePointer = mem::Read<long>(OFF_REGION + OFF_LOCAL_PLAYER);
        if (BasePointer == 0) return;

        IsDead = mem::Read<short>(BasePointer + OFF_LIFE_STATE) > 0;
        IsKnocked = mem::Read<short>(BasePointer + OFF_BLEEDOUT_STATE) > 0;
        IsZooming = mem::Read<short>(BasePointer + OFF_ZOOMING) > 0;
        IsInAttack = mem::Read<short>(OFF_REGION + OFF_INATTACK) > 0;
        highlightSettingsPtr = mem::Read<long>( OFF_REGION + OFF_GLOW_HIGHLIGHTS);
        Team = mem::Read<int>(BasePointer + OFF_TEAM_NUMBER);
        LocalOrigin = mem::Read<Vector3D>(BasePointer + OFF_LOCAL_ORIGIN);
        CameraPosition = mem::Read<Vector3D>(BasePointer + OFF_CAMERAORIGIN);
        ViewAngles = mem::Read<Vector2D>(BasePointer + OFF_VIEW_ANGLES);
        PunchAngles = mem::Read<Vector2D>(BasePointer + OFF_PUNCH_ANGLES);
        PunchAnglesDifferent = PunchAnglesPrevious.Subtract(PunchAngles);
        PunchAnglesPrevious = PunchAngles;

        ViewYaw = mem::Read<float>(BasePointer + OFF_YAW);

        if (!IsDead && !IsKnocked) {
            long WeaponHandle = mem::Read<long>(BasePointer + OFF_WEAPON_HANDLE);
            long WeaponHandleMasked = WeaponHandle & 0xffff;
            long WeaponEntity = mem::Read<long>(OFF_REGION + OFF_ENTITY_LIST + (WeaponHandleMasked << 5));

            int OffHandWeaponID = mem::Read<int>(BasePointer + OFF_OFFHAND_WEAPON);
            IsHoldingGrenade = OffHandWeaponID == -251 ? true : false;
            
            ZoomFOV = mem::Read<float>(WeaponEntity + OFF_CURRENTZOOMFOV);
            TargetZoomFOV = mem::Read<float>(WeaponEntity + OFF_TARGETZOOMFOV);
            
            WeaponIndex = mem::Read<int>(WeaponEntity + OFF_WEAPON_INDEX);
            WeaponProjectileSpeed = mem::Read<float>(WeaponEntity + OFF_PROJECTILESPEED);
            WeaponProjectileScale = mem::Read<float>(WeaponEntity + OFF_PROJECTILESCALE);
        }
    }

    bool IsValid() {
        return BasePointer != 0;
    }

    bool IsCombatReady() {
        if (BasePointer == 0) return false;
        if (IsDead) return false;
        if (IsKnocked) return false;
        return true;
    }

    void setMeYaw(float angle)
    {
        long ptrLong = BasePointer + OFF_VIEW_ANGLES + sizeof(float);
        mem::Write<float>(ptrLong, angle);
    }

    float getPunchPitch()
    {
        long ptrLong = BasePointer + OFF_PUNCH_ANGLES;
        float result = mem::Read<float>(ptrLong);
        return result;
    }

    float getPitch()
    {
        long ptrLong = BasePointer + OFF_VIEW_ANGLES;
        float result = mem::Read<float>(ptrLong);
        return result;
    }
    void setPitch(float angle)
    {
        if (angle > 90 || angle < -90)
            return;
        long ptrLong = BasePointer + OFF_VIEW_ANGLES;
        mem::Write<float>(ptrLong, angle);
    }
    float getYaw()
    {
        long ptrLong = BasePointer + OFF_VIEW_ANGLES + sizeof(float);
        float result = mem::Read<float>(ptrLong);
        return result;
    }
    void setYaw(float angle)
    {
        if (angle > 180 || angle < -180)
            return;
        long ptrLong = BasePointer + OFF_VIEW_ANGLES + sizeof(float);
        mem::Write<float>(ptrLong, angle);
    }
    float getPunchYaw()
    {
        long ptrLong = BasePointer + OFF_PUNCH_ANGLES + sizeof(float);
        float result = mem::Read<float>(ptrLong);
        return result;
    }
};