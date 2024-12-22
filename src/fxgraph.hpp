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
    size_t taskIdx;  
    bool isInputNode;
    std::string texturePath; // this is a hack, we should have a better way to handle this
    // hmm actually maybe not... it makes it easier to traverse the graph
    // std::vector<size_t> inputs; // a lack of foresight here, we could consider types of input/output
    std::vector<size_t> outputs; 
};

class FxGraph {

    public:
    std::vector<FxTask*> tasks;
    std::vector<FxNode*> nodes;

    FxGraph();
    ~FxGraph();
    void run();
};

#endif 