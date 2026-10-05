#include "chimera_collada/ColladaPhysicScene.hpp"
#include "chimera_base/aux/utils.hpp"
#include "chimera_core/bullet/PhysicsControl.hpp"
#include "chimera_core/bullet/Solid.hpp"
#include "chimera_ecs/Entity.hpp"
#include "chimera_ecs/MeshComponent.hpp"
#include "chimera_ecs/TransComponent.hpp"
#include <SDL3/SDL.h>

namespace ce {

    const pugi::xml_node ColladaPhysicScene::find_model(pugi::xml_node node, const std::string& body) {

        for (pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) {

            std::string val = n.name();
            std::string name = n.attribute("name").value();
            if (name == body)
                return n.child("technique_common");
        }
        throw std::string(body + " nao encontrado nos modelos fisicos");
    }

    void ColladaPhysicScene::load_all(pugi::xml_node node) {

        std::string id = node.attribute("id").value();
        std::string name = node.attribute("name").value();

        auto pc = std::make_shared<PhysicsControl>();
        registry->ctx().emplace<std::shared_ptr<PhysicsControl>>(pc);

        pugi::xml_node n_tec = node.child("technique_common");
        std::string s_grav = n_tec.child("gravity").text().as_string();

        [[maybe_unused]]
        float ts = n_tec.child("time_step").text().as_float();

        std::vector<float> l_array_f;
        text_to_float_array(s_grav, l_array_f);
        pc->set_gravity(btVector3(l_array_f[0], l_array_f[1], l_array_f[2]));
        // pc.stepSim(ts); FIXME: remover e ver se funciona!!!!!!

        pugi::xml_node n_instace = node.child("instance_physics_model");
        std::string val = n_instace.name();
        std::string url = n_instace.attribute("url").value();
        pugi::xml_node models = get_library_url("library_physics_models", url);

        for (pugi::xml_node n_rb = n_instace.first_child(); n_rb; n_rb = n_rb.next_sibling()) {

            std::string body = n_rb.attribute("body").value();
            std::string target = n_rb.attribute("target").value();

            pugi::xml_node n_tec = find_model(models, body);
            if (n_tec == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s não encontrado target: %s", body.c_str(),
                             target.c_str());
                continue;
            }

            target.erase(0, 1); // remove #
            auto view = registry.get()->view<TagInfo>();
            for (auto entity : view) {
                // Pega a chave (mesh)
                TagInfo& tag = view.get<TagInfo>(entity);
                if (tag.id == target) {
                    Entity ent2(entity);
                    TransComponent& tc = ent2.get_component<TransComponent>(registry.get());
                    [[maybe_unused]]
                    MeshComponent& mc = ent2.get_component<MeshComponent>(registry.get());
                    Solid* solid = new Solid(pc.get(), tc.trans->get_matrix(), ent2); // nova transformacao
                    delete tc.trans;                                                  // deleta objeto de transformacao
                    tc.trans = nullptr;                                               // limpa ponteiro
                    tc.solid = true;                                                  // muda tipos de dado
                    tc.trans = solid; // carrega novo objeto de transformacao

                    [[maybe_unused]]
                    bool dynamic = n_tec.child("dynamic").text().as_bool();
                    float mass = n_tec.child("mass").text().as_float();

                    solid->set_mass(mass);

                    // Material
                    std::string url = n_tec.child("instance_physics_material").attribute("url").value();

                    pugi::xml_node n_pm = get_library_url("library_physics_materials", url);
                    pugi::xml_node n_tc = n_pm.child("technique_common");

                    solid->set_restitution(n_tc.child("restitution").text().as_float());
                    solid->set_friction_dynamic(n_tc.child("dynamic_friction").text().as_float());
                    solid->set_friction_static(n_tc.child("static_friction").text().as_float());

                    // Shape
                    pugi::xml_node n_shape = n_tec.child("shape").first_child();

                    std::vector<float> array_float;
                    if (std::string s_shape = n_shape.name(); s_shape == "sphere") {

                        std::string rad = n_shape.child("radius").text().as_string();
                        text_to_float_array(rad, array_float);
                        solid->set_shape_sphere(array_float[0]);

                    } else if (s_shape == "plane") {

                        std::string rad = n_shape.child("equation").text().as_string();
                        text_to_float_array(rad, array_float);
                        solid->set_shape_plane(glm::vec3(array_float[0], array_float[1], array_float[2]),
                                               array_float[3]);

                    } else if (s_shape == "box") { // FIXME: ver no colada para usar o parametro correto

                        std::string s_box = n_shape.first_child().text().as_string();
                        text_to_float_array(s_box, array_float);
                        solid->set_shape_box(glm::vec3(array_float[0], array_float[1], array_float[2]));

                    } else if (s_shape == "cylinder") {

                        std::string s_ci = n_shape.first_child().text().as_string();
                        text_to_float_array(s_ci, array_float);
                        solid->set_shape_cilinder(glm::vec3(array_float[0], array_float[1], array_float[2]));

                    } else if (s_shape == "mesh") {

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
