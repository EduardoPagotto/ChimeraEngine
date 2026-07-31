#pragma once
#include "TypeIdGenerator.hpp"
#include <any>
#include <memory>
#include <string>
#include <unordered_map>

namespace ce {

    class AssetScope {
      private:
        // Chave: ID do Tipo + Nome do Asset. Valor: Ponteiro genérico deletado corretamente
        std::unordered_map<std::string, std::any> resources;

      public:
        AssetScope() = default;
        ~AssetScope() = default; // std::any garante a chamada do destrutor correto sem RTTI

        template <typename T>
        void insert(const std::string& name, std::shared_ptr<T> asset) {
            std::string key = std::to_string(TypeIdGenerator::Get<T>()) + "_" + name;
            resources[key] = std::move(asset);
        }

        template <typename T>
        std::shared_ptr<T> get(const std::string& name) {
            std::string key = std::to_string(TypeIdGenerator::Get<T>()) + "_" + name;
            auto it = resources.find(key);
            if (it != resources.end()) {
                return std::any_cast<std::shared_ptr<T>>(it->second);
            }
            return nullptr;
        }

        void Clear() { resources.clear(); }
    };
} // namespace ce
