#ifndef __CHIMERA_ILAYER2__HPP
#define __CHIMERA_ILAYER2__HPP

#include <SDL2/SDL.h>
#include <string>

namespace Chimera {

class ILayer {
  public:
    virtual void onAttach() = 0;
    virtual void onDeatach() = 0;
    virtual void onUpdate() = 0;
    virtual void onEvent(const SDL_Event& _event) = 0;
    virtual std::string getName() const = 0;
};

class Layer2 : public ILayer {
  public:
    Layer2(const std::string& name = "Layer-default") : name(name) {}
    virtual ~Layer2() {}
    virtual void onAttach() {}
    virtual void onDeatach() {}
    virtual void onUpdate() {}
    virtual void onEvent(const SDL_Event& _event) {}
    virtual std::string getName() const { return name; }

  private:
    std::string name;
};

} // namespace Chimera
#endif