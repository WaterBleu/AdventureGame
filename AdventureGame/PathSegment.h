//
//  PathSegment.h
//  AdventureGame
//
//  Created by Jeff Huang on 2015-07-03.
//  Copyright (c) 2015 Jeff Huang. All rights reserved.
//

#ifndef __AdventureGame__PathSegment__
#define __AdventureGame__PathSegment__

#include <stdio.h>

#endif /* defined(__AdventureGame__PathSegment__) */

typedef enum Direction{
    DirectionMain,
    DirectionSide,
} Direction;

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

PathSegment *CreatePathSegment(PathSegmentContents contents);
PathSegmentContents RandomContents();
PathSegment *GenerateAdventure();
void PrintPathSegments(PathSegment *main, PathSegment *side);
void FreeAllPathSegments(PathSegment *segment);