# ChimeraEngine

Just another simple game engine using SDL3, Vulkan/OpenGL<p>
<b>Changing to Vulkan API</b>
OBS: Code with comments in Portuguese mostly

## Features of project:
- Editor: VSCodium (clangd, Clang-Format, CMake Tools, CodeLLDB)
- Build tool: CMAKE
- Compiler: Clang (C++20)
- Debug: lldb-dap
- Ident: clang-format (file: .clang-format)
- LIBS an API's:
  - Vulkan 1.1
  - OpenGL 4
  - SDL3, SDL3_Image, SDL3_TTF

## Fedora 42 Dependencies:
```bash
# Develop base
sudo dnf group upgrade core
sudo dnf group install c-development
sudo dnf group install development-tools
sudo dnf group install sound-and-video

# Develop compiler and tools like clang, llvm, lldb, CMAKE
sudo dnf install clang clang-tools-extra
sudo dnf install cmake cmake-data cmake-rpm-macros libstdc++-static llvm-static llvm-devel llvm-test autoconf automake
sudo dnf install lld lldb lldb-devel lld-devel lld-libs.x86_64
sudo dnf install compiler-rt

# Libs and other tools
sudo dnf install htop git gitk meld gcc g++ python3-pip vim tree curl openssh-server tinyxml2 tinyxml2-devel libyaml libyaml-devel yaml-cpp yaml-cpp-devel pugixml-devel jsoncpp

# Lib vulkan
sudo dnf install vulkan-loader-devel vulkan-tools vulkan-utility-libraries-devel glslang glslc VulkanMemoryAllocator-devel vulkan-validation-layers libshaderc-devel

# OpenGL
sudo dnf install mesa-dri-drivers mesa-libGL freeglut-devel glm-devel glew glew-devel libGLEW
sudo dnf install bullet bullet-devel bullet-extras bullet-extras-devel glfw-devel

# SDL3
sudo dnf install SDL3-static SDL3-devel SDL3_image-devel SDL3_ttf SDL3_ttf-devel

# ASSIMP (Model load)
sudo dnf install assimp assimp-devel

# fastgltf (for now)
sudo dnf install simdjson
sudo clone https://github.com/spnda/fastgltf

# HDM
sudo dnf install monado
```

## Switch Link and toolchain to Clang
Toolchain: [clang.cmake](./toolchain/clang.cmake)<p>
Change to ld.lld
```bash
sudo update-alternatives --config ld
#change to -> 2           /usr/bin/ld.lld
```

## Build engine and examples and test:
```bash
cd ChimeraEngine

cmake -G "Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
      -DCMAKE_TOOLCHAIN_FILE=./toolchain/clang.cmake \
      -B build

make -C build -j 4

# wayland still has erros, force X11
SDL_VIDEODRIVER=x11 ./bin/terrain
```

# compile spirv
```bash
glslangValidator  -V ./assets/shaders/shader.vert -o ./bin/vert.spv
glslangValidator  -V ./assets/shaders/shader.frag -o ./bin/frag.spv

glslangValidator  -V ./samples/sdl31/shades/TexturedQuad.vert -o ./bin/TexturedQuad.vert.spv
glslangValidator  -V ./samples/sdl31/shades/TexturedQuad.frag -o ./bin/TexturedQuad.frag.spv

glslangValidator  -V ./samples/sdl31/shades/ -o ./bin/
glslangValidator  -V ./samples/sdl31/shades/ -o ./bin/T
```


