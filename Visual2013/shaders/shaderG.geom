#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 color[];
in vec3 pos[];
in mat3 TBN[];
in vec2 texCoord[];

out vec3 fragColor;
out vec3 fragPos;
out mat3 fragTBN;
out vec2 fragTexCoord;

void main() {
  for(int i = 0; i < 3; i++) {
    gl_Position = gl_in[i].gl_Position;

	fragColor = color[i];
    fragPos = pos[i];
    fragTBN = TBN[i];
	fragTexCoord = texCoord[i];

    EmitVertex();
  }
  EndPrimitive();
}