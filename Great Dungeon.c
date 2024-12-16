#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> //trap fonksiyonu için gerekli!!!

typedef struct Item {
    char *name;          
    struct Item *next;   
} Item;

typedef struct Creature {
    char *name;        
    int health;         
    int strength;      
    struct Room *room;  
} Creature;

typedef struct Room {
    char *description;       
    struct Room *north;         
    struct Room *south;     
    struct Room *west;      
    struct Room *east;      
    Item *items;             
    Creature *creature;   
    int isLocked;
    int lockCode;
    int hasTrap;
} Room;

typedef struct Player {
    char *name;
    int health;
    int strength;
    int inventory_capacity;
    Item *inventory;         
    Room *current_room;      
} Player;


void displayWelcomeMenu(Player* player); 
void info(Player* player);
void creatureInfo(Player* player);
void itemInfo(Player* player);
Room *create_room(const char *description);
Creature* create_creature(const char *name, int health, int strength, Room *room);
void free_room(Room *room);
Item *create_item(const char *name);
void free_item(Item *item);
void add_item(Room *room, const char *item_name);
Player *create_player(Room *starting_room);
void free_player(Player *player);
int trap();
void free_creature(Creature *creature);
void move(Player *player, const char *direction);
void look(Player *player);
void inventory(Player *player);
void pickup(Player *player, const char *item_name);
void drop(Player *player, const char *item_name);
void attack(Player *player);
void use(Player *player, const char *item_name);
void save(Player *player, const char *filepath);
void load(Player *player, const char *filepath);


int main() {
    Room *room1 = create_room("A medium-sized room, dark and abandoned.");
    Room *room2 = create_room("A small room to the north, surrounded by old stone walls.");
    Room *room3 = create_room("A large room, filled with spider webs.");
    Room *room4 = create_room("A dark, cold, and damp room.");
    Room *room5 = create_room("A large, ruined room with cracks in the walls.");
    Room *room6 = create_room("A dark, damp room with slippery floors.");
    Room *room7 = create_room("A narrow, long room with rotting wood in the corners.");
    Room *room8 = create_room("A cramped, dilapidated room with a passage between a few stone walls on the right.");

    room1->east = room4;
    room1->west = room7;
    room1->north = room2;
    room1->south = room6;
    room2->south = room1;
    room2->east = room3;
    room3->west = room2;
    room3->south = room4;
    room4->west = room1;
    room4->north = room3;
    room4->south = room5;
    room5->north = room4;
    room5->west = room6;
    room6->north = room1;
    room6->west = room8;
    room6->east = room5;
    room8->east = room6;
    room8->north = room7;
    room7->south = room8;
    room7->east = room1;
    
    room8->isLocked = 1;
    room8->lockCode = 1423;
    
    room3->hasTrap = 1;
    
    Player *player = create_player(room1);

    Creature *creature1 = create_creature("Stone Golem", 150, 5, room2);
    room2->creature = creature1;
    Creature *creature2 = create_creature("Skeloten Monster", 80, 20, room7);
    room7->creature = creature2;
    Creature *creature3 = create_creature("Shadow Nightmare", 60, 15, room4);
    room4->creature = creature3;
    Creature *creature4 = create_creature("Poisonous Spider", 70, 20, room6);
    room6->creature = creature4;

    add_item(room2, "Armor");
    add_item(room4, "Sword Of The Fallen King");
    add_item(room5, "Healing Poiton");
    add_item(room1, "Sword");
    add_item(room8, "Spell Book");
    add_item(room3, "Ancient Paper");
    
    
    displayWelcomeMenu(player);  

    char command[100];
    while (1) {
        printf("Command: ");
        fgets(command, 100, stdin);
    
       
        command[strcspn(command, "\n")] = 0;
    
        if (strncmp(command, "move ", 5) == 0) {
            move(player, command + 5);
        } else if (strncmp(command, "look", 4) == 0) {
            look(player);
        } else if (strncmp(command, "inventory", 9) == 0) {
            inventory(player);
        } else if (strncmp(command, "pickup ", 7) == 0) {
            pickup(player, command + 7);
        } else if (strncmp(command, "attack", 6) == 0) {
            attack(player);
        } else if (strncmp(command, "exit", 4) == 0) {
            break;
        } else if (strncmp(command, "drop ", 5) == 0) {
            drop(player, command + 5);
        } else if (strncmp(command, "save ", 5) == 0) {
            save(player, command + 5);  
        } else if (strncmp(command, "load ", 5) == 0) {
            load(player, command + 5);  
        } else if (strncmp(command, "info", 4) == 0) {
            info(player);
        } else if (strncmp(command, "creatureInfo", 12) == 0) {
            creatureInfo(player);
        } else if (strncmp(command, "itemInfo", 8) == 0) {
            itemInfo(player);
        } else {
            printf("Unknown command.\n");
        }
    }
    
    free_player(player);
    
    free_room(room1);
    free_room(room2);
    free_room(room3);
    free_room(room4);
    free_room(room5);
    free_room(room6);
    free_room(room7);
    free_room(room8);
    
    free_creature(creature1);
    free_creature(creature2);
    free_creature(creature3);
    free_creature(creature4);
    
    free_item(room2->items);
    free_item(room5->items);
    free_item(room1->items);
    free_item(room8->items);
    free_item(room3->items);
    free_item(room4->items);
    
    return 0;
}

