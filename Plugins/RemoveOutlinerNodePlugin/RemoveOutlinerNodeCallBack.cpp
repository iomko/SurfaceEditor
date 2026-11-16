#include "RemoveOutlinerNodeCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"

void RemoveOutlinerNodeCallBack::execute(const RemoveOutlinerNodeParams& params)
{
    auto* state = dynamic_cast<OutlinerLayerState*>(params.state);
    if (state == nullptr || params.data == nullptr)
    {
        return;
    }

    if (containsNode(state->m_nodes, state->m_currentSelectedNode) &&
        isRemovedNode(state->m_nodes, params.data, state->m_currentSelectedNode))
    {
        state->m_currentSelectedNode = nullptr;
    }
    removeNode(state->m_nodes, params.data);
}

bool RemoveOutlinerNodeCallBack::containsNode(
    const std::vector<OutlinerNodeConcept*>& nodes,
    OutlinerNodeConcept* target)
{
    if (target == nullptr)
    {
        return false;
    }
    for (OutlinerNodeConcept* node : nodes)
    {
        if (node == target || containsNode(node->m_childs, target))
        {
            return true;
        }
    }
    return false;
}

bool RemoveOutlinerNodeCallBack::isRemovedNode(
    const std::vector<OutlinerNodeConcept*>& nodes,
    void* data,
    OutlinerNodeConcept* target)
{
    for (OutlinerNodeConcept* node : nodes)
    {
        if (node->getData() == data)
        {
            return node == target || containsNode(node->m_childs, target);
        }
        if (isRemovedNode(node->m_childs, data, target))
        {
            return true;
        }
    }
    return false;
}

bool RemoveOutlinerNodeCallBack::removeNode(std::vector<OutlinerNodeConcept*>& nodes, void* data)
{
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        OutlinerNodeConcept* node = *it;
        if (node->getData() == data)
        {
            nodes.erase(it);
            delete node;
            return true;
        }
        if (removeNode(node->m_childs, data))
        {
            return true;
        }
    }
    return false;
}

static AutoRegisterCallback<RemoveOutlinerNodeCallBack> regRemoveNode("REMOVE_OUTLINER_NODE_CALLBACK");
