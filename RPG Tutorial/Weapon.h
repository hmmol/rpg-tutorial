#pragma once
/* weapon.h */
#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include "JsonBox.h"

#include "Item.h"

class EntityManager;

class Weapon : public Item
{
public:

	int damage;

	// Constructors
	Weapon(std::string id, std::string name, std::string description, int damage);
	Weapon(std::string id, JsonBox::Value& v, EntityManager* mgr);

	// Load weapon from the Json value
	void load(JsonBox::Value& v, EntityManager* mgr);
};

#endif /* WEAPON_HPP */