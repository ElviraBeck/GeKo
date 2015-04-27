#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(const int hudWidth, const int hudHeight, const int windowWidth, const int windowHeight, const int questHeight, const int questWidth, Player* player)
{
	m_HUD_WIDTH = hudWidth;
	m_HUD_HEIGHT = hudHeight;
	m_WINDOW_HEIGHT = windowHeight;
	m_WINDOW_WIDTH = windowWidth;
	m_QUEST_WIDTH = questWidth;
	m_QUEST_HEIGHT = questHeight;

	Texture bricks((char*)RESOURCES_PATH "/bricks_diffuse.png");
	m_hud = new GUI("testGUI", m_HUD_WIDTH, m_HUD_HEIGHT);
	m_hud->setPosition(250, 500);
	m_hud->setCollapsable(false);
	m_hud->setTitleBarVisible(false);
	m_hud->setBackgroundAlpha(0.5f);
	m_hud->setResizable(false);
	m_hud->setUseScrollbar(false);
	m_hud->setMoveable(false);

	m_player = player;
	hp = m_player->getHealth();
	hpMax = m_player->getHealth();
	exp = m_player->getExp();
	expMax = m_player->getLevelThreshold();

	GuiElement::ProgressBar *hpBar = new GuiElement::ProgressBar(&hp, &hpMax, 300, glm::fvec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_hud->addElement(hpBar);
	m_hud->addElement(new GuiElement::SameLine());
	m_hud->addElement(new GuiElement::Text("HP"));

	GuiElement::ProgressBar *expBar = new GuiElement::ProgressBar(&exp, &expMax, 300, glm::fvec4(1.0f, 0.9960784f, 0.9529411f, 1.0f));
	m_hud->addElement(expBar);
	m_hud->addElement(new GuiElement::SameLine());
	m_hud->addElement(new GuiElement::Text("EXP"));

	m_hud->addElement(new GuiElement::Spacing());
	m_hud->addElement(new GuiElement::Separator());
	m_hud->addElement(new GuiElement::Spacing());
	m_hud->addElement(new GuiElement::Text("LVL"));

	level = m_player->getLvl();
	m_hud->addElement(new GuiElement::SameLine());
	GuiElement::IntBox *lvlBox = new GuiElement::IntBox(&level, glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f), glm::fvec4(0.7f, 0.7f, 0.7f, 1.0f));
	m_hud->addElement(lvlBox);

	m_hud->addElement(new GuiElement::SameLine());
	GuiElement::PushButton *inventoryButton = new GuiElement::PushButton("Inventory");
	m_hud->addElement(inventoryButton);
	m_hud->addElement(new GuiElement::SameLine());
	GuiElement::PushButton *questButton = new GuiElement::PushButton("Quests");
	m_hud->addElement(questButton);

	m_questWindow = new GuiElement::NestedWindow();
	m_questWindow->hide();
	GuiElement::Header *quest1 = new GuiElement::Header("Test the Quest");
	quest1->addElement(new GuiElement::Text("Quest description here bla bla bla"));
	GuiElement::Header *quest2 = new GuiElement::Header("Testwindow Questwindow");
	quest2->addElement(new GuiElement::Text("Quest description here too bla bla bla"));
	m_questWindow->addElement(quest1);
	m_questWindow->addElement(quest2);
	m_questWindow->setName("Quests");
	m_questWindow->setCollapsable(false);
	m_questWindow->setPosition(m_WINDOW_WIDTH - m_QUEST_WIDTH, (m_WINDOW_HEIGHT / 2) - (m_QUEST_HEIGHT / 2));
	m_questWindow->setSize(m_QUEST_WIDTH, m_QUEST_HEIGHT);
	m_hud->addNestedWindow(m_questWindow);

	m_inventoryWindow = new GuiElement::NestedWindow();
	m_inventoryWindow->hide();

	m_inventoryWindow->setName("Inventory");
	m_inventoryWindow->setCollapsable(false);
	m_inventoryWindow->setResizable(false);
	m_inventoryWindow->setPosition(m_WINDOW_WIDTH - m_QUEST_WIDTH, (m_WINDOW_HEIGHT / 2) - (m_QUEST_HEIGHT / 2));
	m_inventoryWindow->setSize(m_QUEST_WIDTH, m_QUEST_HEIGHT);
	m_hud->addNestedWindow(m_inventoryWindow);

	std::map<std::string, Texture*> *inventoryItems = new std::map<std::string, Texture*>();
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem1"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem2"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem3"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem4"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem5"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem6"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem7"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem8"), &bricks));
	inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem9"), &bricks));
	GuiElement::Inventory *inventory = new GuiElement::Inventory(inventoryItems, 6);
	m_inventoryWindow->addElement(inventory);
}

PlayerGUI::~PlayerGUI()
{

}

GUI* PlayerGUI::getHUD()
{
	return m_hud;
}

void PlayerGUI::update()
{
	hp = m_player->getHealth();
	hpMax = m_player->getHealth();
	exp = m_player->getExp();
	expMax = m_player->getLevelThreshold();
	level = m_player->getLvl();
}
