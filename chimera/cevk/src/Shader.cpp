#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace ce {
    Shader::~Shader() {
        for (size_t i = 0; i < shader_modules_.size(); i++) {
            vkDestroyShaderModule(device_, shader_modules_[i], nullptr);
        }
    }

    void Shader::add_code(VkShaderStageFlagBits stage, const std::vector<char>& code) {

        VkShaderModule shader = {};
        size_t pos = shader_modules_.size();

        shader_modules_.push_back(shader);

        const VkShaderModuleCreateInfo shader_module_create_info{
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = code.size(),                                // size of code
            .pCode = reinterpret_cast<const uint32_t*>(code.data()) // pointer to code(of uint32_t pointer type)
        };

        if (vkCreateShaderModule(device_, &shader_module_create_info, nullptr, &shader_modules_[pos]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a shader module");
        }

        const VkPipelineShaderStageCreateInfo shader_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = stage,                 // Shader stage name
            .module = shader_modules_[pos], // Shader module to be used by stage
            .pName = "main",                // Entry point in to shader
        };

        shader_create_infos_.push_back(shader_create_info);
    }

    void Shader::add_atribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset) {
        //
        const VkVertexInputAttributeDescription attribute{
            .location = location, // Location in shader where data will be read from
            .binding = binding,   // Which binding the data is at (should be sdame as above)
            .format = format,     // Forma the data will take (also helps define size of data)
            .offset = offset,     // Where this attribute is defined in the data for a single vertex
        };

        attribute_descriptions_.push_back(attribute);
    }

    void Shader::add_binding_description(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate) {

        const VkVertexInputBindingDescription binding_description{
            .binding = binding,     // Cam bind multiple streams of data, thos defines which one
            .stride = stride,       // Size of a single vertex object
            .inputRate = input_rate // How to move between data after each vertex
                                    // VK_VERTEX_INPUT_RATE_INDEX : Move on to the next vertex
                                    // VK_VERTEX_INPUT_RATR_INSTANCE: Move to a vertex for the next instance
        };

        binding_descriptions_.push_back(binding_description);
    }

    void Shader::set_vertex_input(VkPrimitiveTopology topology, VkBool32 primitive_restart_enable) {
        //
        // -- VERTEX INPUT --
        vertex_input_create_info_.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_create_info_.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions_.size());
        vertex_input_create_info_.pVertexBindingDescriptions =
            binding_descriptions_.data(); // List of vertex bind Descritions
        ;                                 // (data spacing stride information)
        vertex_input_create_info_.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(attribute_descriptions_.size());
        vertex_input_create_info_.pVertexAttributeDescriptions =
            attribute_descriptions_.data(); // Listof Vertex Attribute Description
        ;                                         //  (data format and where
        ;                                         // to bind to/from)

        //
        // -- INPUT ASSEMBLY --
        input_assembly_.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_.topology = topology; // Primitive type to assemple vertice as
        input_assembly_.primitiveRestartEnable =
            primitive_restart_enable; // Allow overiding of "strip" topology to start new primitive
    }

    namespace aux {

        std::vector<char> readFile(const std::filesystem::path& filename) {

            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open a file!");
            }

            auto filesize = static_cast<size_t>(file.tellg());
            std::vector<char> file_buffer(filesize);

            file.seekg(0);
            file.read(file_buffer.data(), static_cast<long>(filesize));
            file.close();

            return file_buffer;
        }
    } // namespace aux
} // namespace ce
