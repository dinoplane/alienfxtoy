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
            ret = new FxComputeTask();
            break;
    }
    return ret;
}

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

        glTextureStorage2D(texture, 1, GL_RGBA32F, width, height);
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
