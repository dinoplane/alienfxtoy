#ifndef FX_TASK_H
#define FX_TASK_H
#include <vector>
#include <string>
#include <shader_c.hpp>

struct Texture {
    GLuint id;
    GLuint width;
    GLuint height;
    GLuint channels;
    bool isValid;
};



class FxTask {
    public:
    
    enum FxTaskType {
        Empty,
        Load,
        Compute,
        MAX_TYPES = Compute
    };

    static const char* FxTaskNames[];
    static Texture LoadImage(std::string texturePath);
    static FxTask* CreateTask(FxTaskType type);

    FxTaskType type;
    FxTask(FxTaskType type) : type(type) {}
    // virtual void RunTask();
};

class FxLoadTask : public FxTask {
    
    public:
    Texture loadedTexture;
    bool isTextureLoaded;
    std::string texturePath;
    
    FxLoadTask() : FxTask(FxTaskType::Load), texturePath(""), isTextureLoaded(false), loadedTexture({0, 0, 0, 0, false}) {}
    ~FxLoadTask();
    
    std::string GetTexturePath() { return texturePath; }
    void SetTexturePath(std::string newPath ) { texturePath = newPath; }
    bool LoadTexture();
    void ClearTexture();
};

    /*
    public:
    std::string name;
    bool isInputTask;
    ComputeShader* shader;
    std::vector<Texture> inputs; // these are for textures loaded for input
    std::vector<Texture> textures; // these are for textures used by the compute shader
    // There is room for other inputs other than textures
    // Multiple textures can be used as inputs
    // Maybe inputs can be combined into... a flat piece of data with metadata indicating where each thing goes
    public:
    FxTask(std::string name, std::string computeShaderPath, bool isInputTask=false);
    FxTask(std::string name, std::string computeShaderPath, std::vector<std::string> texturePaths);
    
    FxTask(const FxTask& other) = delete;
    FxTask& operator=(const FxTask& other) = delete;

    FxTask(FxTask&& other) noexcept
        : name(other.name), shader(other.shader), inputs(other.inputs), textures(other.textures)
    {
        other.shader = nullptr;
        other.inputs.clear();
        other.textures.clear();
    }

    FxTask& operator=(FxTask&& other) noexcept
    {
        if (this != &other)
        {
            name = other.name;
            shader = other.shader;
            inputs = other.inputs;
            textures = other.textures;
            other.shader = nullptr;
            other.inputs.clear();
            other.textures.clear();
        }
        return *this;
    }


    ~FxTask();
    virtual void run(GLuint inTexture, GLuint outTexture);
    static Texture loadImage(std::string texturePath); // this function could be used internally (for compute shader inputs) or externally (for loading input textures)
};



class FxComputeTask : public FxTask {
    public:
    FxComputeTask(std::string name, std::string computeShaderPath);
    FxComputeTask(std::string name, std::string computeShaderPath, std::vector<std::string> texturePaths);
};
*/
#endif