void displayWelcomeMenu(Player* player) {
    printf("Welcome to the Dungeon!\n\n");
    printf("Character Information:\n");
    printf("Name: %s\n", player->name);  
    printf("Health: %d\n", player->health);
    printf("Strength: %d\n", player->strength);
    printf("Inventory Capacity: %d\n", player->inventory_capacity);
    
    int inventory_count = 0;
    if (player->inventory != NULL) {
        inventory_count = player->inventory_capacity;
    }
    printf("Items in Inventory: %d\n", inventory_count);
    printf("\nCommands:\n");
    printf("info - Character Information\n");
    printf("creatureInfo - Creature Information\n");
    printf("itemInfo - Item Information\n");
    printf("move <direction> - Move in directions (north, south, east, west)\n");
    printf("look - See the items and creatures in the room\n");
    printf("inventory - Check your inventory\n");
    printf("pickup <item> - Pick up an item\n");
    printf("drop <item> - Drop an item\n");
    printf("attack - Attack the creature\n");
    printf("save <filepath.txt> - Save the game\n");
    printf("load <filepath.txt> - Load the game\n");
    printf("exit - Exit the game\n");
    printf("\nEnter a command to start exploring the dungeon...\n");
}

void info(Player* player) {
    printf("Strength: %d\n", player->strength);
    printf("Health: %d\n", player->health);
    printf("Room: %s\n", player->current_room->description);
}

void creatureInfo(Player* player) {
    if(player->current_room->creature != NULL) {
        printf("Name: %s\n", player->current_room->creature->name);
        printf("Strength: %d\n", player->current_room->creature->strength);
        printf("Health: %d\n", player->current_room->creature->health);
    } else {
        printf("There is no creature in this room.\n");
    }
    
}

void itemInfo(Player* player) {
    if(player->current_room->items != NULL) {
        if (strcmp(player->current_room->items->name, "Armor") == 0) {
            printf("Name: Armor\n");
            printf("Gives +30 Health\n");
        } else if (strcmp(player->current_room->items->name, "Sword Of The Fallen King") == 0) {
            printf("Name: Sword Of The Fallen King\n");
            printf("Gives +10 Strength\n");
        } else if (strcmp(player->current_room->items->name, "Healing Poition") == 0) {
            printf("Name: Healing Poition\n");
            printf("It restore your health\n");
        } else if (strcmp(player->current_room->items->name, "Sword") == 0) {
            printf("Name: Sword\n");
            printf("Gives +5 Strength\n");
        } else if (strcmp(player->current_room->items->name, "Spell Book") == 0) {
            printf("Name: Spell Book\n");
            printf("Gives +10 Strength\n");
        } else if (strcmp(player->current_room->items->name, "Ancient Paper") == 0) {
            printf("Name: Ancient Paper\n");
            printf("An old piece of paper with a hidden password\n");
        } else {
            printf("There is no item in this room!!!");
        }
    } else {
        printf("There is no item in this room.\n");
    }
    
}

