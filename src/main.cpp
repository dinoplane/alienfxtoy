
//#include <fx_graph.hpp>

#include <glad/glad.h>

#include <SDL.h>


#include <cstdlib>
#include <cstdio> // wait how is this working?
#include <cassert>

#include <fmt/core.h>
#include <fmt/ostream.h>

#include <fxgraph.hpp>
#include <fx_task.hpp>

#include <fstream>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
// #include <fx_task.hpp>

#include <nfd.h>

#undef main


struct GraphUIState {

};

int setupGLAD()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        fmt::print("Failed to initialize GLAD\n");
        return -1;
    }

    return 0;
}


static void RenderFxNodeWindow(FxGraph& graph, size_t idx){
    ImGui::Begin(fmt::format("Node {}", idx).c_str());
    FxNode& node = *graph.nodes[idx];
    //FxTask* node.task = node.task;

    const FxTask::FxTaskType selectedTaskType = (node.task == nullptr) ? FxTask::FxTaskType::Empty : node.task->type;
    if (ImGui::BeginCombo("Task", FxTask::FxTaskNames[selectedTaskType])){
        int item_selected_idx = -1;
        for (int n = 0; n < FxTask::FxTaskType::MAX_TYPES + 1; ++n)
        {
            const bool is_selected = (node.task == nullptr && n == 0) || (node.task != nullptr  && n == node.task->type);
            if (ImGui::Selectable(FxTask::FxTaskNames[n], is_selected))
                item_selected_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        if (item_selected_idx > -1){
            if (graph.nodes[idx]->task != nullptr){
                delete graph.nodes[idx]->task;
            }
            graph.nodes[idx]->task = FxTask::CreateTask((FxTask::FxTaskType) item_selected_idx);
        }
        // graph.SetNodeTask(idx, &graph.tasks[idx]);
        ImGui::EndCombo();
    }

    if (node.task != nullptr){
        switch (node.task->type) {

            case FxTask::FxTaskType::Compute:
            {
                FxComputeTask* computeFxTask = static_cast<FxComputeTask*>(node.task);
                ImGui::Text(fmt::format("Loaded Shader: {}", computeFxTask->GetShaderPath()).c_str());
                if (ImGui::Button("Load New Shader")) {
                    // Open a dialog
                    computeFxTask->SetShaderPath("./assets/shader/kuwahara.glsl");
                    if (!computeFxTask->LoadShader()) {
                        assert(false);
                    }

                }
                if (ImGui::Button("Clear")) {
                    computeFxTask->ClearShader();
                }
            } break;

            case FxTask::FxTaskType::Load:
            {
                FxLoadTask* loadFxTask = static_cast<FxLoadTask*>(node.task);
                ImGui::Text(fmt::format("Loaded Image: {}", loadFxTask->GetTexturePath()).c_str());
                if (ImGui::Button("Load New Image")) {

                    nfdchar_t *outPath = NULL;
                    nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
                        
                    if ( result == NFD_OKAY ) {
                        puts("Success!");
                        puts(outPath);
                        free(outPath);
                    }
                    else if ( result == NFD_CANCEL ) {
                        puts("User pressed cancel.");
                    }
                    else {
                        printf("Error: %s\n", NFD_GetError() );
                    }
                    // Open a dialog
                    // loadFxTask->SetTexturePath("./assets/texture/luigi.png");
                    // if (!loadFxTask->LoadTexture()) {
                    //     assert(false);
                    // }

                }
                if (ImGui::Button("Clear")) {
                    loadFxTask->ClearTexture();
                }
            } break;
        
        };
    }


    for (size_t i = 0; i < graph.nodes[idx]->outputs.size(); i++){
        ImGui::Text(fmt::format("To Node {}", graph.nodes[idx]->outputs[i]).c_str());
    }
    // ImGui::InputText("Add Output", buf, 32, ImGuiInputTextFlags_CharsDecimal);

    if (ImGui::Button("+")){
        // size_t output = std::stoul(buf);
        graph.addConnection(idx, 1);
    }
    ImGui::End();

}


static void RenderFxGraphWindow(FxGraph& graph){
//    ImGui::Begin("FxGraph");
   for (size_t i = 0; i < graph.nodes.size(); i++){
       RenderFxNodeWindow(graph, i);
   }
   ImGui::Begin("Graph");
   if (ImGui::Button("Add Node")){
       graph.AddNode();
   }

   if (ImGui::Button("Run")) {
       graph.RunGraph();
   }

   ImGui::End();

//    ImGui::End();
}

// Main code
int main(int, char**)
{
    Graph g;
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 460";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
    SDL_Window* window = SDL_CreateWindow("AlienFXToy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr)
    {
        std::ofstream errlog("fxtoy.log");
        fmt::print(errlog, "Error: SDL_CreateWindow(): {}\n", SDL_GetError());
        errlog.close();
        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        std::ofstream errlog("fxtoy.log");

        fmt::print(errlog, "Error: SDL_GL_CreateContext(): {}\n", SDL_GetError());

        errlog.close();
        return -1;
    }
    
    if (setupGLAD() < 0)
    {
        return -1;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync




    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //FxTask graph;

    FxGraph graph;

    // Main loop
    bool done = false;

    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Hello, world!");                        // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            
            ImGui::End();
        }

        RenderFxGraphWindow(graph);

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}