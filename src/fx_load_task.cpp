#include <fx_task.hpp>
#include <fmt/core.h>


bool FxLoadTask::LoadTexture(){
    Texture newTexture = FxTask::LoadImage(texturePath);

    if (newTexture.isValid){
        if (isTextureLoaded){
            ClearTexture();
        }

        loadedTexture = newTexture;
        isTextureLoaded = true;
        isValidTask = true;
    } 
    return isTextureLoaded;
}

void FxLoadTask::ClearTexture(){
    if (isTextureLoaded){
        isTextureLoaded = false;
        texturePath = "";

        glDeleteTextures(1, &loadedTexture.id);
        loadedTexture.id = 0;
        loadedTexture.width = 0;
        loadedTexture.height = 0;
        loadedTexture.channels = 0;
        loadedTexture.isValid = false;
        isValidTask = false;
    }
}

FxLoadTask::~FxLoadTask(){
    if (isTextureLoaded){
        ClearTexture();
        isValidTask = false;
    }
}

void FxLoadTask::RunTask(FxTaskInput input){
    fmt::print("Running Load Task\n");
    if (isTextureLoaded){
        glCopyImageSubData(
            loadedTexture.id, GL_TEXTURE_2D, 0, 0, 0, 0,
            input.outputTexture, GL_TEXTURE_2D, 0, 0, 0, 0, 
            loadedTexture.width, loadedTexture.height, 1);
    }
}