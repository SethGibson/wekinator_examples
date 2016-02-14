#version 150
uniform mat4 ciModelViewProjection;

in vec4 ciPosition;

in vec4 i_Tint;
in vec3 i_Position;
in vec2 i_Size;


out vec4 oColor;

void main()
{
	vec3 scaledPos = ciPosition.xyz;
	scaledPos.x*=i_Size.x;
	scaledPos.z*=i_Size.y;
	oColor = i_Tint;

	gl_Position = ciModelViewProjection*vec4(scaledPos+i_Position, 1.0);
}