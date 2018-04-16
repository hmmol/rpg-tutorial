#pragma once
#include "stdafx.h"
#include <string>
#include "JsonBox.h"

#include "Weapon.h"
#include "Item.h"
#include "Entity Manager.h"

Weapon::Weapon(std::string id, std::string name, std::string description, int damage) : Item(id, name, description)
{
	this->damage = damage;
}

Weapon::Weapon(std::string id, JsonBox::Value& v, EntityManager* mgr) : Item(id, v, mgr)
{
	this->load(v, mgr);
}

void Weapon::load(JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->damage = o["damage"].getInteger();

	return;
}