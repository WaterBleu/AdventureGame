//
//  PathSegment.c
//  AdventureGame
//
//  Created by Jeff Huang on 2015-07-03.
//  Copyright (c) 2015 Jeff Huang. All rights reserved.
//

#include "PathSegment.h"
#include <time.h>
#include <stdlib.h>



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


