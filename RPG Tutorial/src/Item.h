#pragma once
/* item.hpp */
#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include "JsonBox.h"

#include "Entity.h"

class EntityManager;

class Item : public Entity
{
public:

	// Name and description of the item
	std::string name;
	std::string description;

	// Constructors
	Item(std::string id, std::string name, std::string description);
	Item(std::string id, JsonBox::Value& v, EntityManager* mgr);

	// Load the item information from the JSON value
	virtual void load(JsonBox::Value& v, EntityManager* mgr);
};

#endif /* ITEM_HPP */