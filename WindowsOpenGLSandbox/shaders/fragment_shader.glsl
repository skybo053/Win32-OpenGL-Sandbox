#version 430

in vec4 outVertexColor;
out vec4 fragmentColor;

void main()
{
  fragmentColor = outVertexColor;
}