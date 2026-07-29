#pragma once

#include <filesystem>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class Shader {
      public:
        explicit Shader(VkDevice device) : device(device) {}
        virtual ~Shader();

        void addCode(VkShaderStageFlagBits stage, const std::vector<char>& code);
        void addAtribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);
        void addBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);
        void setVertexInput(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);

        std::vector<VkPipelineShaderStageCreateInfo>& getShaderCreateInfos() { return this->shaderCreateInfos; }
        VkPipelineVertexInputStateCreateInfo* getpVertexInputCreateInfo() { return &vertexInputCreateInfo; }
        VkPipelineInputAssemblyStateCreateInfo* getpInputAssembly() { return &inputAssembly; }

      private:
        VkDevice device;
        std::vector<VkShaderModule> shaderModules;
        std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        std::vector<VkVertexInputBindingDescription> bindingDescriptions;

        VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    };

    namespace aux {
        std::vector<char> readFile(const std::filesystem::path& filename);
    } // namespace aux

} // namespace ce
