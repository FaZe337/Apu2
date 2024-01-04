#pragma once
#include <string>

#include "../Entities/LocalPlayer.hpp"
#include "../../Memory/Mem.hpp"
#include "../Util/Structs.hpp"
#include "../Util/Offsets.hpp"
#include "../Utilities/Config.hpp"

struct Player {
    LocalPlayer* Myself;

    int Index;
    long BasePointer;

    std::string Name;
    int Team;

    int GlowEnable;
    int GlowThroughWall;
    int HighlightID;

    bool IsDead;
    bool IsKnocked;
    Vector2D ViewAngles;
    Vector3D LocalOrigin;
    Vector3D AbsoluteVelocity;

    int Health;
    int MaxHealth;
    int Shield;
    int MaxShield;

    int LastTimeAimedAt;
    int LastTimeAimedAtPrevious;
    bool IsAimedAt;
    bool nonBR;
    bool friendly;
    int LastVisibleTime;
    int LastTimeVisiblePrevious;
    bool IsVisible;
   
    bool IsLocal;
    bool IsAlly;
    bool IsHostile;

    float DistanceToLocalPlayer;
    float Distance2DToLocalPlayer;

    float ViewYaw;

    bool IsLockedOn;

    Player(int PlayerIndex, LocalPlayer* Me) {
        this->Index = PlayerIndex;
        this->Myself = Me;
    }

    void Read() {
        BasePointer = mem::Read<long>(OFF_REGION + OFF_ENTITY_LIST + ((Index + 1) << 5));
        if (BasePointer == 0) return;

        Name = mem::ReadString(BasePointer + OFF_NAME);
        Team = mem::Read<int>(BasePointer + OFF_TEAM_NUMBER);

        if (!IsPlayer() && !IsDummy()) { BasePointer = 0; return; }
        IsDead = (IsDummy()) ? false : mem::Read<short>(BasePointer + OFF_LIFE_STATE) > 0;
        IsKnocked = (IsDummy()) ? false : mem::Read<short>(BasePointer + OFF_BLEEDOUT_STATE) > 0;
        
        LocalOrigin = mem::Read<Vector3D>(BasePointer + OFF_LOCAL_ORIGIN);
        AbsoluteVelocity = mem::Read<Vector3D>(BasePointer + OFF_ABSVELOCITY);

        GlowEnable = mem::Read<int>(BasePointer + OFF_GLOW_ENABLE);
        GlowThroughWall = mem::Read<int>(BasePointer + OFF_GLOW_THROUGH_WALL);
        HighlightID = mem::Read<int>(BasePointer + OFF_GLOW_HIGHLIGHT_ID + 1);
        ViewAngles = mem::Read<Vector2D>(BasePointer + OFF_VIEW_ANGLES);
        ViewYaw = mem::Read<float>(BasePointer + OFF_YAW);
        LastTimeAimedAt = mem::Read<int>(BasePointer + OFF_LAST_AIMEDAT_TIME);
        IsAimedAt = LastTimeAimedAtPrevious < LastTimeAimedAt;
        LastTimeAimedAtPrevious = LastTimeAimedAt;

        LastVisibleTime = mem::Read<int>(BasePointer + OFF_LAST_VISIBLE_TIME);
        IsVisible = IsDummy() || IsAimedAt || LastTimeVisiblePrevious < LastVisibleTime;
        LastTimeVisiblePrevious = LastVisibleTime;

        Health = mem::Read<int>(BasePointer + OFF_HEALTH);
        MaxHealth = mem::Read<int>(BasePointer + OFF_MAXHEALTH);
        Shield = mem::Read<int>(BasePointer + OFF_SHIELD);
        MaxShield = mem::Read<int>(BasePointer + OFF_MAXSHIELD);

        if (Myself->IsValid()) {
            IsLocal = Myself->BasePointer == BasePointer;
            nonBR = !Modules::GameMode::threeManSquad;
            friendly = (nonBR)
                ? (Myself->Team % 2 == 0 && Team % 2 == 0) || (Myself->Team % 2 != 0 && Team % 2 != 0)
                : Myself->Team == Team;
            
            IsAlly = friendly;
            IsHostile = !IsAlly;
            DistanceToLocalPlayer = Myself->LocalOrigin.Distance(LocalOrigin);
            Distance2DToLocalPlayer = Myself->LocalOrigin.To2D().Distance(LocalOrigin.To2D());
        }
    }

