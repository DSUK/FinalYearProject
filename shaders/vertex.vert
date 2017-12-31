#version 120
varying vec3 pos;
varying vec3 normal;

uniform mat4 MV;
mat4 MVP;
void main()
{
	MVP = gl_ProjectionMatrix*MV;
    gl_Position = MVP*gl_Vertex; //vertex position (on screen)
    pos = vec3(MV*gl_Vertex);               //transfrom the vertex and pass it onto the fragment shader
    normal = vec3(gl_NormalMatrix*gl_Normal);            //transform normal, pass it onto the fragment shader
}
