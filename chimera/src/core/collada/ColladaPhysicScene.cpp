#include "chimera/core/collada/ColladaPhysicScene.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/bullet/PhysicsControl.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include <SDL2/SDL.h>

namespace ce {

const pugi::xml_node ColladaPhysicScene::findModel(pugi::xml_node node, const std::string& body) {

    for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) {

        std::string val = n.name();
        std::string name = n.attribute("name").value();
        if (name == body)
            return n.child("technique_common");
    }
    throw std::string(body + " nao encontrado nos modelos fisicos");
}

void ColladaPhysicScene::loadAll(pugi::xml_node node) {

    std::string id = node.attribute("id").value();
    std::string name = node.attribute("name").value();

    auto pc = std::make_shared<PhysicsControl>();
    g_service_locator.registerService(pc);

    pugi::xml_node nTec = node.child("technique_common");
    std::string sGrav = nTec.child("gravity").text().as_string();
    float ts = nTec.child("time_step").text().as_float();

    std::vector<float> l_arrayF;
    textToFloatArray(sGrav, l_arrayF);
    pc->setGravity(btVector3(l_arrayF[0], l_arrayF[1], l_arrayF[2]));
    // pc.stepSim(ts); FIXME: remover e ver se funciona!!!!!!

    pugi::xml_node nInstace = node.child("instance_physics_model");
    std::string val = nInstace.name();
    std::string url = nInstace.attribute("url").value();
    pugi::xml_node models = getLibraryUrl("library_physics_models", url);

    for (pugi::xml_node nRb = nInstace.first_child(); nRb; nRb = nRb.next_sibling()) {

        std::string body = nRb.attribute("body").value();
        std::string target = nRb.attribute("target").value();

        pugi::xml_node nTec = findModel(models, body);
        if (nTec == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s não encontrado target: %s", body.c_str(), target.c_str());
            continue;
        }

        target.erase(0, 1); // remove #
        auto view = g_registry.get().view<TagComponent>();
        for (auto entity : view) {
            // Pega a chave (mesh)
            TagComponent& tag = view.get<TagComponent>(entity);
            if (tag.id == target) {
                Entity ent2 = {entity, &g_registry};
                TransComponent& tc = ent2.getComponent<TransComponent>();
                MeshComponent& mc = ent2.getComponent<MeshComponent>();
                Solid* solid = new Solid(pc.get(), tc.trans->getMatrix(), ent2); // nova transformacao
                delete tc.trans;                                                 // deleta objeto de transformacao
                tc.trans = nullptr;                                              // limpa ponteiro
                tc.solid = true;                                                 // muda tipos de dado
                tc.trans = solid;                                                // carrega novo objeto de transformacao

                bool dynamic = nTec.child("dynamic").text().as_bool();
                float mass = nTec.child("mass").text().as_float();

                solid->setMass(mass);

                // Material
                std::string url = nTec.child("instance_physics_material").attribute("url").value();

                pugi::xml_node nPm = getLibraryUrl("library_physics_materials", url);
                pugi::xml_node nTc = nPm.child("technique_common");

                solid->setRestitution(nTc.child("restitution").text().as_float());
                solid->setFrictionDynamic(nTc.child("dynamic_friction").text().as_float());
                solid->setFrictionStatic(nTc.child("static_friction").text().as_float());

                // Shape
                pugi::xml_node nShape = nTec.child("shape").first_child();

                std::vector<float> arrayFloat;
                if (std::string sShape = nShape.name(); sShape == "sphere") {

                    std::string rad = nShape.child("radius").text().as_string();
                    textToFloatArray(rad, arrayFloat);
                    solid->setShapeSphere(arrayFloat[0]);

                } else if (sShape == "plane") {

                    std::string rad = nShape.child("equation").text().as_string();
                    textToFloatArray(rad, arrayFloat);
                    solid->setShapePlane(glm::vec3(arrayFloat[0], arrayFloat[1], arrayFloat[2]), arrayFloat[3]);

                } else if (sShape == "box") { // FIXME: ver no colada para usar o parametro correto

                    std::string sBox = nShape.first_child().text().as_string();
                    textToFloatArray(sBox, arrayFloat);
                    solid->setShapeBox(glm::vec3(arrayFloat[0], arrayFloat[1], arrayFloat[2]));

                } else if (sShape == "cylinder") {

                    std::string sCi = nShape.first_child().text().as_string();
                    textToFloatArray(sCi, arrayFloat);
                    solid->setShapeCilinder(glm::vec3(arrayFloat[0], arrayFloat[1], arrayFloat[2]));

                } else if (sShape == "mesh") {

                    // if (mc.mesh != nullptr) {
                    //     btTriangleIndexVertexArray* indexVertexArray =
                    //         new btTriangleIndexVertexArray(mc.mesh->iPoint.size(),        // indice tot
                    //                                        (uint32_t)&mc.mesh->iPoint[0], // indice prt
                    //                                        3 * sizeof(uint32_t),          // indice stride
                    //                                        mc.mesh->point.size(),         // point tot
                    //                                        (float*)&mc.mesh->point[0],    // point ptr
                    //                                        3 * sizeof(float));            // point stride

                    //     solid->setIndexVertexArray(indexVertexArray);
                }
                break;
            }
        }
    }
}
} // namespace ce