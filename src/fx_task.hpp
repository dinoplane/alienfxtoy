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

struct FxTaskInput {
    GLuint outputTexture;
    GLuint inputTexture;
    int width;
    int height;
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
    bool isValidTask;
    FxTask(FxTaskType type) : type(type), isValidTask(false) {}
    
    virtual void RunTask(FxTaskInput input) = 0;
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
    virtual void RunTask(FxTaskInput input);
};

class FxComputeTask : public FxTask {
    
    public:
    ComputeShader* loadedShader;
    bool isShaderLoaded;
    std::string shaderPath;
    
    FxComputeTask() : FxTask(FxTaskType::Compute), shaderPath(""), isShaderLoaded(false), loadedShader(nullptr) {}
    ~FxComputeTask();
    
    std::string GetShaderPath() { return shaderPath; }
    void SetShaderPath(std::string newPath ) { shaderPath = newPath; }
    bool LoadShader();
    void ClearShader();
    virtual void RunTask(FxTaskInput input);
};

#endif