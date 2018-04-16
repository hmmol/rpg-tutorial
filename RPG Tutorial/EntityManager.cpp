/* entity_manager.cpp */
#include "stdafx.h"
#include <string>
#include <map>

#include "Entity Manager.h"
#include "Area.h"
#include "Item.h"
#include "Door.h"
#include "Creature.h"
#include "Armor.h"
#include "Weapon.h"

template <class T>
// loads the JSON file as a JSON value (the entire set of data between the {}) 
// and then converts it into a JSON object that will have keys and corresponding values
void EntityManager::loadJson(std::string filePath)
{
	JsonBox::Value v;
	v.loadFromFile(filePath);

	JsonBox::Object o = v.getObject();
	for (auto entity : o)
	{
		std::string key = entity.first;
		this->data[key] = dynamic_cast<Entity*>(new T(key, entity.second, this));
	}
}

template <class T>
T* EntityManager::getEntity(std::string id)
{
	// The id prefix should match to the type T, so take the
	// first characters of the id up to the length of the
	// prefix and compare the two
	if (id.substr(0, entityToString<T>().size()) == entityToString<T>())
		return dynamic_cast<T*>(this->data.at(id));
	else
		return nullptr;
}

EntityManager::EntityManager() {}

EntityManager::~EntityManager()
{
	for (auto& entity : this->data)
	{
		delete entity.second;
	}
}

// Template specialization
template <> std::string entityToString<Item>() { return "item"; }
template <> std::string entityToString<Weapon>() { return "weapon"; }
template <> std::string entityToString<Armor>() { return "armor"; }
template <> std::string entityToString<Door>() { return "door"; }
template <> std::string entityToString<Area>() { return "area"; }
template <> std::string entityToString<Creature>() { return "creature"; }

// Template instantiations
template void EntityManager::loadJson<Item>(std::string);
template void EntityManager::loadJson<Weapon>(std::string);
template void EntityManager::loadJson<Armor>(std::string);
template void EntityManager::loadJson<Area>(std::string);
template void EntityManager::loadJson<Door>(std::string);
template void EntityManager::loadJson<Creature>(std::string);

template Item* EntityManager::getEntity<Item>(std::string);
template Weapon* EntityManager::getEntity<Weapon>(std::string);
template Armor* EntityManager::getEntity<Armor>(std::string);
template Door* EntityManager::getEntity<Door>(std::string);
template Area* EntityManager::getEntity<Area>(std::string);
template Creature* EntityManager::getEntity<Creature>(std::string);