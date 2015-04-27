#include "GUI.h"
#include <GeKo_Gameplay/Object/Player.h>


//#include<iostream>

//#include<GL/glew.h>
//#include<GLFW/glfw3.h>
//#include<imgui.cpp>

// Shader variables
class PlayerGUI
{
public:
	PlayerGUI(const int hudWidth, const int hudHeight, const int windowHeight, const int windowWidth, const int questHeight, const int questWidth, Player* player);
	~PlayerGUI();

	GUI* getHUD();

	void update();

protected:
	GUI* m_hud;
	int hp, hpMax, exp, expMax, level;
	int m_HUD_WIDTH, m_HUD_HEIGHT, m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_QUEST_WIDTH, m_QUEST_HEIGHT;
	GuiElement::NestedWindow* m_questWindow, *m_inventoryWindow;
	Player* m_player;
};