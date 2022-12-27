#version 450
#extension GL_ARB_separate_shader_objects : enable

//layout ( std140, set = 0, binding = 0 ) uniform UniformBufferObject
//{
//    mat4 mView;
//    int mWidth;
//    int mHeight;
//} ubo;

//layout( push_constant ) uniform constants
//{
//    mat4 mModel;
//} PushConstants;


layout( location = 0 ) in vec2 inPosition;
//layout( location = 1 ) in vec3 inColor;
//layout( location = 2 ) in vec2 inTexCoord;

//layout( location = 0 ) out vec3 outFragmentColor;
//layout( location = 1 ) out vec2 outTexCoord;

void main()
{
//    gl_Position = ubo.mView * PushConstants.mModel * vec4( inPosition, 0.F, 1.F );
    gl_Position = vec4( inPosition, 0.F, 1.F );
//    outFragmentColor = inColor;
//    outTexCoord = inTexCoord;
}
