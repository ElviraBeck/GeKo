#pragma once

//FIX BUG WITH TOGGLE BUTTONS

#include <string>
#include <vector>
#include <stdio.h>
#include "GUIComponents.hpp"

class Window;

///This Class sets up a GUI and offers methods to create its elements

class GUI
{
  public:
    ///Constructor
    GUI(std::string windowName, int windowWidth = 200, int windowHeight = 100);
    ///Destructor
    ~GUI();
    
    ///renders the GUI into to the given window
    void render(Window& window);

    ///adds an element to the GUI
    /**See GUIComponents.hpp for an overview ovver different GUI Elements that are supported!*/
    void addElement(GuiElement::Element *comp);

    ///Adds a nested window to the current GUI
    /**The additional window can be shown or hidden. Use a GUI Button to toggle it (See example HUD) !*/
    void addNestedWindow(GuiElement::NestedWindow *nestedWindow);

    ///method to set the GUI to visible
    void show();

    ///method to set the GUI to invisible
    void hide();

    ///getter for GUI visibility
    bool visible();

    ///method to set the alpha value of the GUI background
    void setBackgroundAlpha(float a);
    
    ///sets the position of the window on the screen
    void setPosition(float posX, float posY);

    ///changes the visibility of the titlebar
    void setTitleBarVisible(bool visible);


    ///changes the ability to move the GUI around
    void setMoveable(bool moveable);

    ///sets the GUI border to visible / invisible
    void setBorderVisible(bool visible);

    ///changes the ability to resize the GUI window
    void setResizable(bool resize);

    ///allows using the GUI scrollbar, if the window gets to large
    void setUseScrollbar(bool scrollbar);

    ///allows to collapse the GUI window
    void setCollapsable(bool collapse);
    
	std::string m_windowName;

  protected:
    
    void init();
    void loadFonts();
    void update(Window& window);

    bool m_initialized;
    bool m_visible;
       
    int m_windowWidth, m_windowHeight;
    float m_windowPosX, m_windowPosY;
    float m_bgAlpha;
    bool m_titlebar;
    bool m_border;
    bool m_resize;
    bool m_move;
    bool m_scrollbar;
    bool m_collapse;

    std::vector<GuiElement::Element*> *m_guiElements;
    std::vector<GuiElement::NestedWindow*> *m_nestedWindows;
};