    void setCustomGlow(int health, bool isVisible, bool isSameTeam)
    {
        static const int contextId = 1;
        long basePointer = BasePointer;
        int settingIndex = 65;
        std::array<unsigned char, 4> highlightFunctionBits = {
            0,   // InsideFunction
            125, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
            64,  // OutlineRadius: size * 255 / 8
            64   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
        };
        std::array<float, 4> glowColorRGB = { 0, 0, 0, 0 };
        if (isSameTeam) {
            settingIndex = 20;
        } else if (!isVisible) {
            settingIndex = 65;
            glowColorRGB = { 0.5, 0.5, 0.5, 1 }; // knocked enemies // gray
        } else if (health >= 205) {
            settingIndex = 66;
            glowColorRGB = { 1, 0, 0, 1 }; // red shield
        } else if (health >= 190) {
            settingIndex = 67;
            glowColorRGB = { 0.5, 0, 0.5, 1 }; // purple shield
        } else if (health >= 170) {
            settingIndex = 68;
            glowColorRGB = { 0, 0.5, 1, 1 }; // blue shield
        } else if (health >= 95) {
            settingIndex = 69;
            glowColorRGB = { 0, 1, 0.5, 1 }; // gray shield // cyan 
        } else {
            settingIndex = 70;
            glowColorRGB = { 0, 0.5, 0, 1 }; // low health enemies // green
        }
        mem::Write<unsigned char>(basePointer + OFF_GLOW_HIGHLIGHT_ID + contextId, settingIndex);
        if (!isSameTeam) {
            mem::Write<typeof(highlightFunctionBits)>(
                Myself->highlightSettingsPtr + HIGHLIGHT_TYPE_SIZE * settingIndex + 4, highlightFunctionBits);
            mem::Write<typeof(glowColorRGB)>(
                Myself->highlightSettingsPtr + HIGHLIGHT_TYPE_SIZE * settingIndex + 8, glowColorRGB);
            mem::Write<int>(basePointer + OFF_GLOW_FIX, 1);
        }   
    }
    std::string GetPlayerName(){
        uintptr_t NameIndex = mem::Read<uintptr_t>(BasePointer + OFF_NAME_INDEX);
        uintptr_t NameOffset = mem::Read<uintptr_t>(OFF_REGION + OFF_NAME_LIST + ((NameIndex - 1) << 4 ));
        std::string PlayerName = mem::ReadPlayerName(NameOffset, 64);
        return PlayerName;
    }
    std::string getPlayerModelName(){
        uintptr_t modelOffset = mem::Read<uintptr_t>(BasePointer + OFF_MODELNAME);
        std::string modelName = mem::ReadString(modelOffset, 1024);
        // Check for different player names
        if (modelName.find("dummie") != std::string::npos) modelName = "DUMMIE";
        else if (modelName.find("ash") != std::string::npos) modelName = "ASH";
        else if (modelName.find("ballistic") != std::string::npos) modelName = "BALLISTIC";
        else if (modelName.find("bangalore") != std::string::npos) modelName = "BANGALORE";
        else if (modelName.find("bloodhound") != std::string::npos) modelName = "BLOODHOUND";
        else if (modelName.find("catalyst") != std::string::npos) modelName = "CATALYST";
        else if (modelName.find("caustic") != std::string::npos) modelName = "CAUSTIC";
        else if (modelName.find("conduit") != std::string::npos) modelName = "CONDUIT";
        else if (modelName.find("crypto") != std::string::npos) modelName = "CRYPTO";
        else if (modelName.find("fuse") != std::string::npos) modelName = "FUSE";
        else if (modelName.find("gibraltar") != std::string::npos) modelName = "GIBRALTAR";
        else if (modelName.find("horizon") != std::string::npos) modelName = "HORIZON";
        else if (modelName.find("nova") != std::string::npos) modelName = "HORIZON";
        else if (modelName.find("holo") != std::string::npos) modelName = "MIRAGE";
        else if (modelName.find("mirage") != std::string::npos) modelName = "MIRAGE";
        else if (modelName.find("lifeline") != std::string::npos) modelName = "LIFELINE";
        else if (modelName.find("loba") != std::string::npos) modelName = "LOBA";
        else if (modelName.find("madmaggie") != std::string::npos) modelName = "MADMAGGIE";
        else if (modelName.find("newcastle") != std::string::npos) modelName = "NEWCASTLE";
        else if (modelName.find("octane") != std::string::npos) modelName = "OCTANE";
        else if (modelName.find("pathfinder") != std::string::npos) modelName = "PATHFINDER";
        else if (modelName.find("rampart") != std::string::npos) modelName = "RAMPART";
        else if (modelName.find("revenant") != std::string::npos) modelName = "REVENANT";
        else if (modelName.find("seer") != std::string::npos) modelName = "SEER";
        else if (modelName.find("stim") != std::string::npos) modelName = "OCTANE";
        else if (modelName.find("valkyrie") != std::string::npos) modelName = "VALKYRIE";
        else if (modelName.find("vantage") != std::string::npos) modelName = "VANTAGE";
        else if (modelName.find("wattson") != std::string::npos) modelName = "WATTSON";
        else if (modelName.find("wraith") != std::string::npos) modelName = "WRAITH";
        
        return modelName;
    }
    void setGlowEnable(int glowEnable)
    {
        long ptrLong = BasePointer + OFF_GLOW_ENABLE;
        mem::Write<int>(ptrLong, glowEnable);
    }
    void setGlowThroughWall(int glowThroughWall)
    {  
        long ptrLong = BasePointer + OFF_GLOW_THROUGH_WALL;
        mem::Write<int>(ptrLong, glowThroughWall);
    }
    int getGlowThroughWall()
    {
        int ptrInt = mem::Read<int>(BasePointer + OFF_GLOW_THROUGH_WALL);
        if (!mem::IsValidPointer(ptrInt))
            return -1;
        return ptrInt;
    }
    
