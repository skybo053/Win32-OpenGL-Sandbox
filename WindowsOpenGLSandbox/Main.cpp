#include <iostream>
#include <Windows.h>
#include <memory>
#include <thread>
#include "Geometries.h" //includes glm and openGL


HWND createWindow(const HINSTANCE& pHInstance);
BOOL initOpenGL(HWND pHwnd, HDC pDeviceContext);
LRESULT WINAPI WindowProc(HWND pHwnd, UINT pUInt, WPARAM pWParam, LPARAM pLParam);
std::string loadShader(const char* pShaderFileName);
GLuint initShaders();


/*
 *
 ************** ENTRY POINT ********************
 *
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
  HWND   vHwnd          = NULL;
  HDC    vDeviceContext = NULL;
  int    vResult        = NULL;
  int    vRunning       = 0; 
  GLuint vProgramId   = -1;

  vHwnd = createWindow(hInstance);

  if(vHwnd == NULL)
  {
    return 1;
  }
  
  vDeviceContext = GetDC(vHwnd);
  vResult        = initOpenGL(vHwnd, vDeviceContext);
  vProgramId     = initShaders();

  if(!vResult)
  {
    return 1;
  }

  ShowWindow(vHwnd, nCmdShow);
 
  //////////// openGL info ////////////
  
  glEnable(GL_DEPTH_TEST);

  ShapeData vShapeData = ShapeGenerator::generateTriangle();

  GLuint vVertexBufferId;
  GLuint vIndexBufferId;

  glGenBuffers(1, &vVertexBufferId);
  glGenBuffers(1, &vIndexBufferId);

  glBindBuffer(GL_ARRAY_BUFFER, vVertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndexBufferId);

  glBufferData(GL_ARRAY_BUFFER, vShapeData.getVertexBufferSize(), vShapeData.vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vShapeData.getIndexBufferSize(), vShapeData.indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));

  //setting up a uniforms
  glm::vec4 vUniformColorRed(1.0F, 0.0F, 0.0F, 0.0F);
  glm::vec4 vUniformColorBlue(0.0F, 0.0F, 1.0F, 0.0F);

  GLint vUniformColorLocation   = glGetUniformLocation(vProgramId, "allRedColor");
  GLint vUniformFlippedLocation = glGetUniformLocation(vProgramId, "flippedY");


  MSG vMsg;
  vRunning = 1;
  while(vRunning)
  {
    while(PeekMessage(&vMsg, NULL, NULL, NULL, PM_REMOVE))
    {
      if(vMsg.message == WM_QUIT)
      {
        vRunning = false;
        break;
      }

      TranslateMessage(&vMsg);
      DispatchMessage(&vMsg);
    }
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUniform4f(vUniformColorLocation, vUniformColorRed.r, vUniformColorRed.g, vUniformColorRed.b, vUniformColorRed.a);
    glUniform1f(vUniformFlippedLocation, 1.0F);
    glDrawArrays(GL_TRIANGLES, 0, vShapeData.numVertices);

    glUniform4f(vUniformColorLocation, vUniformColorBlue.r, vUniformColorBlue.g, vUniformColorBlue.b, vUniformColorBlue.a);
    glUniform1f(vUniformFlippedLocation, -1.0F);
    glDrawArrays(GL_TRIANGLES, 0, vShapeData.numVertices);



    //glDrawElements(GL_TRIANGLES, vShapeData.numIndices, GL_UNSIGNED_INT, 0);
    
    SwapBuffers(vDeviceContext);
  }
  
  return 0;
}

HWND createWindow(const HINSTANCE& pHInstance)
{
  const char* vWindowClassName = "OpenGLWindow";
  HWND        vHWND            = NULL;
  WNDCLASS    vWindowClass     = {};

  vWindowClass.lpfnWndProc   = WindowProc;
  vWindowClass.hInstance     = pHInstance;
  vWindowClass.lpszClassName = vWindowClassName;
  vWindowClass.style         = CS_OWNDC;

  RegisterClass(&vWindowClass);
  
  vHWND = CreateWindowEx(NULL, vWindowClassName, "OpenGLWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, pHInstance, NULL);
  
  return vHWND;
}


BOOL initOpenGL(HWND pHwnd, HDC pDeviceContext)
{
  HGLRC                 vOpenGlRenderingContext = NULL;
  PIXELFORMATDESCRIPTOR vPixelFormatDescriptor  = {};

  vPixelFormatDescriptor.nSize = sizeof(vPixelFormatDescriptor);
  vPixelFormatDescriptor.nVersion = 1;
  vPixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER | PFD_SWAP_LAYER_BUFFERS;
  vPixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
  vPixelFormatDescriptor.cColorBits = 32;
  vPixelFormatDescriptor.cRedBits = 8;
  vPixelFormatDescriptor.cGreenBits = 8;
  vPixelFormatDescriptor.cBlueBits = 8;
  vPixelFormatDescriptor.cAlphaBits = 8;
  vPixelFormatDescriptor.cDepthBits = 32;
  vPixelFormatDescriptor.cStencilBits = 8;

  int pixelFormat = ChoosePixelFormat(pDeviceContext, &vPixelFormatDescriptor);
  SetPixelFormat(pDeviceContext, pixelFormat, &vPixelFormatDescriptor);

  vOpenGlRenderingContext = wglCreateContext(pDeviceContext);
  BOOL  vWorked           = wglMakeCurrent(pDeviceContext, vOpenGlRenderingContext);

  if(!vWorked)
  {
    return false;
  }


  //GLADloadproc vTemp = (void*(*)(const char*))wglGetProcAddress;
  //vWorked = gladLoadGLLoader(vTemp);
  vWorked = gladLoadGL();

  return vWorked;
}


LRESULT CALLBACK WindowProc(HWND pHwnd, UINT pMsg, WPARAM pWParam, LPARAM pLParam)
{
  static PAINTSTRUCT ps = {};

  switch(pMsg)
  {
    case WM_CLOSE:
    {
      PostQuitMessage(0);
      break;
    }
    default:
    {
      return DefWindowProc(pHwnd, pMsg, pWParam, pLParam);
    }
  };
}


GLuint initShaders()
{
  std::string vVertexShader   = loadShader("shaders/vertex_shader.glsl");
  std::string vFragmentShader = loadShader("shaders/fragment_shader.glsl");

  GLuint vProgramId        = glCreateProgram();
  GLuint vVertexShaderId   = glCreateShader(GL_VERTEX_SHADER);
  GLuint vFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  const char* vTemp = vVertexShader.c_str();
  glShaderSource(vVertexShaderId, 1, &vTemp, NULL);
  vTemp = vFragmentShader.c_str();
  glShaderSource(vFragmentShaderId, 1, &vTemp, NULL);

  glCompileShader(vVertexShaderId);
  glCompileShader(vFragmentShaderId);

  glAttachShader(vProgramId, vVertexShaderId);
  glAttachShader(vProgramId, vFragmentShaderId);
  glLinkProgram(vProgramId);
  glUseProgram(vProgramId);

  return vProgramId;
}