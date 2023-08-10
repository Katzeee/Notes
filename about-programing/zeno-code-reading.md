2023.08.09
---

Consider the situation under

```cmake
option(ZENO_MULTIPROCESS "Enable multiprocessing for ZENO" OFF)
option(ZENO_IPC_USE_TCP "Use TCP for inter-process communication" OFF)
```

## Whole Context

`GraphsManagment` controls the graphs' ui model. In `graphsmanagement.h` 

```cpp
class GraphsManagment {
    IGraphsModel* m_model; // storing a `GraphsModel *` variable which manages all subgraphmodels
};
```

`Session` controls the global status, and provides a function `Session::defNodeClass` called by every nodes to register their constructor functions to `nodeClasses`. Then all constructions of nodes will be queried from it.

```cpp
struct Session {
    std::map<std::string, std::unique_ptr<INodeClass>> nodeClasses;
};
```

## Create Node

## Run graph

```cpp
struct Graph : std::enable_shared_from_this<Graph> {
    Session *session = nullptr; // store the global session
    SubgraphNode *subgraphNode = nullptr;
    std::map<std::string, std::unique_ptr<INode>> nodes; // store all nodes in this graph
    std::unique_ptr<Context> ctx; // the local context of this graph
    std::unique_ptr<DirtyChecker> dirtyChecker; // track which nodes are dirty and need to be re-executed, because one node in a graph may be executed many times
}
```

Call order:

```cpp
void ZToolMenuButton::mouseReleaseEvent(QMouseEvent *e); // button pressed
void ZToolButton::clicked(); // emit this
void DockContent_Editor::initConnections() { // callback function
    connect(m_btnRun, &ZToolMenuButton::clicked, this, [=]() {...};
}
void ZenoMainWindow::onRunTriggered(bool applyLightAndCameraOnly, bool applyMaterialOnly);
void launchProgram(IGraphsModel* pModel, LAUNCH_PARAM param); // located in corelaunch.cpp, serielize the secen to a json string, then pass it to the next function
void launchProgramJSON(std::string progJson, LAUNCH_PARAM param); // start execution follow the progJson
ProgramRunData()(); // be called then starts in a new thread
start(); // create a graph that contains the real nodes
ZENO_API void Graph::loadGraph(const char *json); // construct the real graph
ZENO_API void Graph::applyNodesToExec(); // for every frame, call apply
ZENO_API void Graph::applyNodes(std::set<std::string> const &ids) // apply all nodes in ids, use ctx to store current context when applying
ZENO_API bool Graph::applyNode(std::string const &id); // dfs, with recording vistied nodes
ZENO_API void INode::doApply(); // base function for INode, controling the apply flow
ZENO_API virtual void preApply();
    ZENO_API bool INode::requireInput(std::string const &ds); // call for every inputBounds to get input data, call dirtycheck to record dirty status
        ZENO_API bool Graph::applyNode(std::string const &id); // recursive
    ZENO_API virtual void apply() = 0;
```