Room *create_room(const char *description) {
	Room *room = (Room *)malloc(sizeof(Room));
	room->description = strdup(description); 
	room->north = room->south = room->west = room->east = NULL;
	room->items = NULL;
	room->creature = NULL;
	room->creature = NULL;
	room->isLocked = 0;
	room->lockCode = 0;
	return room;
}

void free_room(Room *room) {
	if (!room) return;
	free(room->description);
	Item *current = room->items;
	while (current) {
		Item *next = current->next;
		free_item(current);
		current = next;
	}
	free(room);
}

Item *create_item(const char *name) {
	Item *item = (Item *)malloc(sizeof(Item));
	item->name = strdup(name); 
	item->next = NULL;
	return item;
}

void free_item(Item *item) {
	if (!item) return;
	free(item->name);
	free(item);
}

void add_item(Room *room, const char *item_name) {
    Item *item = create_item(item_name);
    item->next = room->items;
    room->items = item;
}

Player *create_player(Room *starting_room) {
	Player *player = (Player *)malloc(sizeof(Player));
	if(player == NULL) {
	    exit(1);
	}
	player->name = (char *)malloc(strlen("Warrior") + 1);
	if(player->name == NULL) {
	    free(player);
	    exit(1);
	}
	strcpy(player->name, "Warrior");
	
	player->health = 100;
	player->strength = 10;
	player->inventory_capacity = 5;
	player->inventory = NULL;
	player->current_room = starting_room;
	return player;
}

void free_player(Player *player) {
	if (!player) return;
	Item *current = player->inventory;
	while (current) {
		Item *next = current->next;
		free_item(current);
		current = next;
	}
	free(player);
}

Creature* create_creature(const char *name, int health, int strength, Room *room) {
    Creature *creature = (Creature *)malloc(sizeof(Creature));
    creature->name = strdup(name);
    creature->health = health;
    creature->strength = strength;
    creature->room = room;
    return creature;
}

void free_creature(Creature *creature) {
    if (creature != NULL) {
        free(creature->name);
        free(creature);
    }
}

void move(Player *player, const char *direction) {
    Room *room = NULL;
    
    if (strncmp(direction, "north", 5) == 0 && player->current_room->north) {
        room = player->current_room->north;
    } else if (strncmp(direction, "south", 5) == 0 && player->current_room->south) {
        room = player->current_room->south;
    } else if (strncmp(direction, "east", 4) == 0 && player->current_room->east) {
        room = player->current_room->east;
    } else if (strncmp(direction, "west", 4) == 0 && player->current_room->west) {
        room = player->current_room->west;
    }
    
    if(room != NULL) {
        if(room->isLocked) {
            printf("The door is locked! Please enter the code to proceed: ");
            int code;
            scanf("%d", &code);
            if(code == room->lockCode) {
                printf("The door opens!\n");
                player->current_room = room;
            } else {
                printf("Incorrect code. You can't pass.\n");
            } 
        } else if (room->hasTrap) {
            if (trap()) {
                player->current_room = room;
            } else {
                printf("You have been defeated by the trap.\n");
                exit(0);
            }
        } else {
            player->current_room = room;
            printf("You moved %s.\n", direction);
        }
    } else {
        printf("You can't go that way!\n");
    }
    
}

void look(Player *player) {
	printf("%s\n", player->current_room->description);
	if (player->current_room->creature != NULL) {
        printf("You see a creature: %s\n", player->current_room->creature->name);
    } else {
        printf("There are no creatures in the room.\n");
    }
    if (player->current_room->items != NULL) {
        printf("Items in the room:\n");
        Item *current = player->current_room->items;
        while (current) {
            printf("- %s\n", current->name);
            current = current->next;
        }
    } else {
        printf("There are no items in the room.\n");
    }
}

void inventory(Player *player) {
	printf("Your inventory contains:\n");
	Item *current = player->inventory;
	while (current) {
		printf("- %s\n", current->name);
		current = current->next;
	}
}

