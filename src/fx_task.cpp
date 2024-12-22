#include <fx_task.hpp>

#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

FxTask::FxTask(std::string name, std::string computeShaderPath, bool isInputTask){
    this->name = name;
    this->shader = new ComputeShader(computeShaderPath.c_str());
}

FxTask::FxTask(std::string name, std::string computeShaderPath, std::vector<std::string> texturePaths){
    this->name = name;
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
    if (this->shader != nullptr)
        delete this->shader;

    for (GLuint texture : this->textures){
        glDeleteTextures(1, &texture);
    }

    for (GLuint texture : this->inputs){
        glDeleteTextures(1, &texture);
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

GLuint FxTask::loadImage(std::string texturePath){
    // load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    GLuint texture = 0;


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
    }
    else
    {
        fmt::print("Failed to load texture\n");
    }
    return texture;
}