#pragma once
/* armor.h */
#ifndef ARMOR_HPP
#define ARMOR_HPP

#include <string>
#include "JsonBox.h"

#include "Item.h"

class EntityManager;

class Armor : public Item
{
public:

	int defense;

	// Constructors
	Armor(std::string id, std::string name, std::string description, int defense);
	Armor(std::string id, JsonBox::Value& v, EntityManager* mgr);

	// Load armor from the Json value
	void load(JsonBox::Value& v, EntityManager* mgr);
};

#endif /* ARMOR_HPP */