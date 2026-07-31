#pragma once
#include "AssetScope.hpp"
#include "VulkanContext.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

// Estrutura de contexto Vulkan injetada

namespace ce {

    class AssetManager {
      private:
        VulkanContext& vkContext; // Injeção de Dependência por referência
        std::vector<std::unique_ptr<AssetScope>> scopeStack;

      public:
        explicit AssetManager(VulkanContext& context) : vkContext(context) {
            // Cria o escopo global (raiz) automaticamente
            pushScope();
        }

        void pushScope() { scopeStack.push_back(std::make_unique<AssetScope>()); }
        void popScope() {
            if (scopeStack.size() > 1)
                scopeStack.pop_back();
        }

        // Carrega um asset usando uma Factory baseada em templates
        template <typename T, typename... Args>
        std::shared_ptr<T> load(const std::string& name, Args&&... args) {
            // Verifica do escopo mais recente (topo) ao mais antigo (global)
            for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
                if (auto asset = (*it)->get<T>(name)) {
                    return asset;
                }
            }

            // Se não achar, fábrica cria o recurso injetando o contexto Vulkan
            auto newAsset = T::create(vkContext, std::forward<Args>(args)...);
            scopeStack.back()->insert<T>(name, newAsset);
            return newAsset;
        }
    };
} // namespace ce
