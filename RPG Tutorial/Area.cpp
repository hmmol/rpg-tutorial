#pragma once
/* area.cpp */
#include "stdafx.h"
#include <vector>
#include <string>
#include "JsonBox.h"

#include "Area.h"
#include "Door.h"
#include "Entity.h"
#include "Inventory.h"
#include "Dialogue.h"
#include "Entity Manager.h"

Area::Area(std::string id, Dialogue dialogue, Inventory items,
	std::vector<Creature*> creatures) : Entity(id)
{
	this->dialogue = dialogue;
	this->items = items;

	for (auto creature : creatures)
	{
		this->creatures.push_back(*creature);
	}
}

Area::Area(std::string id, JsonBox::Value& v, EntityManager* mgr) : Entity(id)
{
	this->load(v, mgr);
}

void Area::load(JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();

	// Build the dialogue
	// This is an optional parameter because it will not be saved
	// when the area is modified
	if (o.find("dialogue") != o.end())
		this->dialogue = Dialogue(o["dialogue"]);

	// Build the inventory
	this->items = Inventory(o["inventory"], mgr);

	// Build creatures list
	this->creatures.clear();
	for (auto creature : o["creatures"].getArray())
	{
		// Create a new creature instance identical to the version
		// in the entity manager
		Creature c(*mgr->getEntity<Creature>(creature.getString()));
		this->creatures.push_back(c);
	}

	// Attach doors
	if (o.find("doors") != o.end())
	{
		this->doors.clear();

		for (auto door : o["doors"].getArray())
		{
			Door* d = nullptr;

			// Each door is either an array of the type [id, locked] or
			// a single id string
			if (door.isString())
			{
				d = mgr->getEntity<Door>(door.getString());
			}
			else
			{
				d = mgr->getEntity<Door>(door.getArray()[0].getString());
				d->locked = door.getArray()[1].getInteger();
			}

			this->doors.push_back(d);
		}
	}

	return;
}

JsonBox::Object Area::getJson()
{
	JsonBox::Object o;
	// We don't need to save the dialogue because it doesn't change

	// Save the inventory
	o["inventory"] = this->items.getJson();

	// Save the creatures
	JsonBox::Array a;
	for (auto creature : this->creatures)
	{
		a.push_back(JsonBox::Value(creature.id));
	}

	o["creatures"] = a;

	// Save the doors
	a.clear();
	for (auto door : this->doors)
	{
		JsonBox::Array d;
		d.push_back(door->id);
		d.push_back(door->locked);
		a.push_back(d);
	}

	o["doors"] = a;

	return o;
}