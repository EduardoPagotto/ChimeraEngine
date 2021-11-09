// #pragma once
// #include "chimera/core/Engine.hpp"
// #include "chimera/core/IStateMachine.hpp"
// #include "chimera/render/2d/layer/LayerStack.hpp"

// namespace Chimera {

// class ApplicationGL : public IStateMachine {
//   public:
//     ApplicationGL(Engine* engine) : engine(engine){};
//     virtual ~ApplicationGL();
//     virtual void onAttach() override;
//     virtual void onDeatach() override;
//     virtual void onUpdate() override;
//     virtual void onRender() override;
//     virtual bool onEvent(const SDL_Event& event) override;

//     void pushLayer(IStateMachine* layer);
//     void pushOverlay(IStateMachine* overlay);

//   protected:
//     Engine* engine;

//   private:
//     StateStack layerStack;
// };
// } // namespace Chimera