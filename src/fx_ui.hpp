#ifndef FX_UI_HPP
#define FX_UI_HPP

#include <imgui.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"


#include <fmt/core.h>
#include <fmt/ostream.h>

#include <fxgraph.hpp>
#include <fx_task.hpp>

#include <nfd.h>

struct FxNodeUIState {
    // ImVec2 pos;
    // ImVec2 size;
    // ImVec2 inputSlot;
    // ImVec2 outputSlot;
    std::string status;
};

struct FxGraphUIState {
    std::vector<FxNodeUIState> nodeUIState;
};

namespace FxUi {



    static const char* InitTaskStatus[]{
        "Loaded: None",
        "Loaded Image: None",
        "Loaded Shader: None"
    };
    void RenderFxNodeWindow(FxGraph* graph, FxGraphUIState* graphUIState, size_t idx);
    void RenderFxGraphWindow(FxGraph* graph, FxGraphUIState* graphUIState);



};

#endif // FX_UI_HPP