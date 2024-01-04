// Externals
#pragma once
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#define GLFW_EXPOSE_NATIVE_X11
#include "../imgui/imgui.h"
#include "../imgui/Font.hpp"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../Utilities/InputManager.hpp"

class Overlay {
private:
    GLFWwindow* OverlayWindow;
    int ScreenWidth;
    int ScreenHeight;

    void GrabScreenSize() {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(primaryMonitor);
        ScreenWidth = vidMode->width;
        ScreenHeight = vidMode->height;
    }

    std::string RandomString(int ch) {
        char alpha[35] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                            'h', 'i', 'j', 'k', 'l', 'm', 'n',
                            'o', 'p', 'q', 'r', 's', 't', 'u',
                            'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        std::string result = "";
        for (int i = 0; i<ch; i++)
            result = result + alpha[rand() % 35];
        return result;
    }

    static void GLFWErrorCallback(int error, const char *description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    long long CurrentEpochMilliseconds() {
        auto currentTime = std::chrono::system_clock::now();
        auto duration = currentTime.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    ImWchar *GetFontGlyphRanges() noexcept
    {
        static ImVector<ImWchar> ranges;
        if (ranges.empty())
        {
            ImFontGlyphRangesBuilder builder;
            constexpr ImWchar baseRanges[] = {
                0x0100, 0x024F, // Latin Extended-A + Latin Extended-B
                0x0300, 0x03FF, // Combining Diacritical Marks + Greek/Coptic
                0x0600, 0x06FF, // Arabic
                0x0E00, 0x0E7F, // Thai
                0
            };
            builder.AddRanges(baseRanges);
            builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
            builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesDefault());
            builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
            builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
            // ★龍王™
            builder.AddChar(u'\u2605'); // ★
            builder.AddChar(u'\u9F8D'); // 龍
            builder.AddChar(u'\u738B'); // 王
            builder.AddChar(u'\u2122'); // ™
            builder.BuildRanges(&ranges);
        }
        return ranges.Data;
    }

public:
    int ProcessingTime;
    long long StartTime;
    int SleepTime;
    int TimeLeftToSleep;

    bool InitializeOverlay() {
        glfwSetErrorCallback(GLFWErrorCallback);
        if (!glfwInit()) {
            return false;
        }

        GrabScreenSize();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwDefaultWindowHints();

        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_REFRESH_RATE, 60);

        OverlayWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, RandomString(12).c_str(), NULL, NULL);

        CaptureInput(true);
        glfwMakeContextCurrent(OverlayWindow);
        
        // Centering //
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(primaryMonitor);
        glfwSetWindowPos(OverlayWindow, (vidMode->width - ScreenWidth) / 2, (vidMode->height - ScreenHeight) / 2);
        // End of Centering //

        InitializeUI();

        glfwShowWindow(OverlayWindow);
        glfwSwapInterval(1);

