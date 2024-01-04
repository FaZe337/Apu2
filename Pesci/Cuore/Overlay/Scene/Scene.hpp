#pragma once
#include <iostream>
#include <vector>

#include "../../Entities/LocalPlayer.hpp"
#include "../../Entities/Players.hpp"
#include "../../Util/MyDisplay.hpp"
#include "../../Util/Structs.hpp"
#include "../../Util/Offsets.hpp"
#include "../../Util/QAngle.hpp"
#include "../../Util/Resolver.hpp"
#include "../../../imgui/imgui.h"
#include "../../../imgui/imgui_impl_glfw.h"
#include "../../../imgui/imgui_impl_opengl3.h"
#include "../../../Utilities/Config.hpp"


void SceneRenderUI() {
    if (ImGui::BeginTabItem("CONFIG", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
        ImGui::Separator();
        //-------------------------------[GAMEMODE]-------------------------------
        if (ImGui::CollapsingHeader("Game Mode - Settings", nullptr)) {
            ImGui::Checkbox("TRIOS, GUN RUN or Training Area", &Modules::GameMode::threeManSquad);
        }
        //-------------------------------[AIMBOT]------------------------------------------
        if (ImGui::CollapsingHeader("Aimbot - Settings", nullptr)) {
            const char* HitboxTypes[] = {"Head", "Neck", "Upper Chest", "Lower Chest", "Stomach", "Hip"};
            int HitboxTypeIndex = static_cast<int>(Modules::Aimbot::Hitbox);
            ImGui::Combo("Hitbox Type", &HitboxTypeIndex, HitboxTypes, IM_ARRAYSIZE(HitboxTypes));
            Modules::Aimbot::Hitbox = static_cast<HitboxType>(HitboxTypeIndex);
            
            int aimbotHotkey = static_cast<int>(Modules::Aimbot::aimHotkey);
            ImGui::Combo("Aim HotKey##Aimbot", &aimbotHotkey, InputKeyTypeNames, IM_ARRAYSIZE(InputKeyTypeNames));
            Modules::Aimbot::aimHotkey = static_cast<InputKeyType>(aimbotHotkey);
            
            ImGui::Checkbox("Enabled", &Modules::Aimbot::Enabled);
            ImGui::SameLine();
            ImGui::Checkbox("Predict Movement", &Modules::Aimbot::PredictMovement);
            ImGui::SameLine();
            ImGui::Checkbox("Predict Bullet Drop", &Modules::Aimbot::PredictBulletDrop);
            ImGui::SameLine();
            ImGui::Checkbox("Allow Target Switch", &Modules::Aimbot::AllowTargetSwitch);
            ImGui::Separator();
            ImGui::SliderFloat("Speed", &Modules::Aimbot::Speed, 35, 90, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Speed of the Aim-Assist\nHigher = Faster");
            ImGui::SliderFloat("Smooth", &Modules::Aimbot::Smooth, 5, 25, "%.1f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Smoothness for the Aim-Assist\nHigher = Smoother");
            ImGui::SliderFloat("Extra Smooth", &Modules::Aimbot::ExtraSmooth, 100, 10000, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Extra Smoothness for the Aim-Assist\nHigher = Smoother");
            ImGui::SliderFloat("FOV", &Modules::Aimbot::FOV, 4, 50, "%.1f");
            ImGui::Checkbox("White FOV Circle", &Modules::ESP::DrawFOVCircle);
            ImGui::SameLine();
            ImGui::Checkbox("Rainbow FOV Circle", &Modules::ESP::DrawRainbowCircle);
            ImGui::Separator();
            ImGui::SliderFloat("Min Distance", &Modules::Aimbot::MinDistance, 1, 100, "%.0f");
            ImGui::SliderFloat("Max Distance", &Modules::Aimbot::MaxDistance, 1, 500, "%.0f");
        }
        //--------------------------------[No-Recoil]-------------------------------
        if (ImGui::CollapsingHeader("Recoil Control - Settings", nullptr)) {
            ImGui::Checkbox("Recoil Control", &Modules::NoRecoil::Enabled);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Reduce the intensity of weapon's recoil.");
            ImGui::SliderFloat("Pitch", &Modules::NoRecoil::PitchPower, 1, 30, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Pitch Power");
            ImGui::SliderFloat("Yaw", &Modules::NoRecoil::YawPower, 1, 30, "%.0f");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Yaw Power");
        }
        //--------------------------------[TriggerBot]-------------------------------
        if (ImGui::CollapsingHeader("Trigger Bot - Settings", nullptr)) {
            ImGui::Checkbox("Triggerbot", &Modules::TriggerBot::Enabled);
            ImGui::SliderFloat("Hip Fire Range", &Modules::TriggerBot::HipFireRange, 0, 60, "%.0f");
            ImGui::SliderFloat("Triggerbot Range", &Modules::TriggerBot::TriggerbotRange, 0, 300, "%.0f");   
        }
        //--------------------------------[Radar]-----------------------------------
        if (ImGui::CollapsingHeader("Radar - Settings", nullptr)) {
            ImGui::Checkbox("Mini Map Radar", &Modules::Radar::miniRadar);
            ImGui::Checkbox("Main Map Radar", &Modules::Radar::bigRadar);
            ImGui::Checkbox("Mini Map Guides", &Modules::Radar::MiniMapGuides);
            ImGui::SliderFloat("Mini Map Radar Range", &Modules::Radar::miniMapRange, 0, 200, "%.0f");
            ImGui::SliderInt("Mini Map Radar Dot Size", &Modules::Radar::minimapradardotsize1, 0, 10);
            ImGui::SliderInt("Mini Map Radar Black BG Size", &Modules::Radar::minimapradardotsize2, 0, 10);
            int mainMapHotKey = static_cast<int>(Modules::Radar::bigMapHotKey);
            ImGui::Combo("Main Map HotKey##Radar", &mainMapHotKey, InputKeyTypeNames, IM_ARRAYSIZE(InputKeyTypeNames));
            Modules::Radar::bigMapHotKey = static_cast<InputKeyType>(mainMapHotKey);
        }
        //--------------------------------[Sense]-----------------------------------
        if (ImGui::CollapsingHeader("ESP - Settings", nullptr)) {
        ImGui::Checkbox("Item Glow", &Modules::ESP::ItemGlow);
        ImGui::SameLine();
        ImGui::Checkbox("Aimed At Only##ESP", &Modules::ESP::AimedAtOnly);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip("Only draw those who are locked on by Aim-Assist");
        ImGui::SameLine();     
        ImGui::Checkbox("Draw Health and Armor##ESP", &Modules::ESP::DrawSeer);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip("Draw Health Bar and Armor");
        ImGui::Checkbox("Show Spectators", &Modules::ESP::ShowSpectators);
        ImGui::SameLine();
        ImGui::Checkbox("Snap Lines##ESP", &Modules::ESP::DrawTracers);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip("Draw lines to enemies");
        ImGui::SameLine();
        ImGui::Checkbox("Distance to enemies##ESP", &Modules::ESP::DrawDistance);
        ImGui::Checkbox("Player Names##ESP", &Modules::ESP::DrawNames);
        ImGui::SameLine();
        ImGui::Checkbox("Player Levels##ESP", &Modules::ESP::printPlayersLvl);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip("To be used in other game modes beside Trios or Duo, doesn't work well with 60 players");
        ImGui::SameLine();
        ImGui::Checkbox("Super Glide", &Modules::ESP::SuperGlide);
        ImGui::SliderFloat("Colored ESP Max Distance", &Modules::ESP::GlowMaxDistance, 0, 300, "%.0f");
        ImGui::SliderFloat("Draw Items Distance", &Modules::ESP::SeerMaxDistance, 0, 300, "%.0f");
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip("Only draw those in range.");
        }
        ImGui::EndTabItem();
    }
    //--------------------------------[Credits]-----------------------------------
    if (ImGui::BeginTabItem("Credits", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton | ImGuiTabItemFlags_NoReorder)) {
        ImGui::TextColored(ImVec4(0.65, 1, 0.95, 1), "Credits:");
        ImGui::TextColored(ImVec4(0.03, 1, 0.95, 1), "arturzxc: ESP, Triggerbot and X11Display's Move Mouse method");
        ImGui::TextColored(ImVec4(0.3, 1, 0.64, 1), "Koelion: ImGui Menu, AimbotResolver");
        ImGui::TextColored(ImVec4(0.3, 0.6, 0.9, 1), "unknowncheats: Offsets and ton of other things");
        ImGui::TextColored(ImVec4(0.6, .1, 0.64, 1), "Made by Azreol/Nexilist/Acknowledge");
        ImGui::EndTabItem();
    }
}