void pickup(Player *player, const char *item_name) {
    if (player->current_room->items == NULL) {
        printf("There are no items in this room.\n");
        return;
    }
    
    char clean_item_name[100];
    strncpy(clean_item_name, item_name, sizeof(clean_item_name) - 1);
    clean_item_name[strcspn(clean_item_name, "\n")] = '\0';

    int inventory_count = 0;
    Item *temp = player->inventory;
    while (temp != NULL) {
        inventory_count++;
        temp = temp->next;
    }
    if (inventory_count >= 5) {
        printf("Your inventory is full. Drop an item to pick this one up.\n");
        return;
    }
    
    Item *current = player->current_room->items;
    Item *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, clean_item_name) == 0) {
            
            if (prev == NULL) {
                player->current_room->items = current->next;
            } else {
                prev->next = current->next;
            }

            current->next = player->inventory; 
            player->inventory = current; 

            printf("You picked up %s.\n", clean_item_name);
            if (strcmp(clean_item_name, "Sword") == 0) {
                player->strength += 5;
                printf("Your strength increased by 5!\n");
            } else if (strcmp(clean_item_name, "Armor") == 0) {
                player->health += 30;
                printf("Your health increased by 30!\n");
            } else if (strcmp(clean_item_name, "Healing Potion") == 0) {
                player->health = 100; 
                printf("You have fully restored your health!\n");
            } else if (strcmp(clean_item_name, "Sword Of The Fallen King") == 0) {
                player->strength += 10;
                printf("Your strength increased by 10!\n");
            } else if (strcmp(clean_item_name, "Spell Book") == 0) {
                player->strength += 15;
                player->health += 20;
                printf("Your magical abilities improved. Strength increased by 15, health increased by 20!\n");
            } else if (strcmp(clean_item_name, "Ancient Paper") == 0) {
                printf("You found the Code to the Locked Shadow Rooms. The Code is: 1423\n");
            }
            player->inventory_capacity--; 
            return;
        }
        prev = current;
        current = current->next;
    }
    
    printf("There is no %s in this room.\n", clean_item_name);
}


