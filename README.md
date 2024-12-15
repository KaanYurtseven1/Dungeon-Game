Kaan Yurtseven - 23050151010 - CENG209 HW

**Game: Great Dungeon**

**Gameplay**

Great Dungeon is a dungeon exploration game where the player navigates different rooms, battles creatures, collects items, and overcomes various challenges. The goal is to survive the dangers of the dungeon and strengthen your inventory.

*A detailed code explanation can be found in the code_explanation.pdf file.*

**Game Mechanics**

Move - (Room Navigation): You can move between rooms in the north, south, east, and west directions.

Pickup - (Item Collection): You can collect useful items from rooms and add them to your inventory.

Drop - (Item Dropping): You can drop unnecessary items from your inventory.

Attack - (Creature Combat): You must fight creatures that you encounter in order to defeat them.

Traps - Traps: You may encounter traps in the dungeon as you progress. You must think quickly to avoid them.

Save - (Saving) and Load - (Loading): You can save your progress and continue from where you left off at any time.

**Code Structure**

The game is designed in a modular way and consists of several core components:

1 - Player

The player's name, health, strength, and inventory are defined within this structure. The current room the player is in is also stored here.

2 - Room
The rooms that make up the dungeon.

Each room may contain creatures and items.

Rooms are interconnected, and these connections form the map of the game.

3 - Items
Various objects that can be collected and used.

Items can be used within the limits of the inventory capacity.

4 - Creatures
The enemies that the player encounters.

Each creature has attributes like health and strength.

5 - Traps
Puzzles and obstacles in the dungeon that hinder your progress. You need to make quick decisions to overcome them.

**Game Logic**

1) Move
Direction commands are used to move between rooms. For example:

In-game usage: move north --> Moves the player to the northern room.

2) Attack
When you encounter a creature, you can attack with the attack command:

In-game usage: attack

The player deals damage to the creature.

The creature can also attack the player.

The battle continues until one party's health reaches zero.

3) Inventory Management
You can collect items found in a room with the pickup <itemname> command. You can also drop items from your inventory with the drop <itemname> command.

In-game usage: pickup Sword --> Sword is added to the inventory. In-game usage: drop Sword --> Sword is dropped from the inventory.

4) Look
This command allows you to view the description of the room you are in, the creatures, and the items in the room.

In-game usage: look

5) Inventory
This command allows you to view the items in your inventory.

In-game usage: inventory

6) info - creatureInfo - itemInfo
The info command shows the player's current health, strength, and current room.
In-game usage: info

The creatureInfo command shows the name, health, and strength of the creature in the current room.
In-game usage: creatureInfo

The itemInfo command shows the names and functions of the items in the current room.
In-game usage: itemInfo

7) Save and Load - (Saving and Loading)
Save - (Saving): The game progress is saved to a file.

Load - (Loading): The saved progress can later be loaded to continue the game.

In-game usage: save <filepath> --> The game is saved to the filepath file. In-game usage: load <filepath> --> The filepath file is loaded into the game.

8) Exit
The exit command ends the game.

In-game usage: exit

**Developer Notes**

The code structure is designed with readability in mind.

Necessary precautions have been taken for potential errors during saving and loading processes.

Combat, traps, and inventory mechanics have been optimized.

A flexible structure has been used to facilitate the addition of new features in the future.
