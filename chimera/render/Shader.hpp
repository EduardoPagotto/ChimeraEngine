#pragma once
#include "chimera/render/OpenGLDefs.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Chimera {

class ShadeInclude {
  public:
    static std::string load(std::string path) {
        // Return the source code of the complete shader
        std::string includeIndentifier = "#include ";
        static bool isRecursiveCall = false;

        std::string fullSourceCode = "";
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "ERROR: could not open the shader at: " << path << "\n" << std::endl;
            return fullSourceCode;
        }

        std::string lineBuffer;
        while (std::getline(file, lineBuffer)) {
            // Look for the new shader include identifier
            if (lineBuffer.find(includeIndentifier) != lineBuffer.npos) {
                // Remove the include identifier, this will cause the path to remain
                lineBuffer.erase(0, includeIndentifier.size());

                // The include path is relative to the current shader file path
                std::string pathOfThisFile;
                getFilePath(path, pathOfThisFile);
                lineBuffer.insert(0, pathOfThisFile);

                // By using recursion, the new include file can be extracted
                // and inserted at this location in the shader source code
                isRecursiveCall = true;
                fullSourceCode += load(lineBuffer);

                // Do not add this line to the shader source code, as the include
                // path would generate a compilation issue in the final source code
                continue;
            }

            fullSourceCode += lineBuffer + '\n';
        }

        // Only add the null terminator at the end of the complete file,
        // essentially skipping recursive function calls this way
        if (!isRecursiveCall)
            fullSourceCode += '\0';

        file.close();

        return fullSourceCode;
    }

  private:
    static void getFilePath(const std::string& fullPath, std::string& pathWithoutFileName) {
        // Remove the file name and store the path to this folder
        size_t found = fullPath.find_last_of("/\\");
        pathWithoutFileName = fullPath.substr(0, found + 1);
    }
};

class Shader {
    friend class ShaderManager;

  public:
    Shader() = default;
    // Shader(const std::string& name, const GLuint& id) : name(name), shaderId(id) {}
    Shader(const Shader& other) : shaderId(other.shaderId), name(other.name) {}
    virtual ~Shader() = default;

    inline void enable() const { glUseProgram(this->shaderId); } // TODO: const GLuint apply() const {return this->shaderId} ;
    static void disable() { glUseProgram(0); }                   // TODO: remover metodo
    // TODO: criar inline invalidade() {}
    // inline const std::string getName() const { return name; }

    const GLint getUniform(const char* _varName) const noexcept;
    void setUniform(const char* name, float val) const { glUniform1f(getUniform(name), val); }
    void setUniform(const char* name, int val) const { glUniform1i(getUniform(name), val); }
    void setUniform(const char* name, const glm::vec2& vec) const { glUniform2f(getUniform(name), vec.x, vec.y); }
    void setUniform(const char* name, const glm::ivec2& vec) const { glUniform2i(getUniform(name), vec.x, vec.y); }
    void setUniform(const char* name, const glm::vec3& vec) const { glUniform3f(getUniform(name), vec.x, vec.y, vec.z); }
    void setUniform(const char* name, const glm::ivec3& vec) const { glUniform3i(getUniform(name), vec.x, vec.y, vec.z); }
    void setUniform(const char* name, const glm::vec4& vec) const { glUniform4f(getUniform(name), vec.x, vec.y, vec.z, vec.w); }
    void setUniform(const char* name, const glm::ivec4& vec) const { glUniform4i(getUniform(name), vec.x, vec.y, vec.z, vec.w); }
    void setUniform(const char* name, const glm::mat3& mat) const {
        glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setUniform(const char* name, const glm::mat4& mat) const {
        glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setUniformArray(const char* name, int size, float* val) const { glUniform1fv(getUniform(name), size, val); }
    void setUniformArray(const char* name, int size, int* val) const { glUniform1iv(getUniform(name), size, val); }
    void setUniformArray(const char* name, int size, glm::vec2* val) const { glUniform2fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec2* val) const { glUniform2iv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::vec3* val) const { glUniform3fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec3* val) const { glUniform3iv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::vec4* val) const { glUniform4fv(getUniform(name), size, glm::value_ptr(*val)); }
    void setUniformArray(const char* name, int size, glm::ivec4* val) const { glUniform4iv(getUniform(name), size, glm::value_ptr(*val)); }
    inline const bool isInvalid() const { return shaderId == 0; }
    bool operator==(const Shader& other) const { return shaderId == other.shaderId; }
    bool operator!=(const Shader& other) const { return !(*this == other); }

  private:
    GLuint shaderId = 0;
    std::string name = "invalid"; // TODO: remover!!!!!
};
//---
class UniformVal {
  public:
    UniformVal() : name("invalid"), typeVal(-1) {}
    UniformVal(const std::string& name, const int& value) : name(name), typeVal(0), val_int(value) {}
    UniformVal(const std::string& name, const float& value) : name(name), typeVal(1), val_float(value) {}
    UniformVal(const std::string& name, const glm::vec2& value) : name(name), typeVal(2), val_vec2(value) {}
    UniformVal(const std::string& name, const glm::ivec2& value) : name(name), typeVal(3), val_ivec2(value) {}
    UniformVal(const std::string& name, const glm::vec3& value) : name(name), typeVal(4), val_vec3(value) {}
    UniformVal(const std::string& name, const glm::ivec3& value) : name(name), typeVal(5), val_ivec3(value) {}
    UniformVal(const std::string& name, const glm::vec4& value) : name(name), typeVal(6), val_vec4(value) {}
    UniformVal(const std::string& name, const glm::ivec4& value) : name(name), typeVal(7), val_ivec4(value) {}
    UniformVal(const std::string& name, const glm::mat3& value) : name(name), typeVal(8), val_mat3(value) {}
    UniformVal(const std::string& name, const glm::mat4& value) : name(name), typeVal(9), val_mat4(value) {}

    UniformVal(const UniformVal& cpy) : name(cpy.name), typeVal(cpy.typeVal), val_mat4(cpy.val_mat4) {}

    void setUniform(const Shader& shader) const;

    const std::string getName() const { return this->name; }

  private:
    std::string name;
    int8_t typeVal;

    union {
        int val_int;
        float val_float;
        glm::vec2 val_vec2;
        glm::ivec2 val_ivec2;
        glm::vec3 val_vec3;
        glm::ivec3 val_ivec3;
        glm::vec4 val_vec4;
        glm::ivec4 val_ivec4;
        glm::mat3 val_mat3;
        glm::mat4 val_mat4;
    };
};

class UniformMapped {
    UniformMapped() = default;
    virtual ~UniformMapped() = default;
    void set(const UniformVal& value);
    void bindAll(const Shader& shader) const;
    void clear();

  private:
    std::unordered_map<std::string, UniformVal> uniformMap;
};

//---
class ShaderManager {
  public:
    static void load(const std::string& name, const std::unordered_map<GLenum, std::string>& mFiles, Shader& shader);
    static const Shader get(const std::string& name);
    static bool remove(Shader& shader);
    static void clear();

  private:
    static std::vector<Shader> shaders;
};
} // namespace Chimera
