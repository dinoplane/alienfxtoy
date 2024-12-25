#version 460 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(rgba32f, binding = 1) uniform image2D imgInput0;


// layout (location = 0) uniform float t;                 /** Time */
float t = 0.0;

void main() {
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	
    vec4 input0 = imageLoad(imgInput0, texelCoord);

    value.r = float(texelCoord.x)/(gl_NumWorkGroups.x);
    value.g = float(texelCoord.y)/(gl_NumWorkGroups.y);
    value.b = input0.b;
    imageStore(imgOutput, texelCoord, value);
}