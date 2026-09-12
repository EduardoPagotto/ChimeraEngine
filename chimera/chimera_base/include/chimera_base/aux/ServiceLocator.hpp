#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <type_traits>
#include <typeindex>

namespace ce {

    /// @brief Base class for all services.
    class IService {
      public:
        virtual ~IService() = default;
        /// @brief Virtual function to get the base type index of the service.
        /// Should be overridden in each service interface to return the typeid of the base interface.
        virtual std::type_index getTypeIndex() const = 0;
    };

    /// @brief Specific service base class.
    /// @note Inherit from this instead of IService directly for services with multiple implementations.
    /// @tparam T
    template <typename T>
    class ServiceBase : public T {
      public:
        /// Returns the type index of the base interface.
        std::type_index getTypeIndex() const override { return std::type_index(typeid(T)); }
    };

    class ServiceLocator {
      private:
        std::map<std::type_index, std::shared_ptr<IService>> services;
        mutable std::mutex mutex;

      public:
        /// @brief Registers a service with the service locator.
        /// @tparam T The service implementation type.
        /// @param service Shared pointer to the service instance.
        template <typename T>
        void registerService(std::shared_ptr<T> service) {
            static_assert(std::is_base_of<IService, T>::value, "T must inherit from IService");
            std::lock_guard<std::mutex> lock(mutex);
            std::type_index typeIndex = service->getTypeIndex(); // Use base class type index
            if (services.find(typeIndex) != services.end()) {    // TODO: mudar para algo msg erro
                throw std::runtime_error("Service already registered");
            }

            services[typeIndex] = service;
        }

        template <typename T>
        std::shared_ptr<T> getService() const {
            std::lock_guard<std::mutex> lock(mutex);
            auto typeIndex = std::type_index(typeid(T));
            auto it = services.find(typeIndex);
            if (it == services.end()) {
                throw std::runtime_error("Service not found");
            }
            return std::static_pointer_cast<T>(it->second);
        }

        template <typename T>
        std::shared_ptr<T> getServiceOrNull() const {
            std::lock_guard<std::mutex> lock(mutex);
            auto typeIndex = std::type_index(typeid(T));
            auto it = services.find(typeIndex);
            if (it == services.end()) {
                return std::shared_ptr<T>();
            }
            return std::static_pointer_cast<T>(it->second);
        }
    };

    /// @brief Global access to ServiceLocator
    inline ServiceLocator g_service_locator;

} // namespace ce
