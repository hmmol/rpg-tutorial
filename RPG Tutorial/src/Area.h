#pragma once
/* area.h */
#ifndef AREA_HPP
#define AREA_HPP

#include <vector>
#include <string>
#include "JsonBox.h"

#include "Entity.h"
#include "Inventory.h"
#include "Dialogue.h"
#include "Creature.h"

class EntityManager;
class Door;

// Movement is achieved through the use of areas, which are contained
// units of space consisting of an inventory, a list of creatures and
// a dialogue
class Area : public Entity
{
public:

	// Dialogue is run whenever the area is entered
	Dialogue dialogue;

	// Items contained within the area. Not split into individual containers
	// for simplicity.
	Inventory items;

	// Links between rooms. Every door should have this as one of its area pointers
	std::vector<Door*> doors;

	// Creatures that reside in the room
	std::vector<Creature> creatures;

	// Constructors
	Area(std::string id, Dialogue dialogue, Inventory items, std::vector<Creature*> creatures);
	Area(std::string id, JsonBox::Value& v, EntityManager* mgr);

	// Load the area from a JSON value
	void load(JsonBox::Value& v, EntityManager* mgr);

	// Return a JSON object representing the area
	JsonBox::Object getJson();
};

#endif /* AREA_HPP */