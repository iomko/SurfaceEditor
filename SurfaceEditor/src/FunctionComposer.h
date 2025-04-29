#pragma once
#include "Patterns/Command.h"
#include "../Callables/Callable.h"



template <typename T, typename = void>
struct has_input_params : std::false_type {};

template <typename T>
struct has_input_params<T, std::void_t<typename T::InputType>>
	: std::conditional_t<std::is_void<typename T::InputType>::value, std::false_type, std::true_type> {};


class FunctionNode {
public:
	CallableBase* m_function = nullptr;
	std::vector<FunctionNode*> m_functionChildNodes;
};

class FunctionComposer
{
public:
	template<typename CallableType>
	FunctionNode* initRoot() {
		m_rootNode = new FunctionNode();
		m_rootNode->m_function = new CallableType();

		INPUT_PARAMS_FLAG = has_input_params<CallableType>::value;
		return m_rootNode;
	}

	template<typename CallableType>
	FunctionNode* addFunc(FunctionNode* functionNode) {
		functionNode->m_functionChildNodes.push_back(new FunctionNode());
		FunctionNode* newFunctionNode = functionNode->m_functionChildNodes.back();
		newFunctionNode->m_function = new CallableType();

		return newFunctionNode;
	}

	void execute()
	{
		if (INPUT_PARAMS_FLAG)
		{
			std::cerr << "Error: This callable requires input parameters." << std::endl;
		} else
		{
			Params defaultParams{};
			executeImpl(defaultParams);
		}
	}

	void execute(const Params& inputParams)
	{
		if (!INPUT_PARAMS_FLAG)
		{
			std::cerr << "Error: This callable doesn't require input parameters." << std::endl;
		} else
		{
			executeImpl(inputParams);
		}
	}

	~FunctionComposer() {
		delete m_rootNode;
	}

	FunctionNode* m_rootNode = nullptr;

private:
	bool INPUT_PARAMS_FLAG = false;

	void executeImpl(const Params& initialParams)
	{
		std::queue<std::pair<FunctionNode*, Params*>> queue;

		if (m_rootNode && m_rootNode->m_function)
		{
			Params* outputParams = m_rootNode->m_function->execute(initialParams);
			if (!m_rootNode->m_functionChildNodes.empty())
			{
				queue.push({ m_rootNode, outputParams });
			}
		}

		while (!queue.empty())
		{
			int numOfNodes = queue.size();
			while (numOfNodes != 0)
			{
				std::pair<FunctionNode*, Params*> nodeParamsPair = queue.front();
				queue.pop();
				Params* inputParams = nodeParamsPair.second;
				FunctionNode* node = nodeParamsPair.first;

				for (auto it = node->m_functionChildNodes.begin(); it != node->m_functionChildNodes.end(); ++it) {
					Params* outputParams = (*it)->m_function->execute(*inputParams);
					if (!(*it)->m_functionChildNodes.empty())
					{
						queue.push({ *it, outputParams });
					}
				}

				delete inputParams;
				--numOfNodes;
			}
		}
	}
};


/*
class FunctionComposer
{
public:

	template<typename CallableType>
	FunctionNode* initRoot() {
		m_rootNode = new FunctionNode();
		m_rootNode->m_function = new CallableType();
		return m_rootNode;
	}

	template<typename CallableType>
	FunctionNode* addFunc(FunctionNode* functionNode) {
		functionNode->m_functionChildNodes.push_back(new FunctionNode());
		FunctionNode* newFunctionNode = functionNode->m_functionChildNodes.back();
		newFunctionNode->m_function = new CallableType();
		return newFunctionNode;
	}

	void execute()
	{
		Params defaultParams{};
		execute(defaultParams);
	}

	void execute(const Params& initialParams)
	{
		std::queue<std::pair<FunctionNode*, Params*>> queue;

		if (m_rootNode && m_rootNode->m_function)
		{
			Params* outputParams = m_rootNode->m_function->execute(initialParams);
			if(!m_rootNode->m_functionChildNodes.empty())
			{
				queue.push({ m_rootNode, outputParams });
			}
		}

		while(!queue.empty())
		{
			int numOfNodes = queue.size();
			while (numOfNodes != 0)
			{
				std::pair<FunctionNode*, Params*> nodeParamsPair = queue.front();
				queue.pop();
				Params* inputParams = nodeParamsPair.second;
				FunctionNode* node = nodeParamsPair.first;

				for (auto it = node->m_functionChildNodes.begin(); it != node->m_functionChildNodes.end(); ++it) {
					Params* outputParams = (*it)->m_function->execute(*inputParams);
					if(!(*it)->m_functionChildNodes.empty())
					{
						queue.push({ *it, outputParams });
					}
				}

				delete inputParams;

				--numOfNodes;
			}
		}
	}

	~FunctionComposer() {
		delete m_rootNode;
	}

	FunctionNode* m_rootNode = nullptr;
};
*/