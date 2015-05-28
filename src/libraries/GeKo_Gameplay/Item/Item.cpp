#include "GeKo_Gameplay/Item/Item.h"

Item::Item(int id){
	m_id = id;
	m_name = "";
	m_typeId = ItemType::DEFAULTITEM;
}

Item::Item(){
}

Item::~Item(){
}

void Item::setId(int id){
	m_id = id;
}

void Item::setName(std::string name){
	m_name = name;
}
void Item::setTypeId(ItemType id){
	m_typeId = id;
}
ItemType Item::getTypeId(){
	return m_typeId;
}