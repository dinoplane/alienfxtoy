#ifndef FX_TASK_H
#define FX_TASK_H
#include <vector>
#include <string>
#include <shader_c.hpp>

struct Texture {
    GLuint id;
    std::string path;
    GLuint width;
    GLuint height;
    GLuint channels;
};

class FxTask {
    std::string name;
    ComputeShader* shader;
    std::vector<Texture> inputs; // these are for textures loaded for input
    std::vector<Texture> textures; // these are for textures used by the compute shader
    // There is room for other inputs other than textures
    // Multiple textures can be used as inputs
    // Maybe inputs can be combined into... a flat piece of data with metadata indicating where each thing goes
    public:
    FxTask(std::string name, std::string computeShaderPath, bool isInputTask=false);
    FxTask(std::string name, std::string computeShaderPath, std::vector<std::string> texturePaths);
    
    ~FxTask();
    void run(GLuint inTexture, GLuint outTexture);
    static Texture loadImage(std::string texturePath); // this function could be used internally (for compute shader inputs) or externally (for loading input textures)
};

#endif