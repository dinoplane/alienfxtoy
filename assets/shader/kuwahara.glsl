#version 460 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(rgba32f, binding = 1) uniform image2D imgInput0;

// https://blog.maximeheckel.com/posts/on-crafting-painterly-shaders/
// layout (location = 0) uniform float t;                 /** Time */
float t = 0.0;
int kernelSize = 5;
int sectorCount = 4;

void calculateAvgAndVariance( ivec2 startCoord, out vec3 avg, out float variance) {
    vec3 avgColor = vec3(0.0);
    vec3 varColor = vec3(0.0);
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            ivec2 coord = startCoord + ivec2(i, j);
            vec4 color = imageLoad(imgInput0, coord);
            avgColor += color.rgb;
            varColor += color.rgb * color.rgb;
        }
    }
    avgColor /= float(kernelSize * kernelSize);
    varColor /= float(kernelSize * kernelSize);

    varColor -= avgColor * avgColor;
    
    avg = avgColor;
    variance = dot(varColor, vec3(0.299, 0.587, 0.114));
}

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);	
    vec4 input0 = imageLoad(imgInput0, texelCoord);

    vec3 boxAvgColors[4];
    float boxVariances[4];
    calculateAvgAndVariance(texelCoord + ivec2(-kernelSize, -kernelSize), boxAvgColors[0], boxVariances[0]);
    calculateAvgAndVariance(texelCoord + ivec2(1, -kernelSize), boxAvgColors[1], boxVariances[1]);
    calculateAvgAndVariance(texelCoord + ivec2(-kernelSize, 1), boxAvgColors[2], boxVariances[2]);
    calculateAvgAndVariance(texelCoord + ivec2(1, 1), boxAvgColors[3], boxVariances[3]);

    float minVariance = boxVariances[0];
    vec3 finalColor = boxAvgColors[0];


    for (int i = 1; i < sectorCount; i++) {
        if (boxVariances[i] < minVariance) {
            minVariance = boxVariances[i];
            finalColor = boxAvgColors[i];
        }
    }
    vec4 value = vec4(finalColor, 1.0);
    imageStore(imgOutput, texelCoord, value);
}