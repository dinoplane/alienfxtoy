#include <fxgraph.hpp>
#include <algorithm>
#include <fmt/core.h>
#include <fmt/ostream.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


Graph::Graph() { fmt::print("HIYA"); }

FxGraph::FxGraph(){
    glCreateBuffers(1, &PBO);


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
        if (node.task != nullptr)
            delete node.task;
    }

    nodes.clear();
    // for (auto& task : tasks){
    //     delete task;
    // }
    glDeleteBuffers(1, &PBO);
}

static inline bool ValidateNodeIdx(FxGraph* graph, size_t idx){
    return 0 <= idx && idx < graph->nodes.size();
}

void FxGraph::AddNode(){
    nodes.emplace_back(FxNode());
}

void FxGraph::SetNodeTask(size_t nodeIndex, FxTask* task){
    if (!ValidateNodeIdx(this, nodeIndex)){
        fmt::print("Invalid node index: {}\n", nodeIndex);
        return;
    }
    nodes[nodeIndex].task = task;
}

void FxGraph::addConnection(size_t srcNode, size_t dstNode){
    if (!ValidateNodeIdx(this, srcNode) || !ValidateNodeIdx(this, dstNode)){
        fmt::print("Invalid node index: {} {}\n", srcNode, dstNode);
        return;
    }

    nodes[srcNode].outputs.push_back(dstNode);
    nodes[dstNode].isInput = false;
    ++nodes[dstNode].numInputs;
}

void FxGraph::RemoveConnection(size_t srcNode, size_t dstNode){
    if (!ValidateNodeIdx(this, srcNode) || !ValidateNodeIdx(this, dstNode)){
        fmt::print("Invalid node index: {} {}\n", srcNode, dstNode);
        return;
    }

    nodes[srcNode].outputs.erase(
        std::remove(
            nodes[srcNode].outputs.begin(),
            nodes[srcNode].outputs.end(),
            dstNode
        ),
        nodes[srcNode].outputs.end()
    );
    --nodes[dstNode].numInputs;
    if (nodes[dstNode].numInputs == 0){
        nodes[dstNode].isInput = true;
    }
}

void FxGraph::InitTextureBuffers(){
    for (size_t nodeIdx = 0; nodeIdx < nodes.size(); nodeIdx++){
        FxNode& node = nodes[nodeIdx];
        if (node.isInput){
            FxLoadTask *loadTask = static_cast<FxLoadTask*>(node.task);
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
            textureBuffers.back().srcNodeIdx = nodeIdx;
            textureBuffers.back().currNodeIdx = nodeIdx;

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
    PrintGraph();

    // InitTextureBuffers();
    // // // BFS
    // // for (TextureBuffer& buffer : textureBuffers){
    // //     if (buffer.srcNode->outputs.size() == 0){
    // //         buffer.srcNode->task->RunTask({buffer.id, 0, buffer.width, buffer.height});
    // //     }
    // // }
    // std::ofstream debuglog("fxdebug.log");
    // FxNode& currNode = nodes[textureBuffers[0].currNodeIdx];
    // FxTaskInput input = {textureBuffers[0].inputID, textureBuffers[0].outputID, textureBuffers[0].width, textureBuffers[0].height};
    // fmt::print(debuglog, "Running Load Task\n In {} Out{}\n w {} h{}\n", input.inputTexture, input.outputTexture, input.width, input.height);

    // currNode.task->RunTask(input);

    // currNode = nodes[currNode.outputs[0]];
    
    // input = {textureBuffers[0].outputID, textureBuffers[0].inputID, textureBuffers[0].width, textureBuffers[0].height};

    // fmt::print(debuglog, "Running Compute Task\n In {} Out{}\n w {} h{}\n", input.inputTexture, input.outputTexture, input.width, input.height);
    // currNode.task->RunTask(input);

    // GLuint imageSize = input.width * input.height * 4;
    // GLubyte* data = new GLubyte[imageSize];
    // // glGetTextureImage(input.outputTexture, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSize * sizeof(GLubyte), data);
    // glNamedBufferData(PBO, imageSize * sizeof(GLubyte), NULL,GL_STREAM_READ);

    // glBindBuffer(GL_PIXEL_PACK_BUFFER, PBO);

    // // // float* data;
    // glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);



    // if(ptr)
    // {
    //     stbi_write_png("output.png", input.width, input.height, 4, ptr, 0);

    //     glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    // }

    // // // back to conventional pixel operation
    // // glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    // debuglog.close();
    
    // // Delete TextureBuffers

    // while (currNode != nullptr){
    //     FxTaskInput input = {textureBuffers[0].id, 0, textureBuffers[0].width, textureBuffers[0].height};

    //     currNode = graph.nodes[currNode->outputs[0]];
    // }



    // for (auto& node : nodes){
    //     node.task->run();
    // }
}

void FxGraph::PrintGraph(){
    fmt::print("Graph:\n");
    for (size_t i = 0; i < nodes.size(); i++){
        fmt::print("-----------------------------------\nNode {}\n-----------------------------------\n", i);
        if (nodes[i].task != nullptr)
            nodes[i].task->PrintTask();
        else fmt::print("Empty Task\n");
        fmt::print("Outputs: ");
        for (size_t j = 0; j < nodes[i].outputs.size(); j++){
            fmt::print("{} ", nodes[i].outputs[j]);
        }
        fmt::print("\n-----------------------------------\n");
    }
}
