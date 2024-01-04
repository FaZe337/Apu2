// Externals
#include <iostream>
#include <string>
#include <unistd.h>
#include <atomic>
#include <vector>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h>

// Internals
#include "../Cuore/Entities/LocalPlayer.hpp"
#include "../Cuore/Entities/Players.hpp"
#include "../Cuore/Overlay/Scene/Scene.hpp"
#include "../Cuore/Util/Structs.hpp"
#include "./Modules/Aim.hpp"
#include "./Modules/ESP.hpp"
#include "./Modules/Radar.hpp"
#include "./Modules/Trigger.hpp"
#include "./Modules/NoRecoil.hpp"

#include "../Utilities/Config.hpp"
#include "../Utilities/Config.hpp"
#include "../Cuore/Util/MyDisplay.hpp"
#include "../Memory/Mem.hpp"

// Objects
MyDisplay *X11Display = new MyDisplay();
Overlay OverlayWindow = Overlay();
ImDrawList *Canvas;

// Game Objects
Level *Map = new Level();
LocalPlayer *Myself = new LocalPlayer();
Camera *GameCamera = new Camera();

// Players
std::vector<Player *> *HumanPlayers = new std::vector<Player *>;
std::vector<Player *> *Dummies = new std::vector<Player *>;
std::vector<Player *> *Players = new std::vector<Player *>;

// Features
Sense *ESP = new Sense(X11Display, Players, GameCamera, Map, Myself);
Radar *MapRadar = new Radar(X11Display, Players, GameCamera, Map, Myself);
Aimbot *AimAssist = new Aimbot(X11Display, Myself, Players, Map);
Triggerbot *Trigger = new Triggerbot(X11Display, Myself, Players);
NoRecoil *Rec = new NoRecoil(X11Display,Myself);

// Booleans and Variables
bool IsMenuOpened = true;

// Thread
std::atomic_bool StopThread(false);

// Inputs
void HandleKeyEvent(Display *display, XEvent *Event)
{
    if (Event->type == KeyPress)
    {
        if (IsMenuOpened)
        {
            IsMenuOpened = false;
            OverlayWindow.CaptureInput(false);
        }
        else
        {
            IsMenuOpened = true;
            OverlayWindow.CaptureInput(true);
        }
    }
}

void X11EventListener()
{
    Display *display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);
    XGrabKey(display, XKeysymToKeycode(display, XK_Insert), AnyModifier, root, False, GrabModeAsync, GrabModeAsync);
    XEvent event;
    while (!StopThread)
    {
        XNextEvent(display, &event);
        HandleKeyEvent(display, &event);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    XCloseDisplay(display);
}

// Overlay
bool InitializeOverlayWindow()
{
    if (!OverlayWindow.InitializeOverlay())
    {
        OverlayWindow.DestroyOverlay();
        return false;
    }
    int ScreenWidth;
    int ScreenHeight;
    OverlayWindow.GetScreenResolution(ScreenWidth, ScreenHeight);
    GameCamera->Initialize(ScreenWidth, ScreenHeight);
    std::cout << "overlay initialized" << std::endl;
    return true;
}

void SaveConfig()
{
    if (!GameModeSave())
        std::cout << "Error saving Game Mode settings" << std::endl;
    if (!AimbotSave())
        std::cout << "Error saving Aimbot settings" << std::endl;
    if (!ESPSave())
        std::cout << "Error saving ESP settings" << std::endl;
    if (!RadarSave())
        std::cout << "Error saving Radar settings" << std::endl;
    if (!TriggerSave())
        std::cout << "Error saving Triggerbot settings" << std::endl;
    if (!NoRecoilSave())
        std::cout << "Error saving NoRecoil settings" << std::endl;
    UpdateConfig();
}

