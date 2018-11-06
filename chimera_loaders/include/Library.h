#ifndef __CHIMERA_LOADER_LIBRARY_H
#define __CHIMERA_LOADER_LIBRARY_H

#include <vector>
#include <string>
#include <tinyxml2.h>

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
    Library(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~Library();

private:
    tinyxml2::XMLDocument* doc;

protected:
    tinyxml2::XMLElement* findExtra(tinyxml2::XMLElement* _nNode);
    int findParams(tinyxml2::XMLElement* _nNode, VectorParam *_pVectorParam);

    std::string url;
    tinyxml2::XMLElement* root;
};
}

#endif