#ifndef __CHIMERA_LOADER_LIBRARY_H
#define __CHIMERA_LOADER_LIBRARY_H

#include <string>
#include <tinyxml2.h>

namespace ChimeraLoaders {

class Library {

public:
    Library(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~Library();

private:
    tinyxml2::XMLDocument* doc;

protected:

    tinyxml2::XMLElement* findExtra(tinyxml2::XMLElement* _nNode);

    std::string url;
    tinyxml2::XMLElement* root;
};
}

#endif