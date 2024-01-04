#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "IniReader.h"
#include "../Main/Modules/Modules.hpp"

constexpr char ConfigFile[] = "config.ini";

#define WriteSection(key) \
    conf << "[" #key "]" << "\n";
#define WritePair(section, key) \
    conf << #key " = " << Config::section::key << "\n";
#define WriteSectionEnd() conf << "\n";
#define WriteComment(msg) conf << "; " << msg << '\n';

#define ReadPairBool(section, key) \
    Config::section::key = reader.GetBoolean(#section, #key, Config::section::key);
#define ReadPairFloat(section, key) \
    Config::section::key = reader.GetFloat(#section, #key, Config::section::key);
#define ReadPairInt(section, key) \
    Config::section::key = reader.GetInteger(#section, #key, Config::section::key);
#define ReadPairString(section, key) \
    Config::section::key = reader.Get(#section, #key, Config::section::key);

namespace Config {
    namespace Aimbot {
        bool Enabled = true;
        bool PredictMovement = true;
        bool PredictBulletDrop = true;
        bool AllowTargetSwitch = false;
        float Speed = 40;
        float Smooth = 16;
        float ExtraSmooth = 5499;
        float FOV = 6;
        float MinDistance = 1;
        float MaxDistance = 69;
        int HitBox = 2;
        int aimHotKey = 60;
    };

    namespace NoRecoil{
        bool Enabled = true;
        float PitchPower = 30;
        float YawPower = 30;
    };

    namespace Sense {
        bool Enabled = true;
        bool ItemGlow = true;
        bool DrawSeer = true;
        bool DrawTracers = false;
        bool DrawDistance = true;
        bool DrawNames = true;
        bool AimedAtOnly = true;
        float MaxDistance = 200;
        bool ShowSpectators = true;
        bool printPlayersLvl = false;
        bool DrawFOVCircle = false;
        bool DrawRainbowCircle = true;
        float SeerMaxDistance = 200;
        float GameFOV = 120;
        bool SuperGlide = true;
    };

    namespace Radar {
        bool miniRadar = true;
        bool bigRadar = true;
        bool MiniMapGuides = true;
        float miniMapRange = 100;
        int minimapradardotsize1 = 7;
        int minimapradardotsize2 = 1;
        int bigMapHotKey = 23;
    }

    namespace Triggerbot {
        bool Enabled = true;
        float Range = 200;
        float HipFireRange = 30;
    };
    
    namespace GameMode{
        bool threeManSquad = true;
    }
};


void UpdateConfig() {
    std::ofstream conf(ConfigFile);
    if (conf.is_open()) {
        WriteSection(Aimbot);
        WritePair(Aimbot, Enabled);
        WritePair(Aimbot, PredictMovement);
        WritePair(Aimbot, PredictBulletDrop);
        WritePair(Aimbot, AllowTargetSwitch);
        WritePair(Aimbot, HitBox);
        WritePair(Aimbot, aimHotKey);
        WritePair(Aimbot, Speed);
        WritePair(Aimbot, Smooth);
        WritePair(Aimbot, ExtraSmooth);
        WritePair(Aimbot, FOV);
        WritePair(Aimbot, MinDistance);
        WritePair(Aimbot, MaxDistance);
        WriteSectionEnd();

        WriteSection(NoRecoil);
        WritePair(NoRecoil, Enabled);
        WritePair(NoRecoil, PitchPower);
        WritePair(NoRecoil, YawPower);
        WriteSectionEnd();

        WriteSection(Sense);
        WritePair(Sense, Enabled);
        WritePair(Sense, ItemGlow);
        WritePair(Sense, DrawSeer);
        WritePair(Sense, DrawTracers);
        WritePair(Sense, DrawDistance);
        WritePair(Sense, DrawNames);
        WritePair(Sense, AimedAtOnly);
        WritePair(Sense, MaxDistance);
        WritePair(Sense, SeerMaxDistance);
        WritePair(Sense, ShowSpectators);
        WritePair(Sense, printPlayersLvl);
        WritePair(Sense, DrawFOVCircle);
        WritePair(Sense, DrawRainbowCircle);
        WritePair(Sense, GameFOV);
        WritePair(Sense, SuperGlide);
        WriteSectionEnd();

        WriteSection(Radar);
        WritePair(Radar, miniRadar);
        WritePair(Radar, bigRadar);
        WritePair(Radar, MiniMapGuides);
        WritePair(Radar, miniMapRange);
        WritePair(Radar, minimapradardotsize1);
        WritePair(Radar, minimapradardotsize2);
        WritePair(Radar, bigMapHotKey);
        WriteSectionEnd();

        WriteSection(Triggerbot);
        WritePair(Triggerbot, Enabled);
        WritePair(Triggerbot, Range);
        WritePair(Triggerbot, HipFireRange);
        WriteSectionEnd();

        WriteSection(GameMode);
        WritePair(GameMode, threeManSquad);
        WriteSectionEnd();
        conf.close();
    }
}

bool ReadConfig(const std::string &configFile) {
    INIReader reader(configFile);
    if (reader.ParseError() < 0) {
        UpdateConfig();
        return false;
    }
    
    ReadPairBool(Aimbot, Enabled);
    ReadPairBool(Aimbot, PredictMovement);
    ReadPairBool(Aimbot, PredictBulletDrop);
    ReadPairBool(Aimbot, AllowTargetSwitch);
    ReadPairInt(Aimbot, HitBox);
    ReadPairInt(Aimbot, aimHotKey);
    ReadPairFloat(Aimbot, Speed);
    ReadPairFloat(Aimbot, Smooth);
    ReadPairFloat(Aimbot, ExtraSmooth);
    ReadPairFloat(Aimbot, FOV);
    ReadPairFloat(Aimbot, MinDistance);
    ReadPairFloat(Aimbot, MaxDistance);

    ReadPairBool(NoRecoil, Enabled);
    ReadPairFloat(NoRecoil, PitchPower);
    ReadPairFloat(NoRecoil, YawPower);
    
    ReadPairBool(Sense, Enabled);
    ReadPairBool(Sense, ItemGlow);
    ReadPairBool(Sense, DrawSeer);
    ReadPairBool(Sense, DrawTracers);
    ReadPairBool(Sense, DrawDistance);
    ReadPairBool(Sense, DrawNames);
    ReadPairBool(Sense, AimedAtOnly);
    ReadPairFloat(Sense, MaxDistance);
    ReadPairFloat(Sense, SeerMaxDistance);
    ReadPairBool(Sense, ShowSpectators);
    ReadPairBool(Sense, printPlayersLvl);
    ReadPairBool(Sense, DrawFOVCircle);
    ReadPairBool(Sense, DrawRainbowCircle);
    ReadPairFloat(Sense, GameFOV);  
    ReadPairBool(Sense, SuperGlide);
    
    ReadPairBool(Radar, miniRadar);
    ReadPairBool(Radar, bigRadar);
    ReadPairBool(Radar, MiniMapGuides);
    ReadPairFloat(Radar, miniMapRange);
    ReadPairInt(Radar, minimapradardotsize1);
    ReadPairInt(Radar, minimapradardotsize2);
    ReadPairInt(Radar, bigMapHotKey);

    ReadPairBool(Triggerbot, Enabled);
    ReadPairFloat(Triggerbot, Range);
    ReadPairFloat(Triggerbot, HipFireRange);

    ReadPairBool(GameMode, threeManSquad)

    UpdateConfig();
    return true;
}
//---------------------[SAVE VALUES]----------------------------------------------
bool AimbotSave() {
    try { 
        Config::Aimbot::Enabled = Modules::Aimbot::Enabled;
        Config::Aimbot::PredictMovement = Modules::Aimbot::PredictMovement;
        Config::Aimbot::PredictBulletDrop = Modules::Aimbot::PredictBulletDrop;
        Config::Aimbot::AllowTargetSwitch = Modules::Aimbot::AllowTargetSwitch;
        Config::Aimbot::HitBox = static_cast<int>(Modules::Aimbot::Hitbox);
        Config::Aimbot::aimHotKey = static_cast<int>(Modules::Aimbot::aimHotkey);
        Config::Aimbot::Speed = Modules::Aimbot::Speed;
        Config::Aimbot::Smooth = Modules::Aimbot::Smooth;
        Config::Aimbot::FOV = Modules::Aimbot::FOV;
        Config::Aimbot::ExtraSmooth = Modules::Aimbot::ExtraSmooth;
        Config::Aimbot::MinDistance = Modules::Aimbot::MinDistance;
        Config::Aimbot::MaxDistance = Modules::Aimbot::MaxDistance;    
        return true;
    } catch (...) {
        return false;
    }
}
bool NoRecoilSave() {
    try {
        Config::NoRecoil::Enabled = Modules::NoRecoil::Enabled;
        Config::NoRecoil::PitchPower = Modules::NoRecoil::PitchPower;
        Config::NoRecoil::YawPower = Modules::NoRecoil::YawPower;
        return true;
    } catch (...) {
        return false;
    }
}
bool ESPSave() {
    try {
        Config::Sense::Enabled = Modules::ESP::Enabled;
        Config::Sense::ItemGlow = Modules::ESP::ItemGlow;
        Config::Sense::MaxDistance = Modules::ESP::GlowMaxDistance;
        Config::Sense::DrawSeer = Modules::ESP::DrawSeer;
        Config::Sense::DrawTracers = Modules::ESP::DrawTracers;
        Config::Sense::DrawDistance = Modules::ESP::DrawDistance;
        Config::Sense::DrawNames = Modules::ESP::DrawNames;
        Config::Sense::SeerMaxDistance = Modules::ESP::SeerMaxDistance;
        Config::Sense::AimedAtOnly = Modules::ESP::AimedAtOnly;
        Config::Sense::ShowSpectators = Modules::ESP::ShowSpectators;
        Config::Sense::printPlayersLvl = Modules::ESP::printPlayersLvl;
        Config::Sense::DrawFOVCircle = Modules::ESP::DrawFOVCircle;
        Config::Sense::DrawRainbowCircle = Modules::ESP::DrawRainbowCircle;
        Config::Sense::GameFOV = Modules::ESP::GameFOV;
        Config::Sense::SuperGlide = Modules::ESP::SuperGlide;
      
        return true;
    } catch (...) {
        return false;
    }
}
bool RadarSave() {
    try {
        Config::Radar::miniRadar = Modules::Radar::miniRadar;
        Config::Radar::bigRadar = Modules::Radar::bigRadar;
        Config::Radar::MiniMapGuides = Modules::Radar::MiniMapGuides;
        Config::Radar::miniMapRange = Modules::Radar::miniMapRange;
        Config::Radar::minimapradardotsize1 = Modules::Radar::minimapradardotsize1;
        Config::Radar::minimapradardotsize2 = Modules::Radar::minimapradardotsize2;
        Config::Radar::bigMapHotKey = static_cast<int>(Modules::Radar::bigMapHotKey);
        return true;
    } catch (...) {
        return false;
    }
}
bool TriggerSave() {
    try {
        Config::Triggerbot::Enabled = Modules::TriggerBot::Enabled;
        Config::Triggerbot::Range = Modules::TriggerBot::TriggerbotRange;
        Config::Triggerbot::HipFireRange = Modules::TriggerBot::HipFireRange;
        return true;
    } catch (...) {
        return false;
    }
}
bool GameModeSave() {
    try {
        Config::GameMode::threeManSquad = Modules::GameMode::threeManSquad;
        return true;
    } catch (...) {
        return false;
    }
}
// Config
void LoadConfig() {
    bool success = ReadConfig("config.ini");
    if (success)
        std::cout << "successfully read config" << std::endl;
    else
        std::cout << "No config file found - Creating new one.." << std::endl;

    // Aimbot //
    Modules::Aimbot::Enabled = Config::Aimbot::Enabled;
    Modules::Aimbot::PredictMovement = Config::Aimbot::PredictMovement;
    Modules::Aimbot::PredictBulletDrop = Config::Aimbot::PredictBulletDrop;
    Modules::Aimbot::AllowTargetSwitch = Config::Aimbot::AllowTargetSwitch;
    Modules::Aimbot::Hitbox = static_cast<HitboxType>(Config::Aimbot::HitBox);
    Modules::Aimbot::aimHotkey = static_cast<InputKeyType>(Config::Aimbot::aimHotKey);
    Modules::Aimbot::Speed = Config::Aimbot::Speed;
    Modules::Aimbot::Smooth = Config::Aimbot::Smooth;
    Modules::Aimbot::FOV = Config::Aimbot::FOV;
    Modules::Aimbot::ExtraSmooth = Config::Aimbot::ExtraSmooth;
    Modules::Aimbot::MinDistance = Config::Aimbot::MinDistance;
    Modules::Aimbot::MaxDistance = Config::Aimbot::MaxDistance;
 
    // No Recoil //
    Modules::NoRecoil::Enabled = Config::NoRecoil::Enabled;
    Modules::NoRecoil::PitchPower = Config::NoRecoil::PitchPower;
    Modules::NoRecoil::YawPower = Config::NoRecoil::YawPower;
    
    // ESP //
    Modules::ESP::Enabled = Config::Sense::Enabled;
    Modules::ESP::ItemGlow = Config::Sense::ItemGlow;
    Modules::ESP::GlowMaxDistance = Config::Sense::MaxDistance;
    Modules::ESP::DrawSeer = Config::Sense::DrawSeer;
    Modules::ESP::DrawDistance = Config::Sense::DrawDistance;
    Modules::ESP::DrawNames = Config::Sense::DrawNames;
    Modules::ESP::DrawTracers = Config::Sense::DrawTracers;
    Modules::ESP::SeerMaxDistance = Config::Sense::SeerMaxDistance;
    Modules::ESP::AimedAtOnly = Config::Sense::AimedAtOnly;
    Modules::ESP::ShowSpectators = Config::Sense::ShowSpectators;
    Modules::ESP::printPlayersLvl = Config::Sense::printPlayersLvl;
    Modules::ESP::DrawFOVCircle = Config::Sense::DrawFOVCircle;
    Modules::ESP::DrawRainbowCircle = Config::Sense::DrawRainbowCircle;
    Modules::ESP::GameFOV = Config::Sense::GameFOV;
    Modules::ESP::SuperGlide = Config::Sense::SuperGlide;

    // Radar //
    Modules::Radar::miniRadar = Config::Radar::miniRadar;
    Modules::Radar::bigRadar = Config::Radar::bigRadar;
    Modules::Radar::MiniMapGuides = Config::Radar::MiniMapGuides;
    Modules::Radar::miniMapRange = Config::Radar::miniMapRange;
    Modules::Radar::minimapradardotsize1 = Config::Radar::minimapradardotsize1;
    Modules::Radar::minimapradardotsize2 = Config::Radar::minimapradardotsize2;
    Modules::Radar::bigMapHotKey = static_cast<InputKeyType>(Config::Radar::bigMapHotKey);
   
    // Triggerbot //
    Modules::TriggerBot::Enabled = Config::Triggerbot::Enabled;
    Modules::TriggerBot::TriggerbotRange = Config::Triggerbot::Range;
    Modules::TriggerBot::HipFireRange = Config::Triggerbot::HipFireRange;
    
    // GameMode //
    Modules::GameMode::threeManSquad = Config::GameMode::threeManSquad;
}