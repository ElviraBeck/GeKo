#include "GUI.h"

#include "GeKo_Graphics/Window.h"

#include<iostream>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<imgui.cpp>

//create Shader variables
static int shader_handle, vert_handle, frag_handle;
static int texture_location, proj_mtx_location;
static int position_location, uv_location, colour_location;
static size_t vbo_max_size = 20000;
static unsigned int vbo_handle, vao_handle;
static bool mousePressed[2] = { false, false };

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

GUI::GUI(std::string windowName, int windowWidth, int windowHeight) :
  m_initialized(false),
  m_visible(true),
  m_windowName(windowName),
  m_windowWidth(windowWidth),
  m_windowHeight(windowHeight),
  m_guiElements(new std::vector<GuiElement::Element*>()),
  m_nestedWindows(new std::vector<GuiElement::NestedWindow*>()),
  m_windowPosX(windowWidth-200), m_windowPosY(windowHeight/2),
  m_bgAlpha(0.65),
  m_titlebar(true),
  m_border(false),
  m_resize(true),
  m_move(true),
  m_scrollbar(true),
  m_collapse(true)
{

}

GUI::~GUI()
{
  //delete all GUI elements
  for (std::vector<GuiElement::Element*>::iterator it = m_guiElements->begin(); it != m_guiElements->end(); ++it)
    (*it)->dispose();

  //delete shader variables and deallocate memory
  if (vao_handle) glDeleteVertexArrays(1, &vao_handle);
  if (vbo_handle) glDeleteBuffers(1, &vbo_handle);
  glDetachShader(shader_handle, vert_handle);
  glDetachShader(shader_handle, frag_handle);
  glDeleteShader(vert_handle);
  glDeleteShader(frag_handle);
  glDeleteProgram(shader_handle);

  ImGui::Shutdown();
}

ImVec4 clear_col = ImColor(114, 144, 154);

