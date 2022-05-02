#version 430

in layout(location=0)  vec4 pos;
in layout (location=1) vec4 vertexColor;

uniform vec4 allRedColor;

out vec4 outVertexColor;

void main()
{
  gl_Position    = pos;
  // outVertexColor = vertexColor;
  outVertexColor = allRedColor;
  
}