void drop(Player *player, const char *item_name) {
    if (player->inventory == NULL) {
        printf("Your inventory is empty. Nothing to drop.\n");
        return;
    }
    char clean_item_name[100];
    strncpy(clean_item_name, item_name, sizeof(clean_item_name) - 1);
    clean_item_name[strcspn(clean_item_name, "\n")] = '\0';
    int item_count = 0;
    Item *current_room_item = player->current_room->items;
    while (current_room_item != NULL) {
        item_count++;
        current_room_item = current_room_item->next;
    }
    if (item_count >= 3) {
        printf("The room already has the maximum number of items (3). You cannot drop %s here.\n", clean_item_name);
        return;
    }
    Item *current = player->inventory;
    Item *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, clean_item_name) == 0) {
            if (prev == NULL) {
                player->inventory = current->next;
            } else {
                prev->next = current->next;
            }
            current->next = player->current_room->items;
            player->current_room->items = current;
            printf("You dropped %s in the room.\n", clean_item_name);
            player->inventory_capacity++;
            if (strcmp(clean_item_name, "Sword") == 0) {
                player->strength -= 5;
                printf("Your strength decreased by 5!\n");
            } else if (strcmp(clean_item_name, "Armor") == 0) {
                player->health -= 30;
                printf("Your health decreased by 30!\n");
            } else if (strcmp(clean_item_name, "Sword Of The Fallen King") == 0) {
                player->strength -= 10;
                printf("Your strength decreased by 10!\n");
            } else if (strcmp(clean_item_name, "Spell Book") == 0) {
                player->strength -= 15;
                player->health -= 20;
                printf("Your magical abilities decreased. Strength decreased by 15, health decreased by 20!\n");
            } else if (strcmp(clean_item_name, "Ancient Paper") == 0) {
                printf("You drop the Ancient Paper\n");
                break;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("You do not have %s in your inventory.\n", clean_item_name);
}

void attack(Player *player) {
    if (player->current_room->creature != NULL) {
        Creature *creature = player->current_room->creature;
        printf("You attack the %s!\n", creature->name);
        while (player->health > 0 && creature->health > 0) {
            creature->health -= player->strength;
            printf("You dealt %d damage to the %s.\n",
                player->strength, creature->name);
            if (creature->health <= 0) {
                printf("You defeated the %s!\n", creature->name);
                player->current_room->creature = NULL; 
                break; 
            }
            player->health -= creature->strength;
            printf("The %s attacks you back! You have %d health left.\n",
                creature->name, player->health);
            if (player->health <= 0) {
                printf("You have been defeated by the %s!\n", creature->name);
                exit(0);
            }
        }
    } else {
        printf("There is no creature to attack in this room.\n");
    }
}

int trap() {
    srand(time(NULL));
    int a = rand() % 10 + 1;
    int b = rand() % 10 + 1;
    int answer;
    printf("A trap is triggered! Solve this multiplication to escape: %d x %d\n", a, b);
    printf("You have 10 seconds to answer.\n");

    time_t start = time(NULL);
    time_t end = start;

    for (int i = 10; i > 0; i--) {
        printf("Time remaining: %d seconds\n", i);
        sleep(1); 
        end = time(NULL); 
        if (end - start > 10) {
            break;
        }
    }

    printf("Enter your answer: ");
    scanf("%d", &answer);
    end = time(NULL); 

    if (end - start > 10) {
        printf("Time is up! You failed to escape the trap.\n");
        return 0;
    } else if (answer == a * b) {
        printf("Correct! You escaped the trap.\n");
        return 1;
    } else {
        printf("Wrong answer! You failed to escape the trap.\n");
        return 0;
    }
}

void save(Player *player, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }
    fprintf(file, "%s\n", player->name);
    fprintf(file, "%d\n", player->health);
    fprintf(file, "%d\n", player->strength);
    fprintf(file, "%d\n", player->inventory_capacity);
    if (player->current_room != NULL) {
        fprintf(file, "%s\n", player->current_room->description);
    } 
    Item *current_item = player->inventory;
    while (current_item != NULL) {
        fprintf(file, "Item: %s\n", current_item->name);
        current_item = current_item->next;
    }
    Creature *creature = player->current_room->creature;
    if (creature != NULL) {
        fprintf(file, "Creature: %s\n", creature->name);
        fprintf(file, "%d\n", creature->health);
        fprintf(file, "%d\n", creature->strength);
    }
    fclose(file);
    printf("Game saved successfully to %s\n", filepath);
}

void load(Player *player, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error opening file for loading.\n");
        return;
    }

    char name[100];
    if (fscanf(file, "%s\n", name) != 1) {
        printf("Error reading player name.\n");
        fclose(file);
        return;
    }
    player->name = strdup(name);

    if (fscanf(file, "%d\n", &(player->health)) != 1 ||
        fscanf(file, "%d\n", &(player->strength)) != 1 ||
        fscanf(file, "%d\n", &(player->inventory_capacity)) != 1) {
        printf("Error reading player attributes.\n");
        fclose(file);
        return;
    }

    char room_description[200];
    if (fscanf(file, "%s\n", room_description) != 1) {
        printf("Error reading room description.\n");
        fclose(file);
        return;
    }
    Room *room = create_room(room_description);
    if (!room) {
        printf("Error creating room.\n");
        fclose(file);
        return;
    }
    player->current_room = room;

    char item_name[100];
    while (fscanf(file, "Item: %s\n", item_name) != EOF) {
        Item *new_item = create_item(item_name);
        if (!new_item) {
            printf("Error creating item: %s.\n", item_name);
            continue;
        }
        new_item->next = player->inventory;
        player->inventory = new_item;
    }

    char creature_name[100];
    int health, strength;
    if (fscanf(file, "Creature: %s\n", creature_name) == 1) {
        if (fscanf(file, "%d\n%d\n", &health, &strength) != 2) {
            printf("Error reading creature attributes.\n");
        } else {
            Creature *creature = create_creature(creature_name, health, strength, player->current_room);
            if (creature) {
                player->current_room->creature = creature;
            } else {
                printf("Error creating creature: %s.\n", creature_name);
            }
        }
    }

    fclose(file);
    printf("Game loaded successfully from %s\n", filepath);
}
