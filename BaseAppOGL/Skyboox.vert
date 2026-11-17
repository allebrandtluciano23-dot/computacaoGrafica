#version 330 core 
layout (location =0 ) in vec3 aPos;

out vec3 TexCoords;


uniform mat4 projection; 
uniform mat4 view;

void main()
{
	TexCoords = aPos;    
	vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0); // remove a translacao da view
	gl_Position = pos.xyww; //w=w para manter a profundidade no far plane
}