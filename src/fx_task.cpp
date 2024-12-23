#include <fx_task.hpp>
#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const char* FxTask::FxTaskNames[] {"Empty", "Load", "Compute"};

FxTask* FxTask::CreateTask(FxTaskType type){
    FxTask* ret;
    switch (type) {
        case Empty:
            ret = nullptr;
            break;
        case Load:
            ret = new FxLoadTask();
            break;
        case Compute:
            ret = nullptr;
            break;
    }
    return ret;
}

bool FxLoadTask::LoadTexture(){
    Texture newTexture = FxTask::LoadImage(texturePath);

    if (newTexture.isValid){
        if (isTextureLoaded){
            ClearTexture();
        }

        loadedTexture = newTexture;
        isTextureLoaded = true;
    } 
    return isTextureLoaded;
}




void FxLoadTask::ClearTexture(){
    isTextureLoaded = false;
    glDeleteTextures(1, &loadedTexture.id);
}

FxLoadTask::~FxLoadTask(){
    if (isTextureLoaded){
        ClearTexture();
    }
}

/*
#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

FxTask::FxTask(std::string name, std::string computeShaderPath, bool isInputTask){
    this->name = name;
    this->isInputTask = isInputTask;
    if (isInputTask){
        this->inputs.push_back(FxTask::loadImage(computeShaderPath));
        this->shader = nullptr;
    } else {
        // I was at odds as to what to use here
        // To process textures, there are 2 ways
        // 1. Use a compute shader (flexible but complicated, the shaders must be written in a certain way)
        // 2. Render a big fat triangle and a fragment shader (I pay in vertex pipeline overhead, but shaders are easier to write)
        // I chose the compute shader because it is more flexible
        this->shader = new ComputeShader(computeShaderPath.c_str());
    }
}

FxTask::FxTask(std::string name, std::string computeShaderPath, std::vector<std::string> texturePaths){
    this->name = name;
    this->isInputTask = false;
    this->shader = new ComputeShader(computeShaderPath.c_str());
    for (
        std::vector<std::string>::iterator it = texturePaths.begin();
        it != texturePaths.end()
        ; it++
    ){
        this->inputs.push_back(FxTask::loadImage(*it));
    }
}

FxTask::~FxTask(){
    fmt::print("Deleting task {}\n", this->name);
    if (this->shader != nullptr)
        delete this->shader;

    for (Texture texture : this->textures){
        glDeleteTextures(1, &texture.id);
    }

    for (Texture texture : this->inputs){
        glDeleteTextures(1, &texture.id);
    }
}

void FxTask::run(GLuint inTexture, GLuint outTexture){
    this->shader->use();
    this->shader->setInt("inTexture", 0);
    this->shader->setInt("outTexture", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, outTexture);
    glDispatchCompute(1, 1, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

*/


Texture FxTask::LoadImage(std::string texturePath){
    // load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 4);

    GLuint texture = 0;

    bool isValid = false;
    if (data)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
        isValid = true;
    }
    else
    {
        fmt::print("Failed to load texture\n");
    }
    return {texture, (GLuint)width, (GLuint)height, (GLuint)nrChannels, isValid};
}
