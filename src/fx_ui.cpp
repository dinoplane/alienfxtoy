#include <fx_ui.hpp>

static void RenderTaskSelector(FxNode* node, FxNodeUIState* nodeUIState){
    const FxTask::FxTaskType selectedTaskType = (node->task == nullptr) ? FxTask::FxTaskType::Empty : node->task->type;
    if (ImGui::BeginCombo("Task", FxTask::FxTaskNames[selectedTaskType])){
        int item_selected_idx = -1;
        for (int n = 0; n < FxTask::FxTaskType::MAX_TYPES + 1; ++n)
        {
            const bool is_selected = (node->task == nullptr && n == 0) || (node->task != nullptr  && n == node->task->type);
            if (ImGui::Selectable(FxTask::FxTaskNames[n], is_selected))
                item_selected_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        if (item_selected_idx > -1){
            if ((*node).task != nullptr){
                delete (*node).task;
            }
            (*node).task = FxTask::CreateTask((FxTask::FxTaskType) item_selected_idx);
            (*nodeUIState).status = FxUi::InitTaskStatus[item_selected_idx];
        }
        // graph->SetNodeTask(idx, &graph->tasks[idx]); 
        ImGui::EndCombo();
    }
}

static void RenderComputeTask(FxComputeTask* computeFxTask, FxNodeUIState* nodeUIState){
    // FxComputeTask* computeFxTask = static_cast<FxComputeTask*>(node.task);
    ImGui::Text(nodeUIState->status.c_str());
    if (ImGui::Button("Load New Shader")) {
        // Open a dialog
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog( "glsl,comp,frag,vert", NULL, &outPath );
            
        if ( result == NFD_OKAY ) {
            
            computeFxTask->SetShaderPath(outPath);
            if (!computeFxTask->LoadShader()) {
                (*nodeUIState).status = fmt::format("Invalid Shader: {}\n", outPath);
            } else {
                (*nodeUIState).status = fmt::format("Loaded Shader: {}\n", outPath);
            }
            free(outPath);
        }
        else if ( result == NFD_CANCEL ) {
            (*nodeUIState).status = "Loaded Shader: Cancelled\n";
        }
        else {
            (*nodeUIState).status = fmt::format("Error: %s\n", NFD_GetError() );
        }
    }
    if (ImGui::Button("Clear")) {
        computeFxTask->ClearShader();
        (*nodeUIState).status = "Loaded Shader: None\n";
    }
}

static void RenderLoadTask(FxLoadTask* loadFxTask, FxNodeUIState* nodeUIState){
    ImGui::Text(nodeUIState->status.c_str());
    if (ImGui::Button("Load New Image")) {

        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog( "png,jpg,bmp,jpeg", NULL, &outPath );
            
        if ( result == NFD_OKAY ) {
            
            loadFxTask->SetTexturePath(outPath);
            if (!loadFxTask->LoadTexture()) {
                (*nodeUIState).status = fmt::format("Invalid Texture: {}\n", outPath);
            } else {
                (*nodeUIState).status = fmt::format("Loaded Image: {}\n", outPath);
            }
            free(outPath);
        }
        else if ( result == NFD_CANCEL ) {
            (*nodeUIState).status = "Loaded Image: Cancelled\n";
        }
        else {
            (*nodeUIState).status = fmt::format("Error: %s\n", NFD_GetError() );
        }
    }
    if (ImGui::Button("Clear")) {
        loadFxTask->ClearTexture();
        (*nodeUIState).status = "Loaded Image: None\n";
    }
}

void FxUi::RenderFxNodeWindow(FxGraph* graph, FxGraphUIState* graphUIState, size_t idx){
    ImGui::Begin(fmt::format("Node {}", idx).c_str());
    FxNode* node = &graph->nodes[idx];
    FxNodeUIState* nodeUIState = &graphUIState->nodeUIState[idx];
    //FxTask* node.task = node.task;

    RenderTaskSelector(node, nodeUIState);

    if (node->task != nullptr){
        switch (node->task->type) {

            case FxTask::FxTaskType::Compute:
            {
                RenderComputeTask(static_cast<FxComputeTask*>(node->task), nodeUIState);
            } break;

            case FxTask::FxTaskType::Load:
            {
                RenderLoadTask(static_cast<FxLoadTask*>(node->task), nodeUIState);
            } break;
        
        };
    }


    for (size_t i = 0; i < graph->nodes[idx].outputs.size(); i++){
        ImGui::Text(fmt::format("To Node {}", graph->nodes[idx].outputs[i]).c_str());
    }
    // ImGui::InputText("Add Output", buf, 32, ImGuiInputTextFlags_CharsDecimal);

    if (ImGui::Button("+")){
        // size_t output = std::stoul(buf);
        graph->addConnection(idx, 1);
    }
    ImGui::End();

}

void FxUi::RenderFxGraphWindow(FxGraph* graph, FxGraphUIState* graphUIState){
//    ImGui::Begin("FxGraph");
   for (size_t i = 0; i < graph->nodes.size(); i++){
       RenderFxNodeWindow(graph, graphUIState, i);
   }
   ImGui::Begin("Graph");
   if (ImGui::Button("Add Node")){
       graph->AddNode();
       graphUIState->nodeUIState.push_back(FxNodeUIState());
       graphUIState->nodeUIState.back().status = "Loaded: ";
   }

   if (ImGui::Button("Run")) {
       graph->RunGraph();
   }

   ImGui::End();

//    ImGui::End();
}