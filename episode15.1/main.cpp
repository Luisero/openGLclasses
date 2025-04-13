#include "include/glad/glad.h"
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// Globals
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

bool gQuit = false;
// VAO
GLuint gVertexArrayObject;
// VBO
GLuint gVertexBufferObject;
GLuint gVertexBufferObject2;
// Create Program for our shaders
GLuint gGraphicsPipelineShaderProgram = 0;
void CheckShaderCompilation(GLuint shader) {
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "Erro na compilação do shader:\n" << infoLog << std::endl;
  }
}

GLuint CompileShader(GLuint type, const std::string source) {
  GLuint shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  // Adicione esta linha para verificar a compilação
  CheckShaderCompilation(shader);

  return shader;
}
//*}*/
/*const std::string vs = "#version 410 core\n"*/
/*                       "layout (location = 0) in vec3 aPos;\n"*/
/*                       "void main()\n"*/
/*                       "{\n"*/
/*                       "   gl_Position = vec4(aPos.x, aPos.y,
 * aPos.z, 1.0);\n"*/
/*                       "}\0";*/
/*const std::string fs = "#version 410 core\n"*/
/*"out vec4 FragColor;\n"*/
/**/
/*"{\n"*/
/*"   FragColor = vec4(0.6f, 0.5f, 0.2f, 1.0f);\n"*/
/*"}\n\0";*/
/**/
std::string LoadShaderAsString(const std::string &filename) {
  std::string result = "";
  std::string line = "";
  std::ifstream myFile(filename.c_str());

  if (!myFile.eof()) {
    while (std::getline(myFile, line)) {
      result += line + '\n';
    }

    myFile.close();
  }

  return result;
}

GLuint CreateShaderProgram(const std::string &vertexShaderSouce,
                           const std::string &fragmentShaderSource) {
  GLuint programObject = glCreateProgram();

  GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSouce);
  GLuint myFragmentShader =
      CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  glAttachShader(programObject, myVertexShader);
  glAttachShader(programObject, myFragmentShader);
  glLinkProgram(programObject);

  glValidateProgram(programObject);
  // delete shaders
  glDeleteShader(myFragmentShader);
  glDeleteShader(myFragmentShader);
  return programObject;
}

void CreateGraphicsPipeline() {
  std::string vs = LoadShaderAsString("./shaders/default_vert.glsl");
  std::string fs = LoadShaderAsString("./shaders/default_frag.glsl");
  gGraphicsPipelineShaderProgram = CreateShaderProgram(vs, fs);
}

void GetOpenGLVersionInfo() {
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << "\n";
}
void VertexSpecification() {
  const std::vector<GLfloat> vertexData{
      // first triangle
      -0.5f, -0.5f, 0.0f, // ve1
      1.0f, 0.0f, 0.0f,   // color
      0.5f, -0.5f, 0.0f,  // vej
      0.0f, 1.0f, 0.0f,   // color
      -0.5f, 0.5f, 0.0f,  // ve3
      0.0f, 0.0f, 1.0f,   // color
      // second triangle
      0.5f, -0.5f, 0.0f, // vec1
      0.0f, 1.0f, 0.0f,  // color
      0.5f, 0.5f, 0.0f,  // vec2
      1.0f, 0.0f, 0.0f,  // color
      -0.5f, 0.5f, 0.0f, // vec3
      0.0f, 0.0f, 1.0f,  // color
  };

  const std::vector<GLfloat> vertexColors{};

  // Create VAO and bind
  glGenVertexArrays(1, &gVertexArrayObject);
  glBindVertexArray(gVertexArrayObject);

  // start generation our vbo
  glGenBuffers(1, &gVertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
  // Put info on our VBO
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat),
               vertexData.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  // How VAO will read our data
  glVertexAttribPointer(0,
                        3,        // Size of informations (x,y,z)
                        GL_FLOAT, // Type of information
                        GL_FALSE, sizeof(GL_FLOAT) * 6, (void *)0

  );
  // generate our second VBO for colors
  glGenBuffers(1, &gVertexBufferObject2);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject2);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat),
               vertexData.data(), GL_STATIC_DRAW);
  // Link attributes in our VAO
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6,
                        (void *)(sizeof(GL_FLOAT) * 3));

  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
void InitializeProgram() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL2 could not intialize video substystem\n";

    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  gGraphicsApplicationWindow =
      SDL_CreateWindow("Opengl First program", 0, 0, gScreenWidth,
                       gScreenHeight, SDL_WINDOW_OPENGL);

  if (gGraphicsApplicationWindow == nullptr) {
    std::cout << "Could not create window.\n";
    exit(1);
  }

  gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    std::cout << "Glad was not initialized:/\n";
    exit(1);
  }
  GetOpenGLVersionInfo();
}

void Input() {

  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      std::cout << "Exiting...\n";
      gQuit = true;
    }
    if (e.type == SDL_KEYDOWN) {
    }
  }
}
void PreDraw() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  CreateGraphicsPipeline();
  glViewport(0, 0, gScreenWidth, gScreenHeight);
  glClearColor(0.04f, 0.36f, 0.45f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glUseProgram(gGraphicsPipelineShaderProgram);
}
void Draw() {
  glBindVertexArray(gVertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}
void MainLoop() {
  while (!gQuit) {
    Input();

    PreDraw();
    Draw();

    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
  }
}

void CleanUp() {
  SDL_DestroyWindow(gGraphicsApplicationWindow);
  SDL_Quit();
}

int main() {
  InitializeProgram();
  VertexSpecification();
  CreateGraphicsPipeline();

  MainLoop();
  CleanUp();
  return 0;
}
