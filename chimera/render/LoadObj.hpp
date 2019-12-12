#ifndef __CHIMERA_LOADEROBJ__HPP
#define __CHIMERA_LOADEROBJ__HPP

#include "chimera/render/Material.hpp"
#include "chimera/render/MeshData.hpp"

namespace Chimera {

class LoaderObj {
  public:
    LoaderObj(const std::string& _fileName);
    virtual ~LoaderObj();
    inline bool hasMaterial() { return materialFile.size() > 0 ? true : false; }
    bool getMesh(MeshData& _mesh);
    bool getMaterial(Material& _pMaterial);

  private:
    void eraseAllSubStr(std::string& mainStr, const std::string& toErase);
    bool getValidData(std::string& nova, const std::string& comando);

    std::string fileName;
    std::string materialFile;
};
} // namespace Chimera

#endif