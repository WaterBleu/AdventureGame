//
//  Player.c
//  AdventureGame
//
//  Created by Jeff Huang on 2015-07-03.
//  Copyright (c) 2015 Jeff Huang. All rights reserved.
//

#include "Player.h"

void printPlayerStatus(Player *p){
    char *direction;
    if(p->current->direction == DirectionMain)
        direction = "directionMain";
    else
        direction = "DirectionSide";
    printf("Has traveled %d segments, currently on %s current health: %d current wealth: %d\n",p->steps ,direction , p->health, p->wealth);
}

void updatePlayer(Player *p){
    if(p->current->contents == PathSegmentContentsMonster){
        printf("OMG! A Monster, I'm hit!\n");
        p->health-=5;
    }
    else if(p->current->contents == PathSegmentContentsTreasure){
        printf("Hey! I found gold!!\n");
        p->wealth+=5;
    }
}

void movePlayer(Player *p, Direction d){
    if(d == DirectionMain && p->current->mainRoad != NULL){
        p->current = p->current->mainRoad;
        p->current->direction = DirectionMain;
        p->steps++;
        updatePlayer(p);
    }
    else if(d == DirectionSide && p->current->sideBranch != NULL && p->current->direction == DirectionMain){
        p->current = p->current->sideBranch;
        p->current->direction = DirectionSide;
        p->steps++;
        updatePlayer(p);
    }
    else if(d == DirectionSide && p->current->sideBranch != NULL && p->current->direction == DirectionSide){
        p->current = p->current->sideBranch;
        if(p->current->sideBranch == NULL)
            p->current->direction = DirectionMain;
        p->steps++;
        updatePlayer(p);
    }
    else
        printf("Invalid Selection\n");
}