#version 120
varying vec3 pos;
varying vec3 normal;
//this file gets called, then the fragment shader is called, the fragment shader cannot access vertecies directly, so they have to be passed from here

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex; //vertex position (on screen)
    pos = vec3(gl_ModelViewMatrix*gl_Vertex);               //transfrom the vertex and pass it onto the fragment shader
    normal = vec3(gl_ModelViewMatrix*gl_Normal);            //transform normal, pass it onto the fragment shader
}
