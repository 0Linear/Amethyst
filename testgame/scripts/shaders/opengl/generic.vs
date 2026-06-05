#version 460 core

#include "STDGLModel.incl"
#include "STDGLCamera.incl"

layout (location = 0) in vec3 aPos;
out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
};


void main() {
    gl_Position = CameraViewProjection * InstanceMatrices[InstanceIndices[gl_BaseInstance][gl_InstanceID]] * vec4(aPos, 1.0f);
}
