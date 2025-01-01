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

    FxNode(FxTask* task = nullptr) : task(task) { isInput = true; numInputs = 0; }
};

struct TextureBuffer {
    GLuint inputID;
    GLuint outputID;
    GLuint width;
    GLuint height;
    GLuint channels;

    FxNode* srcNode;
    FxNode* currNode;

    TextureBuffer(GLuint in, GLuint out, GLuint w, GLuint h, GLuint ch)
        : inputID(in), outputID(out), width(w), height(h), channels(ch) {}

};

class FxGraph {

    public:
    GLuint PBO; // preparing for gifs

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


/*
Should Graph have std::vector<FxNode> or std::vector<FxNode*>?

User clicks add node: O(1)
User clicks remove node: O(n)
Traversal: O(n)

Adjacency list is good.
But...


                            |    std::vector<size_t> outputs    |    std::vector<FxNode*> outputs;
----------------------------|-----------------------------------|-------------------------------
std::vector<FxNode>  nodes  |    nodes stack allocated          |    nodes stack allocated
                            |    Addnode gg                     |    Addnode gg
                            |    O(1) add                       |    O(1) add
                            |    O(n) remove                    |    O(n) remove
                            |    O(n) traversal                 |    O(n) traversal
----------------------------|-----------------------------------|-------------------------------
std::vector<FxNode*> nodes  |    nodes heap allocated           |    nodes heap allocated                                      
                            |    contiguous                     |    contiguous
                            |    O(1) add                       |    O(1) add
                            |    O(n) remove                    |    O(n) remove
                            |    O(n) traversal                 |    O(n) traversal


*/


#endif 