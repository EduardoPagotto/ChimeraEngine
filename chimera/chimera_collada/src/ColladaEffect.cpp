#include "chimera_collada/ColladaEffect.hpp"
#include "chimera_collada/ColladaImage.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_ecs/MaterialComponent.hpp"
#include "chimera_ecs/ShaderComponent.hpp"

namespace ce {

    static TexFilter set_filter(const std::string& s_param_val) {
        if (s_param_val == "NEAREST") {
            return TexFilter::NEAREST;
        }
        if (s_param_val == "LINEAR") {
            return TexFilter::LINEAR;
        }

        return TexFilter::NONE;
    }

    static TexWrap set_wrap(const std::string& s_param_val) {
        if (s_param_val == "WRAP") {
            return TexWrap::REPEAT;
        } else if (s_param_val == "MIRROR") {
            return TexWrap::MIRRORED;
        } else if (s_param_val == "CLAMP") {
            return TexWrap::CLAMP_TO_EDGE;
        } else if (s_param_val == "CLAMP2") {
            return TexWrap::CLAMP;
        } else if (s_param_val == "BORDER") {
            return TexWrap::CLAMP_TO_BORDER;
        }

        return TexWrap::NONE;
    }

    void ColladaEffect::set_shader(const std::string& ref_name, const pugi::xml_node& node) {
        pugi::xml_node tech = node.child("technique");

        if ((ref_name.size() > 0) && (std::string(tech.attribute("sid").value()) != ref_name))
            return;

        std::unordered_map<std::string, std::string> mapa_shader_files;
        std::unordered_map<GLenum, std::string> shade_data;

        for (pugi::xml_node item = tech.first_child(); item; item = item.next_sibling()) {

            std::string name = item.name();
            if (name == "include") {

                std::string url = std::string(item.attribute("url").value());
                const char* url_file = "file://";
                size_t url_file_len = strlen(url_file);
                std::size_t found = url.find(url_file, 0, url_file_len);
                if (found != std::string::npos) {
                    std::string file = url.substr(url_file_len, std::string::npos);
                    mapa_shader_files[std::string(item.attribute("sid").value())] = file;
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
                                shade_data[GL_VERTEX_SHADER] = mapa_shader_files[source];
                            } else if (stage == "FRAGMENTPROGRAM") {
                                shade_data[GL_FRAGMENT_SHADER] = mapa_shader_files[source];
                            }
                        }
                    }
                }
            }
        }

        if (shade_data.size() > 1) {
            auto assets = this->registry->ctx().get<std::shared_ptr<AssetManager>>();

            ShaderComponent& sc = entity_.add_component<ShaderComponent>(registry.get());
            sc.tag.name = ref_name;

            // FIXME: mudar a forma para fazer a carga real no attachment do scene
            sc.shader = assets->load_shader(ref_name, shade_data).handle();
        }
    }

    bool ColladaEffect::set_texture_param(const pugi::xml_node& n, TexParam& tp) {
        for (pugi::xml_node nt_para = n.first_child(); nt_para; nt_para = nt_para.next_sibling()) {
            std::string s_param = nt_para.name();
            std::string s_param_val = nt_para.text().as_string();
            if (s_param == "minfilter")
                tp.minFilter = set_filter(s_param_val);
            else if (s_param == "magfilter")
                tp.magFilter = set_filter(s_param_val);
            else if (s_param == "wrap_s")
                tp.wrap_s = set_wrap(s_param_val);
            else if (s_param == "wrap_t")
                tp.wrap_t = set_wrap(s_param_val);
            else if (s_param == "wrap_r")
                tp.wrap_r = set_wrap(s_param_val);
            else if (s_param == "instance_image") {

                std::string url = nt_para.attribute("url").value();
                ColladaImage ci(registry, colladaDom, url);
                ci.create(entity_, tp, ci.get_library("library_images"));
                return true;
            }
        }
        return false;
    }

    void ColladaEffect::set_material(const pugi::xml_node& node, TexParam& tp) {

        std::shared_ptr<Material> p_mat;
        if (entity_.has_component<MaterialComponent>(registry.get())) {
            MaterialComponent& mc = entity_.get_component<MaterialComponent>(registry.get());
            p_mat = mc.material;
        } else {
            return;
        }

        pugi::xml_node phong = node.child("phong");
        for (pugi::xml_node prop = phong.first_child(); prop; prop = prop.next_sibling()) {

            std::string p = prop.name();
            if (p == "emission") {

                pugi::xml_node first = prop.first_child();
                if (std::string(first.name()) == "color") {
                    p_mat->set_emission(textToVec4(first.text().as_string()));
                } else if (std::string(first.name()) == "texture") {
                    // TODO: implementar
                }

            } else if (p == "ambient") {
                pugi::xml_node first = prop.first_child();
                if (std::string(first.name()) == "color") {
                    p_mat->set_ambient(textToVec4(first.text().as_string()));
                } else if (std::string(first.name()) == "texture") {
                    // TODO: implementar
                }
            } else if (p == "diffuse") {
                pugi::xml_node first = prop.first_child();
                if (std::string(first.name()) == "color") {
                    p_mat->set_diffuse(textToVec4(first.text().as_string()));
                } else if (std::string(first.name()) == "texture") {

                    std::string tex_id = first.attribute("texture").value();
                    std::string id_tex = mapa_tex_[mapa2d_[tex_id]];

                    ColladaImage ci(registry, colladaDom, id_tex);
                    ci.create(entity_, tp, ci.get_library("library_images"));

                    auto assets = this->registry->ctx().get<std::shared_ptr<AssetManager>>();

                    p_mat->add_texture(SHADE_TEXTURE_DIFFUSE, assets->get_texture(id_tex).handle());
                    p_mat->set_diffuse(glm::vec4(1.0F, 1.0F, 1.0F, 1.0F)); // FIXME: Arquivo do blender nao tem!!
                }
            } else if (p == "specular") {
                pugi::xml_node first = prop.first_child();
                if (std::string(first.name()) == "color") {
                    p_mat->set_specular(textToVec4(first.text().as_string()));
                } else if (std::string(first.name()) == "texture") {
                    // TODO: implementar
                }
            } else if (p == "shininess") {
                // TODO: implementar

                pugi::xml_node first = prop.first_child();
                if (std::string(first.name()) == "float") {
                    float aa = first.text().as_float();
                    p_mat->set_shine(aa);
                }

            } else if (p == "index_of_refraction") {
                // TODO: implementar
            }
        }
    }

    void ColladaEffect::set_image_parms(const pugi::xml_node& node) {

        for (pugi::xml_node param = node.first_child(); param; param = param.next_sibling()) {

            TexParam tp;
            std::string s_prof = param.name();
            std::string sid = param.attribute("sid").value();
            if (s_prof == "newparam") {

                pugi::xml_node val1 = param.first_child();
                if (std::string s_val1 = val1.name(); s_val1 == "surface") {
                    std::string key_image = val1.child("init_from").text().as_string();
                    // loadImage(keyImage, tp);
                    mapa_tex_[sid] = key_image;

                } else if (s_val1 == "sampler2D") {
                    if (set_texture_param(val1, tp) == false) {
                        std::string key_map = val1.child("source").text().as_string();
                        mapa2d_[sid] = key_map;
                    }
                } else if (s_val1 == "samplerDEPTH") {
                    // nao e textura e FR
                    tp.format = TexFormat::DEPTH_COMPONENT;
                    tp.internalFormat = TexFormat::DEPTH_COMPONENT;
                    set_texture_param(val1, tp);
                }
            } else if (s_prof == "technique") {
                set_material(param, tp);
            }
        }
    }

    void ColladaEffect::create(const std::string& ref_name, Entity& entity, pugi::xml_node node) {
        this->entity_ = entity;
        for (pugi::xml_node n_prof = node.first_child(); n_prof; n_prof = n_prof.next_sibling()) {
            if (std::string name_prof = n_prof.name(); name_prof == "profile_GLSL") {
                set_shader(ref_name, n_prof);
            } else if (name_prof == "profile_COMMON") {
                set_image_parms(n_prof);
            } else if (name_prof == "extra") {
                if (const pugi::xml_node n_fx = getExtra(n_prof, "instance_effect"); n_fx != nullptr) {
                    std::string url = n_fx.attribute("url").value();
                    ColladaEffect cf(registry, colladaDom, url);
                    cf.create("", entity, cf.get_library("library_effects"));
                }
            }
        }
    }
} // namespace ce
