//
//  main.c
//  AdventureGame
//
//  Created by Jeff Huang on 2015-07-01.
//  Copyright (c) 2015 Jeff Huang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

static const char directionMain[15] = "DirectionMain";
static const char directionSide[15] = "DirectionSide";
bool playing = true;

typedef enum Direction{
    DirectionMain,
    DirectionSide,
}Direction;

typedef enum PathSegmentContents {
    PathSegmentContentsTreasure,
    PathSegmentContentsMonster,
    PathSegmentContentsNone,
} PathSegmentContents;

typedef struct PathSegment {
    PathSegmentContents contents;
    Direction direction;
    struct PathSegment *mainRoad;
    struct PathSegment *sideBranch;
} PathSegment;

#pragma mark - Game Path Creation

PathSegment *CreatePathSegment(PathSegmentContents contents) {
    PathSegment *path = malloc(sizeof(PathSegment));
    path->contents = contents;
    path->mainRoad = NULL;
    path->sideBranch = NULL;
    return path;
}

PathSegmentContents RandomContents() {
    int roll = rand() % 10;
    if (roll == 0) return PathSegmentContentsMonster;
    if (roll < 3) return PathSegmentContentsTreasure;
    return PathSegmentContentsNone;
}

PathSegment *GenerateAdventure() {
    srand((int)time(NULL));
    
    PathSegment *home = CreatePathSegment(PathSegmentContentsNone);
    
    PathSegment *leftBranchCursor = home; // primary
    PathSegment *rightBranchCursor = NULL;
    
    for (int i = 0; i < 100; i++) {
        
        if (leftBranchCursor != NULL) {
            // append to left branch
            leftBranchCursor->mainRoad = CreatePathSegment(RandomContents());
            leftBranchCursor = leftBranchCursor->mainRoad;
        }
        
        if (rightBranchCursor != NULL) {
            // append to right branch
            rightBranchCursor->sideBranch = CreatePathSegment(RandomContents());
            rightBranchCursor = rightBranchCursor->sideBranch;
        }
        
        if (leftBranchCursor && rightBranchCursor) {
            // if we're branched right now, maybe merge.
            if (rand() % 10 < 3) {
                rightBranchCursor->mainRoad = rightBranchCursor;
                rightBranchCursor = NULL;
            }
        } else {
            // if we're not branched right now, maybe split.
            if (rand() % 10 < 3) {
                rightBranchCursor = leftBranchCursor;
            }
        }
    }
    
    return home;
}

void PrintPathSegments(PathSegment *main, PathSegment *side) {
    if (!main) return;
    
    if (main->mainRoad && main->sideBranch) {
        printf("|\\");
    } else {
        if (main->mainRoad) {
            printf("|");
        }
        
        if (side) {
            if (side->sideBranch) {
                printf(" |");
            } else {
                printf("/");
            }
        }
    }
}

void PrintPath(PathSegment *pathStart) {
    
    PathSegment *mainPath = pathStart->mainRoad;
    PathSegment *sidePath = pathStart->sideBranch;
    
    while (mainPath->mainRoad != NULL) {
        PrintPathSegments(mainPath, sidePath);
        
        printf("\n");
        mainPath = mainPath->mainRoad;
        
        if (mainPath->sideBranch) {
            sidePath = mainPath->sideBranch;
        } else if (sidePath) {
            sidePath = sidePath->sideBranch;
        }
    }
}

void FreeAllPathSegments(PathSegment *segment) {
    // properly deallocate all segments along the path
    while(segment->mainRoad != NULL){
        if(segment->sideBranch !=NULL){
            free(segment->sideBranch);
        }
        free(segment->mainRoad);
        segment->mainRoad = segment->mainRoad->mainRoad;
    }
}

typedef struct Player{
    PathSegment *current;
    int steps;
    int health;
    int wealth;
} Player;

void printPlayerStatus(Player *p){
    char *direction;
    if(p->current->direction == DirectionMain)
        direction = directionMain;
    else
        direction = directionSide;
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

void pathPrompt(Player *p){
    if(p->current->sideBranch != NULL && p->current->direction == DirectionMain)
        if(p->current->sideBranch->mainRoad == NULL){
            char input[10];
            printf("Would you like to exit? Press Q to exit\n");
            scanf("%s", input);
            if(strstr(input, "Q"))
                playing = false;
            else
                printf("Open road ahead! type 'Y' to continue on main road\n");
        }
        else
            printf("Fork road! Type 'Y' to continue on main road anything else to take the side path\n");
    else if(p->current->sideBranch != NULL && p->current->direction == DirectionSide)
        printf("Haven't seen the road back to main! Type anything to continue on side road\n");
    else if(p->current->sideBranch == NULL && p->current->direction == DirectionSide){
        printf("Side road has merged back to main road, type 'Y' to continue on main road\n");
    }
    else
        printf("Open road ahead! type 'Y' to continue on main road\n");
}

int main(int argc, const char * argv[]) {
    
    Player player;
    PathSegment *path = GenerateAdventure();
    PrintPath(path);
    
    player.current = path;
    player.current->direction = DirectionMain;
    player.health = 100;
    
    
    
    char input[300];
    
    while (playing) {
        pathPrompt(&player);
        if(playing){
            scanf("%s", input);
            printf("input was %s\n", input);
            if(strstr(input,"Y"))
                movePlayer(&player,DirectionMain);
            else
                movePlayer(&player,DirectionSide);
            printPlayerStatus(&player);
        }
        
    }
    
    FreeAllPathSegments(path);
    
    return 0;
}
