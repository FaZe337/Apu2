#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include <vector>

#include "./Aim.hpp"
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
#include "../Utilities/Config.hpp"

#include "../../Cuore/Entities/LocalPlayer.hpp"
#include "../../Cuore/Util/MyDisplay.hpp"
#include "../../Cuore/Util/Structs.hpp"
#include "../../Cuore/Util/Offsets.hpp"
#include "../../Cuore/Util/QAngle.hpp"
#include "../../Cuore/Util/Resolver.hpp"
#include "../../Cuore/Overlay/Rendering/Overlay.hpp"
#include "../../Cuore/Overlay/Rendering/Drawings.hpp"

// Geometry
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )

//===-------[DRAW RADAR POINT mini Map]--------------------======================
void DrawRadarPointMiniMap(Vector3D EneamyPos, Vector3D LocalPos, float targetY, float enemyDist, int TeamID, int xAxis, int yAxis, int width, int height, ImColor color, float targetyaw)
{
	bool out = false;
	Vector3D siz;
	siz.x = width;
	siz.y = height;
	Vector3D pos;
	pos.x = xAxis;
	pos.y = yAxis;
	bool ck = false;
	Vector3D single = Drawings::RotatePoint(EneamyPos, LocalPos, pos.x, pos.y, siz.x, siz.y, targetY, 0.3f, &ck);
	if (enemyDist >= 0.f && enemyDist < Modules::Radar::miniMapRange)
	{
		for (int i = 1; i <= 30; i++)
		{
			Drawings::TeamMiniMap(single.x, single.y, Modules::Radar::minimapradardotsize1, TeamID, targetyaw);
		}
	}
}

//===-------[MINI MAP RADAR IMPLEMENTATION]======--------------===================
void MiniMapRadar(Vector3D EneamyPos, Vector3D LocalPos, float targetY, float eneamyDist, int TeamId, float targetyaw)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowRounding = 0.2f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13529413f, 0.14705884f, 0.15490198f, 0.82f));
	ImGuiWindowFlags TargetFlags;
	TargetFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;	
	if (Modules::Radar::miniRadar == true)
	{
		ImGui::SetNextWindowSize({ 215, 215 });
		ImGui::Begin(("##Radar"), 0, TargetFlags);
		{
			ImDrawList* Draw = ImGui::GetWindowDrawList();
			ImVec2 DrawPos = ImGui::GetCursorScreenPos();
			ImVec2 DrawSize = ImGui::GetContentRegionAvail();
			ImVec2 midRadar = ImVec2(DrawPos.x + (DrawSize.x / 2), DrawPos.y + (DrawSize.y / 2));
			if (Modules::Radar::MiniMapGuides)
			{
				ImVec2 startHorizontal(midRadar.x - DrawSize.x / 2.f, midRadar.y);
				ImVec2 endHorizontal(midRadar.x + DrawSize.x / 2.f, midRadar.y);
				ImVec2 startVertical(midRadar.x, midRadar.y - DrawSize.y / 2.f);
				ImVec2 endVertical(midRadar.x, midRadar.y + DrawSize.y / 2.f);
				float extension = -500000.f;
				
				ImGui::GetWindowDrawList()->AddLine(startHorizontal, ImVec2(startHorizontal.x - extension, endHorizontal.y), IM_COL32(255, 255, 255, 255));
				ImGui::GetWindowDrawList()->AddLine(endHorizontal, ImVec2(endHorizontal.x + extension, endHorizontal.y), IM_COL32(255, 255, 255, 255));

	
				ImGui::GetWindowDrawList()->AddLine(startVertical, ImVec2(startVertical.x, startVertical.y - extension), IM_COL32(255, 255, 255, 255));
				ImGui::GetWindowDrawList()->AddLine(endVertical, ImVec2(endVertical.x, endVertical.y + extension), IM_COL32(255, 255, 255, 255));		}
							
			DrawRadarPointMiniMap(EneamyPos, LocalPos, targetY, eneamyDist, TeamId, DrawPos.x, DrawPos.y, DrawSize.x, DrawSize.y, { 255, 255, 255, 255 }, targetyaw);
		}
		ImGui::End();
	}
	ImGui::PopStyleColor();
}

struct Radar {
    MyDisplay* X11Display;
    Camera* GameCamera;
    Level* m_level;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;
    std::chrono::milliseconds LastUpdateTime;
 
    Radar(MyDisplay* X11Display, std::vector<Player*>* Players, Camera* GameCamera, Level* level, LocalPlayer* localplayer) {
        this->X11Display = X11Display;
        this->Players = Players;
        this->GameCamera = GameCamera;
        this->m_level = level;
        this->Myself = localplayer;
    }
    void RenderDrawings(ImDrawList* Canvas, Aimbot* AimAssistState, LocalPlayer* Myself, Overlay OverlayWindow) {
        int ScreenWidth;
        int ScreenHeight;
        OverlayWindow.GetScreenResolution(ScreenWidth, ScreenHeight);
        if (!m_level->playable) return;
		
       if(Modules::Radar::miniRadar){
			ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(ImVec2(0.0f, Center.y), ImGuiCond_Once, ImVec2(0.02f, 0.5f));
			
			ImGui::Begin(("RADAR"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);

			for (auto i = 0; i < Players->size(); i++)
			{
				Player *p = Players->at(i);
				if (!p->IsHostile || !p->IsValid() || Myself->BasePointer == p->BasePointer) continue;

				float radardistance = (int)((Myself->LocalOrigin, p->Distance2DToLocalPlayer) / 39.62);

				MiniMapRadar(p->LocalOrigin, Myself->LocalOrigin, p->ViewAngles.y, radardistance, p->Team, p->ViewYaw);
			} 
			ImGui::End();                   
        }

		if (Modules::Radar::bigRadar && InputManager::isKeyDownOrPress(Modules::Radar::bigMapHotKey)) {
			uint64_t pLocal = mem::Read<uint64_t>(OFF_REGION + OFF_LOCAL_PLAYER);
			int currentTEAM = mem::Read<int>(pLocal + OFF_TEAM_NUMBER);

			for (uintptr_t i = 0; i <= 80000; i++)
			{
			mem::Write<int>(pLocal + OFF_TEAM_NUMBER, 1);
			}
			for (uintptr_t i = 0; i <= 80000; i++)
			{
			mem::Write<int>(pLocal + OFF_TEAM_NUMBER, currentTEAM);
			} 
		}
    }   
};