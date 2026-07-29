#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace ce {
    Shader::~Shader() {
        for (size_t i = 0; i < this->shaderModules.size(); i++) {
            vkDestroyShaderModule(device, shaderModules[i], nullptr);
        }
    }

    void Shader::addCode(VkShaderStageFlagBits stage, const std::vector<char>& code) {

        VkShaderModule Shader = {};
        size_t pos = shaderModules.size();

        shaderModules.push_back(Shader);

        const VkShaderModuleCreateInfo shaderModuleCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = code.size(),                                // size of code
            .pCode = reinterpret_cast<const uint32_t*>(code.data()) // pointer to code(of uint32_t pointer type)
        };

        if (vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModules[pos]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a shader module");
        }

        const VkPipelineShaderStageCreateInfo shaderCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = stage,               // Shader stage name
            .module = shaderModules[pos], // Shader module to be used by stage
            .pName = "main",              // Entry point in to shader
        };

        shaderCreateInfos.push_back(shaderCreateInfo);
    }

    void Shader::addAtribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset) {
        //
        const VkVertexInputAttributeDescription attribute{
            .location = location, // Location in shader where data will be read from
            .binding = binding,   // Which binding the data is at (should be sdame as above)
            .format = format,     // Forma the data will take (also helps define size of data)
            .offset = offset,     // Where this attribute is defined in the data for a single vertex
        };

        attributeDescriptions.push_back(attribute);
    }

    void Shader::addBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate) {

        const VkVertexInputBindingDescription bindingDescription{
            .binding = binding,    // Cam bind multiple streams of data, thos defines which one
            .stride = stride,      // Size of a single vertex object
            .inputRate = inputRate // How to move between data after each vertex
                                   // VK_VERTEX_INPUT_RATE_INDEX : Move on to the next vertex
                                   // VK_VERTEX_INPUT_RATR_INSTANCE: Move to a vertex for the next instance
        };

        bindingDescriptions.push_back(bindingDescription);
    }

    void Shader::setVertexInput(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable) {
        //
        // -- VERTEX INPUT --
        vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputCreateInfo.pVertexBindingDescriptions =
            bindingDescriptions.data(); // List of vertex bind Descritions
        ;                               // (data spacing stride information)
        vertexInputCreateInfo.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(this->attributeDescriptions.size());
        vertexInputCreateInfo.pVertexAttributeDescriptions =
            this->attributeDescriptions.data(); // Listof Vertex Attribute Description
        ;                                       //  (data format and where
        ;                                       // to bind to/from)

        //
        // -- INPUT ASSEMBLY --
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = topology; // Primitive type to assemple vertice as
        inputAssembly.primitiveRestartEnable =
            primitiveRestartEnable; // Allow overiding of "strip" topology to start new primitive
    }

    namespace aux {

        std::vector<char> readFile(const std::filesystem::path& filename) {

            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open a file!");
            }

            auto filesize = static_cast<size_t>(file.tellg());
            std::vector<char> fileBuffer(filesize);

            file.seekg(0);
            file.read(fileBuffer.data(), filesize);
            file.close();

            return fileBuffer;
        }
    } // namespace aux
} // namespace ce
