Core Vulkan Context Layout Tips
When compiling GLSL/SPIR-V shaders to bind and read these generated textures using SDL3's Vulkan backend, remember the native Vulkan descriptor set layouts assigned by SDL3:
Set 0: Read-Only storage textures/buffers.
Set 1: Read-Write storage textures/buffers.
Set 2: Uniform buffers.
If you would like, please let me know:
Are you using SDL_image or a custom parser like stb_image?
Do you need help writing the GLSL sampler shader code to bind this texture?
Are you setting up Mipmaps for this texture resource?
