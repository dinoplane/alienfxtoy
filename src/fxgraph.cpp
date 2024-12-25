#include <fxgraph.hpp>
#include <algorithm>
#include <fmt/core.h>
#include <fmt/ostream.h>


Graph::Graph() { fmt::print("HIYA"); }

FxGraph::FxGraph(){
    
    // // FxTask task1("default", "./assets/shader/default.glsl");
    // tasks.push_back(new FxTask ("baka", "./assets/texture/baka.jpg", true) );
    // tasks.push_back(new FxTask ("default", "./assets/shader/default.glsl") );
    // // tasks.push_back(task1);
    

    // // FxNode node0 = {0, true, "./assets/texture/baka.jpg",  {1}};
    // // FxNode node1 = {1, false, "", {}};
    // // // FxNode node2 = {0, false, "", { }};
    // nodes.push_back(new FxNode {0, true, "./assets/texture/baka.jpg",  {1}});
    // nodes.push_back(new FxNode {1, false, "", {}});
    // // nodes.push_back(node0);
    // nodes.push_back(node1);
    // nodes.push_back(node2);
    // Create a default task
    // tasks.push_back(FxTask("default", "shaders/default.comp"));
    // nodes.push_back(FxNode(&tasks[0]));
}

FxGraph::~FxGraph(){
    for (auto& node : nodes){
        delete node;
    }

    nodes.clear();
    // for (auto& task : tasks){
    //     delete task;
    // }
}

void FxGraph::AddNode(){
    nodes.emplace_back(new FxNode());
}

void FxGraph::SetNodeTask(size_t nodeIndex, FxTask* task){
    nodes[nodeIndex]->task = task;
}

void FxGraph::addConnection(size_t srcNode, size_t dstNode){
    nodes[srcNode]->outputs.push_back(dstNode);
    nodes[dstNode]->isInput = false;
    ++nodes[dstNode]->numInputs;
}

void FxGraph::RemoveConnection(size_t srcNode, size_t dstNode){
    nodes[srcNode]->outputs.erase(
        std::remove(
            nodes[srcNode]->outputs.begin(),
            nodes[srcNode]->outputs.end(),
            dstNode
        ),
        nodes[srcNode]->outputs.end()
    );
    --nodes[dstNode]->numInputs;
    if (nodes[dstNode]->numInputs == 0){
        nodes[dstNode]->isInput = true;
    }
}

void FxGraph::InitTextureBuffers(){
    for (auto& node : nodes){
        if (node->isInput){
            FxLoadTask *loadTask = static_cast<FxLoadTask*>(node->task);
            GLuint inID, outID;
            glCreateTextures(GL_TEXTURE_2D, 1, &inID);

            glTextureParameteri(inID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTextureParameteri(inID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glTextureParameteri(inID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(inID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureStorage2D(inID, 1, GL_RGBA32F, loadTask->loadedTexture.width, loadTask->loadedTexture.height);


            glCreateTextures(GL_TEXTURE_2D, 1, &outID);

            glTextureParameteri(outID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTextureParameteri(outID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glTextureParameteri(outID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(outID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureStorage2D(outID, 1, GL_RGBA32F, loadTask->loadedTexture.width, loadTask->loadedTexture.height);
            

            textureBuffers.emplace_back(inID, outID, loadTask->loadedTexture.width, loadTask->loadedTexture.height, loadTask->loadedTexture.channels);
            textureBuffers.back().srcNode = node;
            textureBuffers.back().currNode = node;

        }
    }
    // for (auto& task : tasks){
    //     if (task->isInputTask){
    //         textureBuffers.emplace_back(task->inputs[0].id, task->inputs[0].width, task->inputs[0].height, task->inputs[0].channels);
    //         glCreateTextures(GL_TEXTURE_2D, 1, &textureBuffers.back().id);
    //     }
    // }
}
#include <fstream>
void FxGraph::RunGraph(){
    InitTextureBuffers();
    // // BFS
    // for (TextureBuffer& buffer : textureBuffers){
    //     if (buffer.srcNode->outputs.size() == 0){
    //         buffer.srcNode->task->RunTask({buffer.id, 0, buffer.width, buffer.height});
    //     }
    // }
    std::ofstream debuglog("fxdebug.log");
    FxNode *currNode = textureBuffers[0].currNode;
    FxTaskInput input = {textureBuffers[0].inputID, textureBuffers[0].outputID, textureBuffers[0].width, textureBuffers[0].height};
    fmt::print(debuglog, "Running Load Task\n In {} Out{}\n w {} h{}\n", input.inputTexture, input.outputTexture, input.width, input.height);

    currNode->task->RunTask(input);

    currNode = nodes[currNode->outputs[0]];
    
    input = {textureBuffers[0].outputID, textureBuffers[0].inputID, textureBuffers[0].width, textureBuffers[0].height};

    fmt::print(debuglog, "Running Compute Task\n In {} Out{}\n w {} h{}\n", input.inputTexture, input.outputTexture, input.width, input.height);
    currNode->task->RunTask(input);

    debuglog.close();
    


    // while (currNode != nullptr){
    //     FxTaskInput input = {textureBuffers[0].id, 0, textureBuffers[0].width, textureBuffers[0].height};

    //     currNode = graph.nodes[currNode->outputs[0]];
    // }



    // for (auto& node : nodes){
    //     node.task->run();
    // }
}