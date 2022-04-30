#include "chimera/colladaDB/ColladaShader.hpp"
#include "chimera/core/visible/Shader.hpp"

namespace Chimera {

ColladaShader::~ColladaShader() {}

void ColladaShader::create(const std::string& refName, Entity& entity, pugi::xml_node node) {

    pugi::xml_node profileGLSL = node.child("profile_GLSL");
    if (profileGLSL != nullptr) {

        pugi::xml_node tech = profileGLSL.child("technique");

        if (std::string(tech.attribute("sid").value()) == refName) {

            std::unordered_map<std::string, std::string> mapaShaderFiles;
            std::unordered_map<GLenum, std::string> shadeData;

            for (pugi::xml_node item = tech.first_child(); item; item = item.next_sibling()) {

                std::string name = item.name();
                if (name == "include") {

                    std::string url = std::string(item.attribute("url").value());
                    const char* urlFile = "file://";
                    size_t urlFileLen = strlen(urlFile);
                    std::size_t found = url.find(urlFile, 0, urlFileLen);
                    if (found != std::string::npos) {
                        std::string file = url.substr(urlFileLen, std::string::npos);
                        mapaShaderFiles[std::string(item.attribute("sid").value())] = file;
                    } else {
                        throw std::string("Url de arquivo Shader invalido: %s", url.c_str());
                    }

                } else if (name == "pass") {
                    std::string sid = item.attribute("sid").value();

                    for (pugi::xml_node pass = item.first_child(); pass; pass = pass.next_sibling()) {
                        if (std::string(pass.name()) == "shader") {

                            std::string stage = pass.attribute("stage").value();
                            std::string source = pass.child("name").attribute("source").value();

                            if ((stage.size() > 0) && (source.size() > 0)) {
                                if (stage == "VERTEXPROGRAM") {
                                    shadeData[GL_VERTEX_SHADER] = mapaShaderFiles[source];
                                } else if (stage == "FRAGMENTPROGRAM") {
                                    shadeData[GL_FRAGMENT_SHADER] = mapaShaderFiles[source];
                                }
                            }
                        }
                    }
                }
            }

            if (shadeData.size() > 1) {
                Shader& shader = entity.addComponent<Shader>();
                ShaderManager::load(refName, shadeData, shader);
            }
        }
    }
}
} // namespace Chimera