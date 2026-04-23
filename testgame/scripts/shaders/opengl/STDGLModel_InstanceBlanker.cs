#version 460 core

#include "STDGLModel.incl"

layout(local_size_x = STDGLMODEL_LOD_MAX_COUNT, local_size_y = 1, local_size_z = 1) in;

void main() {
    IndirectBuffers[gl_LocalInvocationID.x][0].instanceCount = 0;
}