#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>


std::string loadShader(const char* pShaderFileName)
{
  std::ifstream vInputStream(pShaderFileName);

  if(!vInputStream)
  {
    char vErrorMessage[255];
    strcat(vErrorMessage, "Unable to load shader file: ");
    strcat(vErrorMessage, pShaderFileName);
    OutputDebugString(vErrorMessage);
  }

  std::stringstream vStringStream;
  vStringStream << vInputStream.rdbuf();

  return vStringStream.str();
}