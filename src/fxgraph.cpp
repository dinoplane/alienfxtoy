#include <fxgraph.hpp>


Graph::Graph() { fmt::print("HIYA"); }

FxGraph::FxGraph(){
    
    // FxTask task1("default", "./assets/shader/default.glsl");
    tasks.push_back(new FxTask ("baka", "./assets/texture/baka.jpg", true) );
    tasks.push_back(new FxTask ("default", "./assets/shader/default.glsl") );
    // tasks.push_back(task1);
    

    // FxNode node0 = {0, true, "./assets/texture/baka.jpg",  {1}};
    // FxNode node1 = {1, false, "", {}};
    // // FxNode node2 = {0, false, "", { }};
    nodes.push_back(new FxNode {0, true, "./assets/texture/baka.jpg",  {1}});
    nodes.push_back(new FxNode {1, false, "", {}});
    // nodes.push_back(node0);
    // nodes.push_back(node1);
    // nodes.push_back(node2);
    // Create a default task
    // tasks.push_back(FxTask("default", "shaders/default.comp"));
    // nodes.push_back(FxNode(&tasks[0]));
}

FxGraph::~FxGraph(){

    for (auto& task : tasks){
        delete task;
    }

    for (auto& node : nodes){
        delete node;
    }

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