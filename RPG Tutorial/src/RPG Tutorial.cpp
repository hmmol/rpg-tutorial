// RPG Tutorial.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include "JsonBox.h"

#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Entity Manager.h"
#include "Door.h"
#include "Area.h"
#include "Creature.h"
#include "Player.h"
#include "Dialogue.h"
#include "Battle.h"

#include "RPG Tutorial.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RPGTUTORIAL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RPGTUTORIAL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RPGTUTORIAL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RPGTUTORIAL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// New character menu
Player startGame();

// Character information menu
void dialogueMenu(Player& player);

// Keeps track of items, weapons, creatures, etc.
EntityManager entityManager;

int main()
{
	// Load the entities
	entityManager.loadJson<Item>("Items.json");
	entityManager.loadJson<Weapon>("Weapon.json");
	entityManager.loadJson<Armor>("Armor.json");
	entityManager.loadJson<Creature>("Creatures.json");
	entityManager.loadJson<Door>("Doors.json");
	entityManager.loadJson<Area>("Areas.json");

	// Seed the random number generator with the system time, so the
	// random numbers produced by rand() will be different each time
	// std::srand(std::time(nullptr));
	srand(time(NULL));

	Player player = startGame();

	// Set the curent area to be the first in the atlas,
	// placing the player there upon game start
	player.currentArea = "area_01";

	// Play the game until a function breaks and closes it
	while (1)
	{
		// Mark the current player as visited
		player.visitedAreas.insert(player.currentArea);

		// Pointer to the current area for convenience
		Area* areaPtr = player.getAreaPtr(&entityManager);

		// Autosave the game
		player.save(&entityManager);

		// If the area has any creatures in it, start a battle with them
		if (areaPtr->creatures.size() > 0)
		{
			// Create a vector of pointers to the creatures in the area
			std::vector<Creature*> combatants;
			std::cout << "You are attacked by ";
			for (int i = 0; i < areaPtr->creatures.size(); ++i)
			{
				Creature* c = &(areaPtr->creatures[i]);
				combatants.push_back(c);
				std::cout << c->name << (i == areaPtr->creatures.size() - 1 ? "!\n" : ", ");
			}

			// Add the player to the combat vector
			combatants.push_back(&player);
			// Run th ebattle
			Battle battle(combatants);
			battle.run();

			// If the player is still alive, grant them some experience, assuming
			// that every creature was killed
			if (player.hp > 0)
			{
				// Or use std::accumulate, but that requires an additional header
				unsigned int xp = 0;
				for (auto creature : areaPtr->creatures) xp += creature.xp;
				std::cout << "You gained " << xp << " experience!\n";
				player.xp += xp;
				// Remove the creatures from the area
				areaPtr->creatures.clear();
				// Restart the loop to force a save, then the game will carry on
				// as usual
				continue;
			}	

			// Otherwise player is dead, so end the program
			else
			{
				std::cout << "\t----YOU DIED----\n	Game Over\n";
				return 0;
			}
		}

		// Add the search and movement options to the dialogue
		Dialogue roomOptions = areaPtr->dialogue;
		for (auto door : areaPtr->doors)
		{
			roomOptions.addChoice("Go through the " + door->description);
		}

		roomOptions.addChoice("Search");

		// Activate the current area's dialogue
		int result = roomOptions.activate();

		if (result == 0)
		{
			dialogueMenu(player);
		}

		else if (result <= areaPtr->dialogue.size())
		{
			// Add more events here
		}

		else if (result < roomOptions.size())
		{
			Door* door = areaPtr->doors.at(result-areaPtr->dialogue.size() - 1);
			int flag = player.traverse(door);

			switch (flag)
			{
			default:
			case 0:
				std::cout << "The " << door->description << " is locked." << std::endl;
				break;
			case 1:
				std::cout << "You unlock the " << door->description << " and go through it." << std::endl;
				break;
			case 2:
				std::cout << "You go through the " << door->description << "." << std::endl;
				break;
			}
		}

		else
		{
			std::cout << "You find:" << std::endl;
			areaPtr->items.print();
			player.inventory.merge(&(areaPtr->items));
			areaPtr->items.clear();
		}
	}

	return 0;
}