## Refs
<b>SDL3</b>
- [Main SDL3](https://wiki.libsdl.org/SDL3/FrontPage)
- [Migration SDL3](https://wiki.libsdl.org/SDL3/README-migration)

<b>OpenGL</b>
- [Collada](https://www.khronos.org/collada/)
- [learnopengl​](https://learnopengl.com/Introduction)
- [khrono](https://www.khronos.org/opengl/wiki/Example_Code)
- [gamedev](https://www.gamedev.com/)

<b>YAML</b> (future)
 - [jbeder](https://github.com/jbeder/yaml-cpp/wiki/How-To-Emit-YAML)

em vulkan api como criar multiplas texturas usando indice apenas no fragment

Bindless Textures



gerar abaixo na sequencia

em SDL3 e vulkan api nativa como criar Bindless Textures mostrando os set dos dectriptors e samples usando indice apenas no fragment passando como indice e model no push e view e projection biond com rotina desenho

em SDL3 e vulkan api nativa como criar Bindless Textures mostrando os set dos dectriptors e samples usando indice apenas no fragment passando como indice em push e a rotina desenho

em SDL3 e vulkan api nativa como criar Bindless Textures com diversara images de texturas mostrando os set dos dectriptors e samples usando indice apenas no fragment passando como indice em push

em SDL3 e vulkan api nativa como criar Bindless Textures mostrando a criacao do descriptorset unico e como passar para o fragment o indice

em Vulkan API criar Class com FrameData em double buffer, contendo VkCommandPool, VkCommandBuffer, VkSemaphores e VkFence, em pathner RAII para a criacao de destruicao dos handles do vulkan, criar class SwapchainData com VkSwapchainKHR, e um struct separado para VkImage, VkImageView, VkFence que sera um vector para cada image, no pathner RAII,implementar em c++20 codifique rotina des de criacao, destruicao e detalhe rotina de desenho con enfase na sincronizacao e no trato dos indices de imagens do swapchain e de current frame.


Struct de SwapchainData com VkSwapchainKHR, VkFormat, VkExtent2D, vetores de VkImage, vetores de VkImageView, vetores de VkFence,
codigique em c++20 com criacao do swapchain, e rodina de desenho detelhada




/-----
// --- ETAPA A: DEFINIR O LAYOUT BINDLESS ---
VkDescriptorSetLayoutBinding bindlessBinding{};
bindlessBinding.binding         = 0;
bindlessBinding.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
bindlessBinding.descriptorCount = 10000; // Tamanho máximo do array (capacidade total de texturas)
bindlessBinding.stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;

// Flags críticas para o comportamento Bindless
VkDescriptorBindingFlags bindingFlags =
    VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT | // Permite atualizar o set após vinculá-lo na GPU
    VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;    // Permite ter índices vazios (sem textura alocada)

VkDescriptorBindingFlagsCreateInfo extendedInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO};
extendedInfo.bindingCount = 1;
extendedInfo.pBindingFlags = &bindingFlags;

VkDescriptorSetLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
layoutInfo.pNext        = &extendedInfo;
layoutInfo.flags        = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT; // Obrigatório
layoutInfo.bindingCount = 1;
layoutInfo.pBindings    = &bindlessBinding;

VkDescriptorSetLayout bindlessLayout;
vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &bindlessLayout);


// --- ETAPA B: CRIAR O DESCRIPTOR POOL ---
VkDescriptorPoolSize poolSize{};
poolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
poolSize.descriptorCount = 10000;

VkDescriptorPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
poolInfo.flags        = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT; // Ativa suporte a bindless no pool
poolInfo.maxSets      = 1; // Precisamos de apenas 1 set único global
poolInfo.poolSizeCount = 1;
poolInfo.pPoolSizes   = &poolSize;

VkDescriptorPool descriptorPool;
vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);


// --- ETAPA C: ALOCAR O DESCRIPTOR SET ÚNICO ---
uint32_t maxTextures = 10000;
VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO};
variableCountInfo.descriptorSetCount = 1;
variableCountInfo.pDescriptorCounts  = &maxTextures;

VkDescriptorSetAllocateInfo allocInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
allocInfo.pNext              = &variableCountInfo;
allocInfo.descriptorPool     = descriptorPool;
allocInfo.descriptorSetCount = 1;
allocInfo.pSetLayouts        = &bindlessLayout;

VkDescriptorSet globalBindlessDescriptorSet;
vkAllocateDescriptorSets(device, &allocInfo, &globalBindlessDescriptorSet);


//---
// Estrutura de dados que será enviada diretamente para a GPU
struct PushConstantData {
    uint32_t textureIndex;
};

// --- DENTRO DO LOOP DE RENDERIZAÇÃO ---

// 1. Vincula o Descriptor Set ÚNICO uma vez no início do frame/passo de render
vkCmdBindDescriptorSets(
    cmdBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    pipelineLayout,
    0,                            // Primeiro Set (set = 0)
    1,                            // Quantidade de sets
    &globalBindlessDescriptorSet, // O set único global criado acima
    0, nullptr
);

// 2. Renderizar Objeto A com a textura que está salva no índice 0
PushConstantData pushA{ 0 };
vkCmdPushConstants(
    cmdBuffer,
    pipelineLayout,
    VK_SHADER_STAGE_FRAGMENT_BIT, // Alvo: Fragment Shader
    0,                            // Offset
    sizeof(PushConstantData),     // Tamanho dos dados
    &pushA                        // Ponteiro para os dados
);
vkCmdDrawIndexed(cmdBuffer, indexCountA, 1, 0, 0, 0);

// 3. Renderizar Objeto B mudando APENAS o índice (textura no índice 1)
PushConstantData pushB{ 1 };
vkCmdPushConstants(
    cmdBuffer,
    pipelineLayout,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    0,
    sizeof(PushConstantData),
    &pushB
);
vkCmdDrawIndexed(cmdBuffer, indexCountB, 1, 0, 0, 0);

//--

#version 450
#extension GL_EXT_nonuniform_qualifier : require // Necessário para indexação dinâmica em arrays abertos

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

// Recebe o índice enviado por vkCmdPushConstants
layout(push_constant) uniform PushConstants {
    uint textureIndex;
} push;

// Representa o Descriptor Set Único criado no código C++ (Set 0, Binding 0)
layout(set = 0, binding = 0) uniform sampler2D globalTextures[];

void main() {
    // nonuniformEXT garante que a amostragem seja correta mesmo se pixels vizinhos
    // executarem em paralelo acessando índices de texturas diferentes
    uint index = nonuniformEXT(push.textureIndex);

    // Amostra a textura diretamente do array usando o índice mapeado
    outColor = texture(globalTextures[index], inUV);
}


//--

// Estrutura que bate com o push_constant do Fragment Shader
struct MeshPushConstants {
    uint32_t textureIndex;
};

// --- DENTRO DO SEU LOOP DE RENDER (FRAME) ---

// 1. Vincula o pipeline de renderização
vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

// 2. Vincula o Descriptor Set BINDLESS global (Apenas 1 chamada de bind por frame para todas as texturas!)
vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &globalBindlessDescriptorSet, 0, nullptr);

// 3. DESENHAR OBJETO 1 (Ex: Parede de Tijolos - Textura no Índice 0)
MeshPushConstants pushObj1{ 0 };
vkCmdPushConstants(cmdBuffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(MeshPushConstants), &pushObj1);
vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &mesh1VertexBuffer, offsets);
vkCmdDraw(cmdBuffer, vertexCount1, 1, 0, 0);

// 4. DESENHAR OBJETO 2 (Ex: Personagem - Textura no Índice 1)
MeshPushConstants pushObj2{ 1 };
vkCmdPushConstants(cmdBuffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(MeshPushConstants), &pushObj2);
vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &mesh2VertexBuffer, offsets);
vkCmdDraw(cmdBuffer, vertexCount2, 1, 0, 0);
