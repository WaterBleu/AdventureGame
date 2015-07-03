//
//  main.c
//  AdventureGame
//
//  Created by Jeff Huang on 2015-07-01.
//  Copyright (c) 2015 Jeff Huang. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Player.h"




bool playing = true;

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
