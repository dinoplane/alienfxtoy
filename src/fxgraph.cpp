#include <fxgraph.hpp>
#include <algorithm>

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
            GLuint textureid;
            glCreateTextures(GL_TEXTURE_2D, 1, &textureid);

            glTextureParameteri(textureid, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTextureParameteri(textureid, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glTextureParameteri(textureid, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(textureid, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureStorage2D(textureid, 1, GL_RGBA32F, loadTask->loadedTexture.width, loadTask->loadedTexture.height);

            

            textureBuffers.emplace_back(loadTask->texture.id, loadTask->texture.width, loadTask->texture.height, loadTask->texture.channels);
            &textureBuffers.back().srcNode = node;
            &textureBuffers.back().currNode = node;

        }
    }
    // for (auto& task : tasks){
    //     if (task->isInputTask){
    //         textureBuffers.emplace_back(task->inputs[0].id, task->inputs[0].width, task->inputs[0].height, task->inputs[0].channels);
    //         glCreateTextures(GL_TEXTURE_2D, 1, &textureBuffers.back().id);
    //     }
    // }
}

void FxGraph::RunGraph(){

    // BFS
    for (TextureBuffer& buffer : textureBuffers){
        if (buffer.srcNode->outputs.size() == 0){
            buffer.srcNode->task->RunTask({buffer.id, 0, buffer.width, buffer.height});
        }
    }



    // for (auto& node : nodes){
    //     node.task->run();
    // }
}