void GUI::render(Window& window)
{
  if (!m_initialized)
    init();

  if (!m_visible)
    return;

  mousePressed[0] = static_cast<bool>(glfwGetKey(window.getWindow(), GLFW_MOUSE_BUTTON_1));
  mousePressed[1] = static_cast<bool>(glfwGetKey(window.getWindow(), GLFW_MOUSE_BUTTON_2));

  update(window);

  //set window options
  ImGuiWindowFlags window_flags = 0;
  if (!m_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
  if (m_border)     window_flags |= ImGuiWindowFlags_ShowBorders;
  if (!m_resize)    window_flags |= ImGuiWindowFlags_NoResize;
  if (!m_move)       window_flags |= ImGuiWindowFlags_NoMove;
  if (!m_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
  if (!m_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;

  ImGui::SetNextWindowPos(ImVec2(m_windowPosX, m_windowPosY));
  ImGui::SetNextWindowSize(ImVec2(m_windowWidth,m_windowHeight));
  ImGui::Begin(m_windowName.c_str(), &m_visible, ImVec2(m_windowWidth, m_windowHeight), m_bgAlpha, window_flags);

  //render all elements
  for (std::vector<GuiElement::Element*>::iterator it = m_guiElements->begin(); it != m_guiElements->end(); ++it)
  {
      (*it)->render();
  }
  ImVec2 newSize = ImGui::GetWindowSize();

  if (newSize.y >= 35)
  {
    m_windowWidth  = newSize.x;
    m_windowHeight = newSize.y;
  }

  ImVec2 newPos = ImGui::GetWindowPos();
  m_windowPosX = newPos.x;
  m_windowPosY = newPos.y;

  ImGui::End();

  for (std::vector<GuiElement::NestedWindow*>::iterator it = m_nestedWindows->begin(); it != m_nestedWindows->end(); ++it)
  {
    if ((*it)->isVisible())
      (*it)->render();
  }

  ImGuiIO& io = ImGui::GetIO();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  ImGui::Render();

}

void GUI::addElement(GuiElement::Element *comp)
{
  m_guiElements->push_back(comp);
}

void GUI::addNestedWindow(GuiElement::NestedWindow *nestedWindow)
{
  m_nestedWindows->push_back(nestedWindow);
}

void GUI::show()
{
  m_visible = true;
}

void GUI::hide()
{
  m_visible = false;
}

bool GUI::visible()
{
  return m_visible;
}

void GUI::setBackgroundAlpha(float a)           { m_bgAlpha = a; }
void GUI::setPosition(float posX, float posY)   { m_windowPosX = posX; m_windowPosY = posY; }
void GUI::setTitleBarVisible(bool visible)      { m_titlebar = visible; }
void GUI::setMoveable(bool moveable)            { m_move = moveable; }
void GUI::setBorderVisible(bool visible)        { m_border = visible; }
void GUI::setResizable(bool resize)             { m_resize  = resize; }
void GUI::setUseScrollbar(bool scrollbar)       { m_scrollbar = scrollbar; }
void GUI::setCollapsable(bool collapse)         { m_collapse = collapse; }

void GUI::update(Window& window)
{
  ImGuiIO& io = ImGui::GetIO();
  // Setup resolution (every frame to accommodate for window resizing)
  int w, h;
  int display_w, display_h;
  glfwGetWindowSize(window.getWindow(), &w, &h);
  glfwGetFramebufferSize(window.getWindow(), &display_w, &display_h);
  io.DisplaySize = ImVec2((float)display_w, (float)display_h);                                   // Display size, in pixels. For clamping windows positions.

  // Setup time step
  static double time = 0.0f;
  const double current_time = glfwGetTime();
  io.DeltaTime = (float)(current_time - time);
  time = current_time;

  // Setup inputs
  // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
  double mouse_x, mouse_y;
  glfwGetCursorPos(window.getWindow(), &mouse_x, &mouse_y);
  mouse_x *= (float)display_w / w;                                                               // Convert mouse coordinates to pixels
  mouse_y *= (float)display_h / h;
  io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);                                          // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
  io.MouseDown[0] = mousePressed[0] || glfwGetMouseButton(window.getWindow(), GLFW_MOUSE_BUTTON_LEFT) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
  io.MouseDown[1] = mousePressed[1] || glfwGetMouseButton(window.getWindow(), GLFW_MOUSE_BUTTON_RIGHT) != 0;

  // Start the frame
  ImGui::NewFrame();
}

static void renderfn(ImDrawList** cmd_lists, int cmd_lists_count)
{
  if (cmd_lists_count == 0)
    return;

  // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glActiveTexture(GL_TEXTURE0);

  //Setup orthographic projection matrix
  const float width = ImGui::GetIO().DisplaySize.x;
  const float height = ImGui::GetIO().DisplaySize.y;

  const float ortho_projection[4][4] =
  {
    { 2.0f / width, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f / -height, 0.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
    { -1.0f, 1.0f, 0.0f, 1.0f },
  };
  glUseProgram(shader_handle);
  glUniform1i(texture_location, 0);
  glUniformMatrix4fv(proj_mtx_location, 1, GL_FALSE, &ortho_projection[0][0]);


  // Grow our buffer according to what we need
  size_t total_vtx_count = 0;

  for (int n = 0; n < cmd_lists_count; n++)
    total_vtx_count += cmd_lists[n]->vtx_buffer.size();


  glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
  size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
  if (neededBufferSize > vbo_max_size)
  {
    vbo_max_size = neededBufferSize + 5000;  // Grow buffer
    glBufferData(GL_ARRAY_BUFFER, vbo_max_size, NULL, GL_STREAM_DRAW);
  }

  // Copy and convert all vertices into a single contiguous buffer
  unsigned char* buffer_data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

  if (!buffer_data)
    return;

  for (int n = 0; n < cmd_lists_count; n++)
  {
    const ImDrawList* cmd_list = cmd_lists[n];
    memcpy(buffer_data, &cmd_list->vtx_buffer[0], cmd_list->vtx_buffer.size() * sizeof(ImDrawVert));
    buffer_data += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(vao_handle);

  int cmd_offset = 0;
  for (int n = 0; n < cmd_lists_count; n++)
  {
    const ImDrawList* cmd_list = cmd_lists[n];
    int vtx_offset = cmd_offset;
    const ImDrawCmd* pcmd_end = cmd_list->commands.end();

    for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
    {
      glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->texture_id);
      glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
      glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
      vtx_offset += pcmd->vtx_count;
    }
    cmd_offset = vtx_offset;
  }

  // Restore modified state
  glBindVertexArray(0);
  glUseProgram(0);
  glDisable(GL_SCISSOR_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);

  glDisable(GL_BLEND);
  //glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

}

void GUI::init()
{
  m_initialized = true;

  const GLchar *vertex_shader =
    "#version 330\n"
    "uniform mat4 ProjMtx;\n"
    "in vec2 Position;\n"
    "in vec2 UV;\n"
    "in vec4 Color;\n"
    "out vec2 Frag_UV;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "	Frag_UV = UV;\n"
    "	Frag_Color = Color;\n"
    "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

  const GLchar* fragment_shader =
    "#version 330\n"
    "uniform sampler2D Texture;\n"
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
    "}\n";

  shader_handle = glCreateProgram();
  vert_handle = glCreateShader(GL_VERTEX_SHADER);
  frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_handle, 1, &vertex_shader, 0);
  glShaderSource(frag_handle, 1, &fragment_shader, 0);
  glCompileShader(vert_handle);
  glCompileShader(frag_handle);
  glAttachShader(shader_handle, vert_handle);
  glAttachShader(shader_handle, frag_handle);
  glLinkProgram(shader_handle);

  texture_location = glGetUniformLocation(shader_handle, "Texture");
  proj_mtx_location = glGetUniformLocation(shader_handle, "ProjMtx");
  position_location = glGetAttribLocation(shader_handle, "Position");
  uv_location = glGetAttribLocation(shader_handle, "UV");
  colour_location = glGetAttribLocation(shader_handle, "Color");

  glGenBuffers(1, &vbo_handle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
  glBufferData(GL_ARRAY_BUFFER, vbo_max_size, NULL, GL_DYNAMIC_DRAW);

  glGenVertexArrays(1, &vao_handle);
  glBindVertexArray(vao_handle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
  glEnableVertexAttribArray(position_location);
  glEnableVertexAttribArray(uv_location);
  glEnableVertexAttribArray(colour_location);

  glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(colour_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  ImGuiIO& io = ImGui::GetIO();
  io.DeltaTime = 1.0f / 60.0f;                                  // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

  io.RenderDrawListsFn = renderfn;
  //io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
  //io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;

  loadFonts();
}

void GUI::loadFonts()
{
  ImGuiIO& io = ImGui::GetIO();
  //ImFont* my_font1 = io.Fonts->AddFontDefault();

  //FONT: http://www.1001fonts.com/aller-font.html


  ImFont* my_font2 = io.Fonts->AddFontFromFileTTF(RESOURCES_PATH "/Fonts/Aller_Std_Rg.ttf", 15.0f);


  //ImFont* my_font1 = io.Fonts->AddFontFromFileTTF("..\\..\\..\\resources\\Fonts\\Karla-Regular.ttf", 15.0f);
  //ImFont* my_font4 = io.Fonts->AddFontFromFileTTF("extra_fonts/ProggyTiny.ttf", 10.0f); my_font4->DisplayOffset.y += 1;
  //ImFont* my_font5 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 20.0f, io.Fonts->GetGlyphRangesJapanese());

  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

  GLuint tex_id;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (void *)(intptr_t)tex_id;
}

float width,height;