// Interface
ImVec4 ProcessingTimeColor;
void RenderUI()
{
    auto io = ImGui::GetIO();
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("##Overlay", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoBackground);
    Canvas = ImGui::GetWindowDrawList();
    if (Map->playable)
        ESP->RenderDrawings(Canvas, AimAssist, Myself, OverlayWindow);
        MapRadar->RenderDrawings(Canvas, AimAssist, Myself, OverlayWindow);
    ImGui::End();

    if (!IsMenuOpened)
        return;

    // Menu
    ImGui::SetNextWindowSizeConstraints(ImVec2(540, 480), ImVec2(540, 480));
    ImGui::SetNextWindowSize(ImVec2(580, 580), ImGuiCond_FirstUseEver);
    ImGui::Begin("Apu-Pesci", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    ProcessingTimeColor = OverlayWindow.ProcessingTime > 20 ? ProcessingTimeColor = ImVec4(1, 0.343, 0.475, 1) : ProcessingTimeColor = ImVec4(0.4, 1, 0.343, 1);
    ImGui::TextColored(ProcessingTimeColor, "Processing Time: %02dms", OverlayWindow.ProcessingTime);
    ImGui::SameLine();
    if (OverlayWindow.AlignedButton("Save Config", 0.8f))
    {
        SaveConfig();
        std::cout << "Config saved" << std::endl;
    }
    ImGui::SameLine();
    if (OverlayWindow.AlignedButton("CLOSE", 1.0f))
    {
        OverlayWindow.DestroyOverlay();
        std::system("clear");
        std::cout << "Closing Overlay.." << std::endl;
    }

    if (ImGui::BeginTabBar("Menus"), ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)
    {
        // Draw Scene
        SceneRenderUI();
        ImGui::EndTabBar();
    }
    ImGui::End();
}

// Core
bool UpdateCore()
{
   
    try
    {
        // Map Checking //
        Map->readFromMemory();
        if (!Map->playable)
        {
            return true;
        }
        // Read Local Player //
        Myself->Read();
        if (!Myself->IsValid())
        {
            return true;
        }

        // Populate Players //
        Players->clear();
        if (Map->trainingArea)
        {
            for (int i = 0; i < Dummies->size(); i++)
            {
                Player *p = Dummies->at(i);
                p->Read();
                if (p->BasePointer != 0 && (p->IsPlayer() || p->IsDummy()))
                    Players->push_back(p);
            }
        }
        else
        {
            for (int i = 0; i < HumanPlayers->size(); i++)
            {
                Player *p = HumanPlayers->at(i);
                p->Read();
                if (p->BasePointer != 0 && (p->IsPlayer() || p->IsDummy()))
                    Players->push_back(p);
            }
        }

        // Updates //
        GameCamera->Update();
        ESP->Update();
        AimAssist->Update();
        Trigger->Update();
        Rec->controlWeapon();
        return true;
    }
    catch (const std::exception &ex)
    {
        std::system("clear");
        std::cout << "Errore: " << ex.what() << std::endl;
        return true;
    }

    return false;
}

// Main
int main(int argc, char *argv[])
{
    if (getuid())
    {
        std::cout << "Please run as sudo!" << std::endl;
        return -1;
    }

    // Waiting for Apex Legends
    while (mem::GetPID() == 0)
    {
        std::system("clear");
        std::cout << "waiting for apex..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    std::system("clear");
    std::cout << "apu ver 1.0" << std::endl;

    // Initialize Overlay Window //
    if (!InitializeOverlayWindow())
        return -1;

    // Theading //
    std::thread X11Thread(X11EventListener);
    X11Thread.detach();
    std::thread InputManagerThread(InputManager::run);
    InputManagerThread.detach();

    // Initialize the whole process //
    try
    {
        for (int i = 0; i < 70; i++)
            HumanPlayers->push_back(new Player(i, Myself));

        for (int i = 0; i < 15000; i++)
            Dummies->push_back(new Player(i, Myself));
        std::cout << "core initialized" << std::endl;
        std::cout << "---------------------------" << std::endl;

        LoadConfig();
        // This is where the fun starts //
        OverlayWindow.Start(&UpdateCore, &RenderUI);
    }
    catch (...)
    {
    }

    StopThread = true;
    InputManager::StopThread = true;
    X11Thread.join();
    InputManagerThread.join();

    return 0;
}