//
//  Player.h
//  AdventureGame
//
//  Created by Jeff Huang on 2015-07-03.
//  Copyright (c) 2015 Jeff Huang. All rights reserved.
//

#ifndef __AdventureGame__Player__
#define __AdventureGame__Player__

#include <stdio.h>
#include "PathSegment.h"
#endif /* defined(__AdventureGame__Player__) */

typedef struct Player{
    PathSegment *current;
    int steps;
    int health;
    int wealth;
} Player;

void printPlayerStatus(Player *p);
void updatePlayer(Player *p);
void movePlayer(Player *p, Direction d);