#version 330 core
out vec4 FragColor;

in vec3 oNormal;
in vec2 oTexCoord;

uniform sampler2D ourTexture;

void main()
{
	//FragColor = texture(ourTexture, oTexCoord) * vec4(1., 0., 0., 1.0);
	FragColor = vec4(1.0);
}