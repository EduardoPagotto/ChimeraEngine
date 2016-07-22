vec3 rgb = vec3(1.0, 1.0, 1.0);
rgb = PhongShade(g_light, g_material, position_, normal_);
_frag_color = vec4(rgb, 1.0);

//_frag_color = texel * vec4(ambient + diffuse + specular, 1.0);


