#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D rubiktexture;



void main(){
	
	FragColor = texture(rubiktexture, TexCoord);
}