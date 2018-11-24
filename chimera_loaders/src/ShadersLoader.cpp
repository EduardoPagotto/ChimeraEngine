#include <fstream>

#include "ExceptionChimera.h"
#include "ShadersLoader.h"

namespace ChimeraLoaders {

ShadersLoader::ShadersLoader() {
    log = spdlog::get("chimera");
    log->debug("Contructor ShadersLoader");
}

Chimera::Shader* ShadersLoader::loadShader(const std::string& programName,
                                           const std::string& vertex_file_path,
                                           const std::string& fragment_file_path) {

    GLuint id = load(programName, vertex_file_path, fragment_file_path);
    Chimera::Shader* pShader = new Chimera::Shader(programName, id);
    return pShader;
}

GLuint ShadersLoader::load(const std::string& programName,
                           const std::string& vertex_file_path,
                           const std::string& fragment_file_path) {

    bool retorno = false;

    // Ler os arquivos
    std::string VertexShaderCode = getShaderCode(vertex_file_path.c_str());
    std::string FragmentShaderCode = getShaderCode(fragment_file_path.c_str());

    log->debug("Shader name:{}", programName);

    // Compila Vertex Shader
    log->debug("Compiling Vertex:{}", vertex_file_path);
    GLuint VertexShaderID = compileShader(VertexShaderCode, true);

    // Compila Fragment Shader
    log->debug("Compiling Fragment:{}", fragment_file_path);
    GLuint FragmentShaderID = compileShader(FragmentShaderCode, false);

    // Link o programa
    GLuint idProgram = linkShader(VertexShaderID, FragmentShaderID);
    log->debug("Linked: {0:03d}", idProgram);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    if (idProgram != -1)
        log->info("Shader OK:{0} Id:{1:03d}", programName, idProgram);
    else
        log->error("Shader Erro: {}", programName);

    return idProgram;
}

std::string ShadersLoader::getShaderCode(const char* file_path) {

    // Read the Vertex Shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(file_path, std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line = "";
        while (getline(shaderStream, Line)) {
            shaderCode += "\n" + Line;
        }

        shaderStream.close();

    } else {

        throw Chimera::ExceptionChimera(Chimera::ExceptionCode::OPEN,
                                        "Impossivel abrir arquivo: " +
                                            std::string(file_path));
    }

    return shaderCode;
}

GLuint ShadersLoader::compileShader(const std::string& shaderCode, bool _shadeKind) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint shaderID = -1;
    if (_shadeKind == false)
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    else
        shaderID = glCreateShader(GL_VERTEX_SHADER);

    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    // Check Fragment Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);

    if (Result == GL_FALSE) {
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> shaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
            log->error("Shader Check Fragment Shader: {}",
                       std::string(&shaderErrorMessage[0]));
        }
    }

    return shaderID;
}

GLuint ShadersLoader::linkShader(const GLuint& VertexShaderID,
                                 const GLuint& FragmentShaderID) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);

    if (Result == GL_FALSE) {

        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            log->error("Shader Check program: {}", std::string(&ProgramErrorMessage[0]));
        }
    }
    return ProgramID;
}
} // namespace ChimeraLoaders
