#pragma once

#include "GeKo_Graphics/GUI/GUI.h"
#include "GeKo_Graphics/Material/Texture.h"
#include <imgui.h>
#include <string>
#include <map>
#include <glm/ext.hpp>

///These Classes are wrapper classes for the ImGui Render methods

namespace GuiElement
{
  class Element
  {
  public:
    Element() {}
    ~Element() {}

    virtual void render() = 0;
    virtual void dispose() = 0;

  private:
  };

  ///Element to display plain text
  class Text : public Element
  {
  
  public:
    Text(std::string text, glm::fvec4 textColor = glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f)) : m_text(text) { m_textColor = textColor; }
    
    inline void render() { ImGui::TextColored(ImVec4(m_textColor.r, m_textColor.g, m_textColor.b, m_textColor.a), m_text.c_str()); }
    inline void dispose() { delete this; }

  private:
    ~Text() {}

    std::string m_text;
    glm::fvec4 m_textColor;

  };

  ///Element for a simple slider for a float variable
  class SliderFloat : public Element
  {

  public:
    SliderFloat(std::string ID, float *parameter, float min, float max) : m_ID(ID), m_parameter(parameter), m_min(min), m_max(max) {}
    inline void render() { ImGui::SliderFloat(m_ID.c_str(), m_parameter, m_min, m_max); }
    inline void dispose() {delete this;}

  private:
    ~SliderFloat() {}

    std::string m_ID;
    float *m_parameter, m_min, m_max;
  };

  ///Class to create a new header for the GUI
  /** Class can have subelements, which are contained within the header */
  class Header : public Element
  {
  public:
    Header(std::string ID) : m_ID(ID) { }
    inline void render()
    {
      if (ImGui::CollapsingHeader(m_ID.c_str())) 
        for (std::vector<Element*>::iterator it = m_childElements.begin(); it != m_childElements.end(); ++it)
          (*it)->render(); 
    }

    inline void addElement(Element *child) { m_childElements.push_back(child); }
	inline void clearElements(){ m_childElements.clear(); }
    inline void dispose() { delete this;}

  private:
    ~Header() 
    { 
      for(std::vector<Element*>::iterator it = m_childElements.begin(); it != m_childElements.end(); ++it)
        (*it)->dispose();
    }

    std::string m_ID;
    std::vector<Element*> m_childElements;

  };

  ///Creates a spacing between the last and next element
  class Spacing : public Element
  {
  public:
    Spacing() {}
    inline void render() { ImGui::Spacing(); }
    inline void dispose() { delete this; }

  private:
    ~Spacing() {}

  };

  ///Creates a separator line between the last and the next element
  class Separator : public Element
  {
  public:
    Separator() {}
    inline void render() { ImGui::Separator(); }
    inline void dispose() { delete this; }

  private:
    ~Separator() {}

  };

  ///Instead of moving to the next line, the next item will be placed in the same line as the last one
  class SameLine : public Element
  {
  public:
    SameLine() {}
    inline void render() { ImGui::SameLine(); }
    inline void dispose() { delete this; }

  private:
    ~SameLine() {}

  };

  ///Class for creating a simple push button
  class PushButton : public Element
  {
  public:
    PushButton(std::string ID) : m_isPushed(false), m_ID(ID) {}
    inline void render() { m_isPushed = ImGui::Button(m_ID.c_str()); }
    inline bool isPushed() { return m_isPushed; }
    inline void dispose() { delete this; };
    inline bool* getBooleanAdress() { return &m_isPushed; }
  private:
    ~PushButton() {}

    bool m_isPushed;
    std::string m_ID;
  };

  ///Class for a checkbox for a boolean variable
  class Checkbox : public Element
  {
  public:
    Checkbox(std::string ID, bool startingValue = false) : m_ID(ID), m_bool(startingValue){}
    inline void render() 
    {
      ImGui::Checkbox(m_ID.c_str(),&m_bool);
    }
    
    inline void dispose()   { delete this; };
    inline bool isActive()  { return m_bool; }

  private:
    ~Checkbox() {}

    bool m_bool;
    std::string m_ID;

  };

  ///Box that shows a normal int variable inside of a colored border
  class IntBox : public Element
  {
  public:
    IntBox(int *number, glm::fvec4 textColor, glm::fvec4 borderColor) : m_number(number), m_textColor(textColor), m_borderColor(borderColor) {}

    inline void render() 
    {
      
      std::string strInteger = " ";
      strInteger.append(std::to_string(*m_number));
      strInteger.append(" ");

      float wrapWidth = strInteger.size()*300;

      ImVec2 pos = ImVec2(ImGui::GetCursorScreenPos().x + wrapWidth, ImGui::GetCursorScreenPos().y);
      ImVec2 size = ImVec2(ImGui::GetCursorScreenPos().x + wrapWidth + 10, ImGui::GetCursorScreenPos().y + ImGui::GetTextLineHeight());

      ImGui::GetWindowDrawList()->AddRectFilled(pos, size, ImGui::ColorConvertFloat4ToU32(ImVec4(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a)));
      ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrapWidth);
      ImGui::TextColored(ImVec4(m_textColor.r, m_textColor.g, m_textColor.b, m_textColor.a), strInteger.c_str(), wrapWidth);
      ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::ColorConvertFloat4ToU32(ImVec4(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a)));
      ImGui::PopTextWrapPos();
    }

    inline void dispose(){ delete this; }

  private:
    ~IntBox() {}
    glm::fvec4 m_textColor;
    glm::fvec4 m_borderColor;
    int *m_number;
  };

  ///Widget with three float values for easy color tweaking
  class ColorEditRGB : public Element
  {
  public:
    ColorEditRGB(std::string ID, glm::fvec3 *color) : m_ID(ID), m_colorVec(color) { m_color[0] = m_colorVec->r; m_color[1] = m_colorVec->g; m_color[2] = m_colorVec->b; }
  
    inline void render() { ImGui::ColorEdit3(m_ID.c_str(), m_color); m_colorVec->r = m_color[0]; m_colorVec->g = m_color[1]; m_colorVec->b = m_color[2]; }
    inline void dispose(){ delete this; }
   
  private:
    ~ColorEditRGB() {}
    float m_color[3];

    std::string m_ID;
    glm::fvec3 *m_colorVec;
  };

  ///Widget with four float values for easy color and alpha tweaking
  class ColorEditRGBA : public Element
  {
  public:
    ColorEditRGBA(std::string ID, glm::fvec4 *color) : m_ID(ID), m_colorVec(color) { m_color[0] = m_colorVec->r; m_color[1] = m_colorVec->g; m_color[2] = m_colorVec->b; m_color[3] = m_colorVec->a; }

    inline void render() { ImGui::ColorEdit4(m_ID.c_str(), m_color); m_colorVec->r = m_color[0]; m_colorVec->g = m_color[1]; m_colorVec->b = m_color[2]; m_colorVec->a = m_color[3]; }
    inline void dispose(){ delete this; }

  private:
    ~ColorEditRGBA() {}
    float m_color[4];

    std::string m_ID;
    glm::fvec4 *m_colorVec;
  };

  //Class to create a game inventory
  class Inventory : public Element
  {
  public:
    Inventory(std::map<std::string,Texture*> *inventory, int numberOfSlotsWidth) :
        m_inventory(inventory),
        m_numberOfSlotsWidth(numberOfSlotsWidth),
        m_selectedItem("none")
    {

    }

    inline void render()
    {
        int counter = 0;
        for (std::map<std::string,Texture*>::iterator it = m_inventory->begin();it!=m_inventory->end();++it)
        {
            ImGui::PushID(counter);
            int frame_padding = 1;
            ImTextureID texId = ImTextureID((*it).second->getTexture());

            //selected item gets a bigger frame
            if(m_selectedItem == (*it).first)
            {
                frame_padding = 2;

            }

            if (ImGui::ImageButton(texId, ImVec2(40,40), ImVec2(0,0), ImVec2(1,1), frame_padding))
            {
                m_selectedItem = (*it).first;
            }

            //hovering reveals item name
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip((*it).first.c_str());

            ImGui::PopID();
            ImGui::NextColumn();

            //Same line if width is not reached
            if(counter % m_numberOfSlotsWidth != m_numberOfSlotsWidth-1)
                ImGui::SameLine();

            counter++;
        }
    }

    inline std::string getSelectedItem() {return m_selectedItem;}

    inline void dispose(){ delete this; }

  private:
    ~Inventory() {}
    std::map<std::string,Texture*> *m_inventory;
    int m_numberOfSlotsWidth;
    std::string m_selectedItem;
  };

  const int width = 50;
  static float arr[width];

  ///Class for showing a progress (Loading, HP Bar,...)
  class ProgressBar : public Element
  {

  public:
	  ProgressBar(int *hp, int *hpMax, int hpBarWidth, glm::fvec4 color) : m_hp(hp), m_hpMax(hpMax), m_hpBarWidth(hpBarWidth), m_color(color){ }
	  inline void render()
	  {
		  int currentHP = *m_hp;
		  float restHP = static_cast<float>(currentHP) / static_cast<float>(*m_hpMax);

		  if (restHP >= 0 && restHP <= 1)
		  {

			  restHP *= width;

			  for (int i = width - 1; i >= restHP; i--)
			  {
				  arr[i] = 0.0f;
			  }

			  for (int i = 0; i < restHP; i++)
			  {
				  arr[i] = 50.0f;
			  }

		  }

		  else if (restHP < 0)
		  {
			  for (int i = 0; i < width; i++)
			  {
				  arr[i] = 0.0f;
			  }

		  }

		  else
		  {
			  for (int i = 0; i < width; i++)
			  {
				  arr[i] = 50.0f;
			  }

		  }
		  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(m_color.r, m_color.g, m_color.b, m_color.a));
		  ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(m_color.r, m_color.g, m_color.b, m_color.a));
		  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 00.7f));
		  ImGui::PushStyleColor(ImGuiCol_TooltipBg, ImVec4(0.0, 0.0, 0.0, 0.0));

		  ImGui::PlotHistogram("", arr, ((int)(sizeof(arr) / sizeof(*arr))), 0, " ", 0.0f, 50.0f, ImVec2(m_hpBarWidth, 0));

		  if (ImGui::IsItemHovered())
			  ImGui::SetTooltip(" ");
	  }
	  inline void dispose() { delete this; }


  private:
	  ~ProgressBar() {}

	  std::string m_ID;
	  glm::fvec4 m_color;
	  int *m_hp, *m_hpMax, m_hpBarWidth;
  };
  
  ///Class to create a nested window inside the current GUI
  class NestedWindow
  {
  public:
    NestedWindow() : 
      m_visible(true), 
      m_guiElements(new std::vector<GuiElement::Element*>()),
      m_windowName(" "),
      m_windowWidth(100),
      m_windowHeight(300),
      m_windowPosX(300 - 200), m_windowPosY(100 / 2),
      m_bgAlpha(0.65),
      m_titlebar(true),
      m_border(false),
      m_resize(true),
      m_move(true),
      m_scrollbar(true),
      m_collapse(true)
    {}

    inline void render()
    { 
      ImGuiWindowFlags window_flags = 0;
      if (!m_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
      if (m_border)     window_flags |= ImGuiWindowFlags_ShowBorders;
      if (!m_resize)    window_flags |= ImGuiWindowFlags_NoResize;
      if (!m_move)      window_flags |= ImGuiWindowFlags_NoMove;
      if (!m_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
      if (!m_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;

      ImGui::SetNextWindowPos(ImVec2(m_windowPosX, m_windowPosY));
      //ImGui::SetNextWindowSize(ImVec2(m_windowWidth,m_windowHeight));
      ImGui::Begin(m_windowName.c_str(), &m_visible, ImVec2(m_windowWidth, m_windowHeight), m_bgAlpha, window_flags);
      
      //ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
      //ImGui::Begin("Another Window", &m_visible);
      for (std::vector<GuiElement::Element*>::iterator it = m_guiElements->begin(); it != m_guiElements->end(); ++it)
        (*it)->render();

      ImVec2 newSize = ImGui::GetWindowSize();

      if (newSize.y >= 35)
      {
        m_windowWidth = newSize.x;
        m_windowHeight = newSize.y;
      }

      ImVec2 newPos = ImGui::GetWindowPos();
      m_windowPosX = newPos.x;
      m_windowPosY = newPos.y;
      
      ImGui::End();
    }
    
    inline void dispose(){ delete this; }
    inline void addElement(GuiElement::Element *comp) { m_guiElements->push_back(comp); }
	inline void clearElements(){ m_guiElements->clear(); }
    inline bool isVisible() { return m_visible; }
    inline void show() { m_visible = true; }
    inline void hide() { m_visible = false;}
    inline void toggleVisibility() { m_visible = !m_visible; }

    void setBackgroundAlpha(float a)         { m_bgAlpha = a; }
    void setPosition(float posX, float posY) { m_windowPosX = posX; m_windowPosY = posY; }
    void setTitleBarVisible(bool visible)    { m_titlebar = visible; }
    void setMoveable(bool moveable)          { m_move = moveable; }
    void setBorderVisible(bool visible)      { m_border = visible; }
    void setResizable(bool resize)           { m_resize = resize; }
    void setUseScrollbar(bool scrollbar)     { m_scrollbar = scrollbar; }
    void setCollapsable(bool collapse)       { m_collapse = collapse; }
    void setName(std::string name)           { m_windowName = name; }
    void setSize(float width, float height)  { m_windowWidth = width; m_windowHeight = height; }
  private:
    ~NestedWindow() {}

    bool m_visible;
    std::vector<GuiElement::Element*> *m_guiElements;
    std::string m_windowName;
    int m_windowWidth, m_windowHeight;
    float m_windowPosX, m_windowPosY;
    float m_bgAlpha;
    bool m_titlebar;
    bool m_border;
    bool m_resize;
    bool m_move;
    bool m_scrollbar;
    bool m_collapse;

  };

};
