#include <fxgraph.hpp>


Graph::Graph() { fmt::print("HIYA"); }

FxGraph::FxGraph(){
    FxTask task0("baka", "./assets/texture/baka.jpg", true);
    // FxTask task1("default", "./assets/shader/default.glsl");
    tasks.push_back(task0);
    // tasks.push_back(task1);
    

    // FxNode node0 = {0, true, "./assets/texture/baka.jpg",  {1}};
    // FxNode node1 = {1, false, "", {2}};
    // // FxNode node2 = {0, false, "", { }};
    // nodes.push_back(node0);
    // nodes.push_back(node1);
    // nodes.push_back(node2);
    // Create a default task
    // tasks.push_back(FxTask("default", "shaders/default.comp"));
    // nodes.push_back(FxNode(&tasks[0]));
}

FxGraph::~FxGraph(){
    tasks.clear();
    nodes.clear();
    // for (auto& task : tasks){
    //     delete task;
    // }
}

void FxGraph::run(){
    // for (auto& node : nodes){
    //     node.task->run();
    // }
}