    int getGlowEnable()
    {
        int ptrInt = mem::Read<int>(BasePointer + OFF_GLOW_ENABLE);
        if (!mem::IsValidPointer(ptrInt))
            return -1;
        return ptrInt;
    }
    int GetPlayerLevel()
    {
        int m_xp = mem::Read<int>(BasePointer + 0x369c); //m_xp
        if (m_xp < 0) return 0;
        if (m_xp < 100) return 1;
     
        int levels[] = { 2750, 6650, 11400, 17000, 23350, 30450, 38300, 46450, 55050,
        64100, 73600, 83550, 93950, 104800, 116100, 127850, 140050, 152400, 164900, 
        177550, 190350, 203300, 216400, 229650, 243050, 256600, 270300, 284150, 298150, 
        312300, 326600, 341050, 355650, 370400, 385300, 400350, 415550, 430900, 446400, 
        462050, 477850, 493800, 509900, 526150, 542550, 559100, 575800, 592650, 609650, 626800, 
        644100, 661550, 679150, 696900, 714800 };
     
        int level = 56;
        int arraySize = sizeof(levels) / sizeof(levels[0]);
     
        for (int i = 0; i < arraySize; i++)
        {
            if (m_xp < levels[i])
            {
                return i + 1;
            }
        }   
        return level + ((m_xp - levels[arraySize - 1] + 1) / 18000);
    }

    bool IsValid() {
        return BasePointer != 0 && Health > 0 && (IsPlayer() || IsDummy());
    }

    bool IsCombatReady() {
        if (!IsValid())return false;
        if (IsDummy()) return true;
        if (IsDead) return false;
        if (IsKnocked) return false;
        return true;
    }

    bool IsPlayer() {
        return Name == "player";
    }

    bool IsDummy() {
        return Team == 97;
    }

    int GetBoneFromHitbox(HitboxType HitBox) const {
        long ModelPointer = mem::Read<long>(BasePointer + OFF_STUDIOHDR);
        if (!mem::IsValidPointer(ModelPointer))
            return -1;

        long StudioHDR = mem::Read<long>(ModelPointer + 0x8);
        if (!mem::IsValidPointer(StudioHDR + 0x34))
            return -1;

        auto HitboxCache = mem::Read<uint16_t>(StudioHDR + 0x34);
        auto HitboxArray = StudioHDR + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1))); 
        if (!mem::IsValidPointer(HitboxArray + 0x4))
            return -1;

        auto IndexCache = mem::Read<uint16_t>(HitboxArray + 0x4);
        auto HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));
        auto BonePointer = HitboxIndex + HitboxArray + (static_cast<int>(HitBox) * 0x20);
        if (!mem::IsValidPointer(BonePointer))
            return -1;
        
        return mem::Read<uint16_t>(BonePointer);
    }

    Vector3D GetBonePosition(HitboxType HitBox) const {
        Vector3D Offset = Vector3D(0.0f, 0.0f, 0.0f);

        int Bone = GetBoneFromHitbox(HitBox);
        if (Bone < 0 || Bone > 255)
            return LocalOrigin.Add(Offset);

        long BonePtr = mem::Read<long>(BasePointer + OFF_BONES);
        BonePtr += (Bone * sizeof(Matrix3x4));
        if (!mem::IsValidPointer(BonePtr))
            return LocalOrigin.Add(Offset);

        Matrix3x4 BoneMatrix = mem::Read<Matrix3x4>(BonePtr);
        Vector3D BonePosition = BoneMatrix.GetPosition();

        if (!BonePosition.IsValid())
            return LocalOrigin.Add(Vector3D(0, 0, 0));

        BonePosition += LocalOrigin;
        return BonePosition;
    }
};