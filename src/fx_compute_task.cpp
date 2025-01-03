#include <fx_task.hpp>
#include <fmt/core.h>

FxComputeTask::~FxComputeTask(){
    if (isShaderLoaded){
        ClearShader();
        isValidTask = false;
    }
}

bool FxComputeTask::LoadShader(){
    try {
        loadedShader = new ComputeShader(shaderPath.c_str());
        isShaderLoaded = true;
        isValidTask = true;
    } catch (const std::exception& e) {
        loadedShader = nullptr;
        isShaderLoaded = false;
        isValidTask = false;
    }
    return isShaderLoaded;
}

void FxComputeTask::ClearShader(){
    if (isShaderLoaded){
        isShaderLoaded = false;
        isValidTask = false;
        shaderPath = "";

        loadedShader->deleteProgram();
        delete loadedShader;
        loadedShader = nullptr;
    }

}

void FxComputeTask::RunTask(FxTaskInput input){
    if (isShaderLoaded){
        fmt::print("Running Compute Task\n");
        glBindImageTexture(0, input.outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glBindTextureUnit(0, input.outputTexture);

        glBindImageTexture(1, input.inputTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindTextureUnit(1, input.inputTexture);
        
        loadedShader->use();
        glDispatchCompute(input.width, input.height, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }
}

