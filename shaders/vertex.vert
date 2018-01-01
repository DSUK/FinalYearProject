#version 120
varying vec3 pos;
varying vec3 normal;

uniform mat4 MV;
uniform mat4 ProjectionMatrix;
mat4 MVP;
void main()
{
	MVP = ProjectionMatrix*MV;
	gl_Position = MVP*gl_Vertex;
	pos = vec3(MV*gl_Vertex);
	normal = vec3(mat3(MV)*gl_Normal); //assuming no non-uniform scaling or shering
}
