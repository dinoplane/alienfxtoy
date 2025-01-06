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

    size_t srcNodeIdx;
    size_t currNodeIdx;

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
    void PrintGraph();  
};


/*
Should Graph have std::vector<FxNode> or std::vector<FxNode*>?

User clicks add node: O(1)
User clicks remove node: O(n)
Traversal: O(n)

Adjacency list is good.
But...


                            |    std::vector<size_t> outputs                           |    std::vector<FxNode*> outputs;
----------------------------|----------------------------------------------------------|--------------------------------------------------------------------------
std::vector<FxNode>  nodes  |    nodes stack allocated                                 |    nodes stack allocated
                            |    AddNode O(1) just stack alloc                         |    AddNode O(1) but POINTERS GET INVALIDATED ON RESIZE
                            |    AddEdge nodes[src].out.push_back(dst)                 |    AddEdge nodes[src].out.push_back(&nodes[dst])
                            |    RemoveEdge nodes[src].out.erase(remove(dst))          |    RemoveEdge nodes[src].out.erase(remove(&nodes[dst]))
                            |    RemoveNode nodes.erase(d), fix all idcs in edges      |    
                            |    RemoveNode (Lazy) set to deleted, use when new add    |    NO THIS BAD
                            |        fix only edges of d, fixes idx offset iss         |  
                            |    O(n) traversal, indexing                              |    
                            |                                                          | 
----------------------------|----------------------------------------------------------|--------------------------------------------------------------------------
std::vector<FxNode*> nodes  |    nodes heap allocated                                  |    nodes heap allocated                                      
                            |    AddNode O(1) just dynamic alloc                       |    AddNode O(1) pointers point to heap allocated nodes
                            |    AddEdge nodes[src].out.push_back(dst)                 |    AddEdge nodes[src].out.push_back(&nodes[dst])
                            |    RemoveEdge nodes[src].out.erase(remove(dst))          |    RemoveEdge nodes[src].out.erase(remove(&nodes[dst]))
                            |    RemoveNode del d, nodes.erase(d), fix idcs in edges   |    RemoveNode del d, nodes.erase(d), fix idcs in edges
                            |    RemoveNode (Lazy) set to deleted, use when new add    |    RemoveNode (Lazy) set d to deleted, use when add
                            |        fix only edges of d, fixes idx offset iss         |        fix only edges connected to d, no real issues            
                            |    O(n) traversal, pointer chasing                       |    O(n) traversal, pointer chasing
                            |                                                          |    


*/


#endif 