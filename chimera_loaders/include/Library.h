#ifndef __CHIMERA_LOADER_LIBRARY__HPP
#define __CHIMERA_LOADER_LIBRARY__HPP

#include <string>
#include <tinyxml2.h>
#include <vector>

#include <glm/glm.hpp>

namespace ChimeraLoaders {

struct ParamCollada {
    std::string name;
    std::string sid;
    std::string type;
    std::string value;
};

typedef std::vector<ParamCollada> VectorParam;

class Library {

  public:
    Library(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~Library();

  private:
    tinyxml2::XMLDocument* doc;

  protected:
    static std::string getIdFromUrl(const std::string& _url);
    static tinyxml2::XMLElement* findExtra(tinyxml2::XMLElement* _nNode);

    static int findParams(tinyxml2::XMLElement* _nNode, VectorParam* _pVectorParam);

    static void loadArrayBtScalar(const char* _val, std::vector<float>& _arrayF);
    static void loadArrayI(const char* _val, std::vector<int>& _arrayI);

    static glm::mat4 loadTransformMatrix(const char* _matrix);
    static glm::mat4 carregaMatrix(const std::vector<float>& listaMatrix);

    std::string file_atual;
    std::string url;
    tinyxml2::XMLElement* root;
};
} // namespace ChimeraLoaders

#endif