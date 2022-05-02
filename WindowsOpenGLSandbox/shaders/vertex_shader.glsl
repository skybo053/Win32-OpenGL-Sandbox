#version 430

in layout(location=0)  vec4 pos;
in layout (location=1) vec4 vertexColor;

uniform vec4 allRedColor;
uniform float flippedY;

out vec4 outVertexColor;

void main()
{
  // gl_Position    = pos;
  // outVertexColor = vertexColor;
  
  gl_Position = pos;
  gl_Position.y *= flippedY;
  outVertexColor = allRedColor;
  
}