Player startGame()
{
	// Ask for a name and a class
	// Name does not use a dialogue since dialogues only request options,
	// not string input. Could be generalized into its own TextInput
	// class, but not really necessary.
	std::cout << "What is your name?" << std::endl;
	std::string name;
	std::cin >> name;

	// Check for existence then open using JsonBox if it exists
	std::ifstream f((name + ".json").c_str());
	if (f.good())
	{
		f.close();

		// Load the player
		JsonBox::Value saveData;
		JsonBox::Value areaData;
		saveData.loadFromFile(name + ".json");
		areaData.loadFromFile(name + "_areas.json");
		Player player = Player(saveData, areaData, &entityManager);

		// Return the player
		return player;
	}

	else
	{
		f.close();
		int result = Dialogue(
			"Choose your class",
			{ "Fighter", "Rogue" }).activate();
		
		switch (result)
		{
			// Fighter class favors strength
		case 1:
			return Player(name, 15, 5, 4, 1.0 / 64.0, 0, 1, "Fighter");

			// Rogue class favors agility
		case 2:
			return Player(name, 15, 4, 5, 1.0 / 64.0, 0, 1, "Rogue");

			// Default case that should never happen
		default:
			return Player(name, 15, 4, 4, 1.0 / 64.0, 0, 1, "Adventurer");
		}
	}
}

void dialogueMenu(Player& player)
{
	// Output the menu
	int result = Dialogue(
		"Menu\n====",
		{ "Items", "Equipment", "Character" }).activate();

	switch (result)
	{
		// Print the items that the player owns
	case 1:
		std::cout << "Items\n====";
		player.inventory.print();
		std::cout << "--------------\n";
		break;
		// Print the equipment that the player is wearing (if they
		// are wearing anything) and then ask if they want to equip
		// a weapon or some armor
	case 2:
	{
		std::cout << "Equipment\n=========\n";
		std::cout << "Armor: "
			<< (player.equippedArmor != nullptr ?
				player.equippedArmor->name : "Nothing")
			<< std::endl;
		std::cout << "Weapon: "
			<< (player.equippedWeapon != nullptr ?
				player.equippedWeapon->name : "Nothing")
			<< std::endl;

		int result2 = Dialogue(
			"",
			{ "Equip Armor", "Equip Weapon", "Close" }).activate();

		// Equipping armor
		if (result2 == 1)
		{
			int userInput = 0;

			// Cannot equip armor if they do not have any
			// Print a list of the armor and retrieve the amount
			// of armor in one go
			int numItems = player.inventory.print<Armor>(true);

			if (numItems == 0)
				break;

			while (!userInput)
			{
				// Choose a piece of armor to equip
				std::cout << "Equip which item?" << std::endl;
				std::cin >> userInput;
				// Equipment is numbered but is stored in a list, 
				// so the number must be converted to a list element
				if (userInput >= 1 && userInput <= numItems)
				{
					player.equipArmor(player.inventory.get<Armor>(userInput - 1));
				}
			}
		}

		std::cout << "----------------\n";
		break;
	}

	// Output the character information, including name, class (if
	// they have one), stats, level, and experience
	case 3:
		std::cout << "Character\n=========\n";
		std::cout << player.name;
		if (player.className != "")
			std::cout << " the " << player.className;
		std::cout << std::endl;

		std::cout << "Health: " << player.hp << " / " << player.maxHp << std::endl;
		std::cout << "Strength: " << player.strength << std::endl;
		std::cout << "Agility: " << player.agility << std::endl;
		std::cout << "Level: " << player.level << " (" << player.xp;
		std::cout << " / " << player.xpToLevel(player.level + 1) << ")" << std::endl;
		std::cout << "----------------\n";
		break;
	default:
		break;
	}

	return;
}