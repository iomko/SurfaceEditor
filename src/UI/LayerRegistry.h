#pragma once
#include <any>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <tuple>
#include <utility>
#include <type_traits>
#include "../Core/Layer.h"
#include <stdexcept>
#include "VisibilityHandler.h"

class LayerRegistry
{
public:
    using Creator = std::function<std::unique_ptr<Layer>(const std::any &)>;

    static LayerRegistry &instance()
    {
        static LayerRegistry registry;
        return registry;
    }

    template <typename LayerT>
    void registerLayer(std::string id)
    {
        m_creators[id] = [](const std::any &) -> std::unique_ptr<Layer>
        {
            auto layer = std::make_unique<LayerT>();

            VisibilityHandler::init(LayerT::ID);

            return layer;
        };
    }

    template <typename LayerT, typename... CtorArgs>
    void registerLayerWithArgs(std::string id)
    {
        m_creators[id] = [](const std::any &a) -> std::unique_ptr<Layer>
        {
            // musí existovať tuple argumentov
            if (!a.has_value())
            {
                throw std::runtime_error("Layer requires constructor args.");
            }

            using TupleT = std::tuple<std::decay_t<CtorArgs>...>;
            const TupleT &tup = std::any_cast<const TupleT &>(a);
                
            auto layer =  std::apply(
                [&](auto &&...args)
                {
                    return std::make_unique<LayerT>(
                        std::forward<decltype(args)>(args)...);
                },
                tup);
                
            VisibilityHandler::init(static_cast<LayerIDS>(LayerT::ID));

            return layer;
        };
    }

    template <typename... Args>
    Layer *getLayer(std::string id, Args &&...args)
    {
        auto it = m_instances.find(id);
        if (it != m_instances.end())
        {
            return it->second.get();
        }

        auto itCreator = m_creators.find(id);
        if (itCreator == m_creators.end())
            return nullptr;

        std::any anyArgs;
        if constexpr (sizeof...(Args) > 0)
        {
            using TupleT = std::tuple<std::decay_t<Args>...>;
            anyArgs = std::make_any<TupleT>(std::forward<Args>(args)...);
        }

        auto instance = itCreator->second(anyArgs);
        Layer *rawPtr = instance.get();
        m_instances[id] = std::move(instance);
        return rawPtr;
    }

    void clear()
    {
        m_instances.clear();
    }

private:
    std::unordered_map<std::string, Creator> m_creators;
    std::unordered_map<std::string, std::unique_ptr<Layer>> m_instances;
};

template <typename LayerT>
struct AutoRegisterLayer
{
    AutoRegisterLayer(std::string id)
    {
        LayerRegistry::instance().registerLayer<LayerT>(id);
    }
};
template <typename LayerT, typename... CtorArgs>
struct AutoRegisterLayerArgs
{
    AutoRegisterLayerArgs(std::string id) { LayerRegistry::instance().registerLayerWithArgs<LayerT, CtorArgs...>(id); }
};