//type vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 color;
layout (location = 3) in float texSlot;

out vec2 v_TexCoord;
out vec4 v_VertexColor;
out float v_TexSlot;

uniform mat4 modelViewProjection;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	v_TexCoord = texCoord;
	v_VertexColor = color;
	v_TexSlot = texSlot;
}

//type fragment
#version 330 core

in vec2 v_TexCoord;
in vec4 v_VertexColor;
in float v_TexSlot;
out vec4 fragColor;

uniform sampler2D u_Textures[16];

void main()
{
	fragColor = texture(u_Textures[int(v_TexSlot)], v_TexCoord) * v_VertexColor;
}