        return true;
    }

    void InitializeUI() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImFontConfig cfg;
        cfg.OversampleH = cfg.OversampleV = 1;
        cfg.PixelSnapH = true;
        cfg.SizePixels = 13.0f;
        cfg.GlyphOffset = {1.0f, -1.0f};
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.Fonts->AddFontFromMemoryCompressedTTF(_compressedFontData, _compressedFontSize, cfg.SizePixels, &cfg, GetFontGlyphRanges());
    
        ImGui::StyleColorsDark();
        SetUIStyle();

        ImGui_ImplGlfw_InitForOpenGL(OverlayWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    bool AlignedButton(const char* label, float alignment = 0.5f) {
        ImGuiStyle& style = ImGui::GetStyle();

        float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;

        float off = (avail - size) * alignment;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        return ImGui::Button(label);
    }

    void SetUIStyle() {
   ImGuiStyle& style = ImGui::GetStyle();
style.WindowPadding                     = ImVec2(15.0f, 15.0f);
style.FramePadding                      = ImVec2(5.0f, 5.0f);
style.CellPadding                       = ImVec2(8.0f, 8.0f);
style.ItemSpacing                       = ImVec2(12.0f, 8.0f);
style.ItemInnerSpacing                  = ImVec2(8.0f, 6.0f);
style.TouchExtraPadding                 = ImVec2(1.0f, 1.0f);
style.IndentSpacing                     = 25.f;
style.ScrollbarSize                     = 15.0f;
style.GrabMinSize                       = 5.0f;
style.WindowBorderSize                  = 1.0f;
style.ChildBorderSize                   = 1.0f;
style.PopupBorderSize                   = 1.0f;
style.FrameBorderSize                   = 1.0f;
style.TabBorderSize                     = 1.0f;
style.WindowRounding                    = 5.0f;
style.ChildRounding                     = 3.0f;
style.FrameRounding                     = 4.0f;
style.PopupRounding                     = 3.0f;
style.ScrollbarRounding                 = 9.0f;
style.GrabRounding                      = 3.0f;
style.LogSliderDeadzone                 = 4.0f;
style.TabRounding                       = 3.0f;

ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Text]                   = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
colors[ImGuiCol_TextDisabled]           = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
colors[ImGuiCol_ChildBg]                = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
colors[ImGuiCol_PopupBg]                = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
colors[ImGuiCol_Border]                 = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
colors[ImGuiCol_BorderShadow]           = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
colors[ImGuiCol_FrameBg]                = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
colors[ImGuiCol_FrameBgActive]          = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
colors[ImGuiCol_TitleBgActive]          = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
colors[ImGuiCol_MenuBarBg]              = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
colors[ImGuiCol_CheckMark]              = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
colors[ImGuiCol_SliderGrab]             = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
colors[ImGuiCol_Button]                 = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
colors[ImGuiCol_ButtonHovered]          = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
colors[ImGuiCol_ButtonActive]           = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
colors[ImGuiCol_Header]                 = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
colors[ImGuiCol_HeaderHovered]          = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
colors[ImGuiCol_HeaderActive]           = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
colors[ImGuiCol_Separator]              = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
colors[ImGuiCol_SeparatorActive]        = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
colors[ImGuiCol_ResizeGrip]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
colors[ImGuiCol_Tab]                    = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
colors[ImGuiCol_TabHovered]             = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
colors[ImGuiCol_TabActive]              = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
colors[ImGuiCol_TabUnfocused]           = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.18f, 0.18f, 0.18f, 0.74f);
colors[ImGuiCol_PlotLines]              = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
colors[ImGuiCol_PlotHistogram]          = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.25f, 0.25f, 0.26f, 1.00f);
colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.40f, 0.40f, 0.45f, 1.00f);
colors[ImGuiCol_TableBorderLight]       = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.27f, 0.27f, 0.27f, 0.07f);
colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
colors[ImGuiCol_NavHighlight]           = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

 
}

    void DestroyOverlay() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        if(OverlayWindow != nullptr) {
            glfwDestroyWindow(OverlayWindow);
        }

        glfwTerminate();
    }

    void CaptureInput(bool capture) {
        glfwSetWindowAttrib(OverlayWindow, GLFW_MOUSE_PASSTHROUGH, capture ? GLFW_FALSE : GLFW_TRUE);
    }

    void FocusOverlay() {
        glfwFocusWindow(OverlayWindow);
    }

    void Start(bool(*Update)(), void(*RenderUI)()) {
        while(!glfwWindowShouldClose(OverlayWindow)) {
            StartTime = CurrentEpochMilliseconds();
            glfwPollEvents();
            glViewport(0, 0, ScreenWidth, ScreenHeight);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, ScreenWidth, ScreenHeight, 0, -1, 1);

            if (Update != nullptr) {
                Update();
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            // Render
            if (RenderUI != nullptr){
                RenderUI();
            }
            
            // Render ImGui and swap buffers
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(OverlayWindow);

            ProcessingTime = static_cast<int>(CurrentEpochMilliseconds() - StartTime);
            SleepTime = 6.97; // 16.67 > 60hz | 6.97 > 144hz
            TimeLeftToSleep = std::max(0, SleepTime - ProcessingTime);
            std::this_thread::sleep_for(std::chrono::milliseconds(TimeLeftToSleep));
        }

        DestroyOverlay();
    }

    void GetScreenResolution(int& Width, int& Height) const {
        Width = ScreenWidth;
        Height = ScreenHeight;
    }
};