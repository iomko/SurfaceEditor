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
    void registerLayer(const std::string &name)
    {
        m_creators[name] = [name](const std::any &) -> std::unique_ptr<Layer>
        {
            return std::make_unique<LayerT>(name);
        };
    }

    template <typename LayerT, typename... CtorArgs>
    void registerLayerWithArgs(const std::string &name)
    {
        m_creators[name] = [name](const std::any &a) -> std::unique_ptr<Layer>
        {
            // musí existovať tuple argumentov
            if (!a.has_value())
            {
                throw std::runtime_error("Layer '" + name + "' requires constructor args.");
            }

            using TupleT = std::tuple<std::decay_t<CtorArgs>...>;
            const TupleT &tup = std::any_cast<const TupleT &>(a);

            return std::apply(
                [&](auto &&...args)
                {
                    return std::make_unique<LayerT>(
                        name, std::forward<decltype(args)>(args)...);
                },
                tup);
        };
    }

    template <typename... Args>
    Layer *getLayer(const std::string &name, Args &&...args)
    {
        auto it = m_instances.find(name);
        if (it != m_instances.end())
        {
            return it->second.get();
        }

        auto itCreator = m_creators.find(name);
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
        m_instances[name] = std::move(instance);
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
    AutoRegisterLayer(const std::string &name)
    {
        LayerRegistry::instance().registerLayer<LayerT>(name);
    }
};
template <typename LayerT, typename... CtorArgs>
struct AutoRegisterLayerArgs
{
    AutoRegisterLayerArgs(const std::string &name) { LayerRegistry::instance().registerLayerWithArgs<LayerT, CtorArgs...>(name); }
};