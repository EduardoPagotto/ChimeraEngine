#pragma once

#include <filesystem>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    class Shader {
      public:
        explicit Shader(VkDevice device) : device_(device) {}
        virtual ~Shader();

        void add_code(VkShaderStageFlagBits stage, const std::vector<char>& code);
        void add_atribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);
        void add_binding_description(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate);
        void set_vertex_input(VkPrimitiveTopology topology, VkBool32 primitive_restart_enable);

        std::vector<VkPipelineShaderStageCreateInfo>& get_shader_create_infos() { return this->shader_create_infos_; }
        VkPipelineVertexInputStateCreateInfo* getp_vertex_input_create_info() { return &vertex_input_create_info_; }
        VkPipelineInputAssemblyStateCreateInfo* getp_input_assembly() { return &input_assembly_; }

      private:
        VkDevice device_;
        std::vector<VkShaderModule> shader_modules_;
        std::vector<VkPipelineShaderStageCreateInfo> shader_create_infos_;
        std::vector<VkVertexInputAttributeDescription> attribute_descriptions_;
        std::vector<VkVertexInputBindingDescription> binding_descriptions_;

        VkPipelineVertexInputStateCreateInfo vertex_input_create_info_ = {};
        VkPipelineInputAssemblyStateCreateInfo input_assembly_ = {};
    };

    namespace aux {
        std::vector<char> read_file(const std::filesystem::path& filename);
    } // namespace aux

} // namespace ce
