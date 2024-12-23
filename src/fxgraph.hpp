#ifndef FX_GRAPH_H
#define FX_GRAPH_H


#include <vector>
#include <fmt/core.h>
#include <fx_task.hpp>

class Graph {
public:
    Graph() ;
};

struct FxInputNode {
    size_t task;
    size_t output;
};

struct FxOutputNode {
    size_t task;
    size_t input;
};

struct FxNode {
    bool isInput;
    size_t numInputs;
    FxTask* task;
    // std::string texturePath; // this is a hack, we should have a better way to handle this
    // hmm actually maybe not... it makes it easier to traverse the graph
    // std::vector<size_t> inputs; // a lack of foresight here, we could consider types of input/output
    std::vector<size_t> outputs; 

    FxNode(FxTask* task) : task(task) { isInput = true; numInputs = 0; }
};

struct TextureBuffer {
    GLuint id;
    GLuint width;
    GLuint height;
    GLuint channels;

    size_t srcNode;
    size_t currNode;

    TextureBuffer(GLuint id, GLuint width, GLuint height, GLuint channels)
        : id(id), width(width), height(height), channels(channels) {}

};

class FxGraph {

    public:
    // std::vector<FxTask*> tasks;
    std::vector<FxNode> nodes;


    std::vector<TextureBuffer> textureBuffers;

    FxGraph();
    ~FxGraph();
    void InitTextureBuffers();
    void AddNode();
    void SetNodeTask(size_t nodeIndex, FxTask* task);
    void addConnection(size_t srcNode, size_t dstNode);
    void RemoveConnection(size_t srcNode, size_t dstNode);
    void RunGraph();

};

#endif 