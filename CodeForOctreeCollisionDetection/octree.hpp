//
//  octree.hpp
//  OctreeCollisionDetection
//
//  Created by Ydy on 16/3/31.
//  Copyright © 2016年 Ydy. All rights reserved.
//

#ifndef octree_hpp
#define octree_hpp

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <set>

#include "vec3f.h"

using namespace std;

const float GRAVITY = 8.0f;
const float BOX_SIZE = 12.0f;

const float TIME_BETWEEN_UPDATES = 0.01f;
const int TIMER_MS = 25;

const int MAX_OCTREE_DEPTH = 6;
const int MIN_BALLS_PER_OCTREE = 3;
const int MAX_BALLS_PER_OCTREE = 6;

//float RandomFloat();
inline float RandomFloat()
{
    return (float)rand() / ((float)RAND_MAX + 1);
}
struct Ball
{
    Vec3f velocity;
    Vec3f position;
    float radius;
    Vec3f color;
};

enum Wall
{
    WALL_LEFT,
    WALL_RIGHT,
    WALL_FAR,
    WALL_NEAR,
    WALL_TOP,
    WALL_BOTTOM
};

struct BallPair
{
    Ball *ball1;
    Ball *ball2;
};

struct BallWallPair
{
    Ball *ball;
    Wall wall;
};

class Octree
{
private:
    Vec3f corner1; //the lower_left_far corner of the cube
    Vec3f corner2; //the upper_right_near corner of the cube;
    Vec3f center;  //the middle of the cube
    
    Octree *children[2][2][2];
    //children[0][*][*] are the children with x coordinates ranging from minX to centerX
    //children[1][*][*] are the children with x coordinates ranging from centerX to maxX.
    //similarly for the other two dimensions of this array
    
    bool has_children;
    set<Ball *> balls;
    int depth;
    int num_balls;
    
    //Adds a ball to or removes one from the children of this
    void FileBall(Ball *ball, Vec3f pos, bool add_ball);
    
    //Creates children of this, and moves the balls in this to the children
    void CreateChildren();
    
    //Destroys the children of this, and moves all balls in its descendants
    //to the "balls" set
    void DestroyChildren();
    
    //Adds all balls in this or one of its descendants to the specified set
    void CollectBalls(set<Ball*> &bs);
    
    //Removes the specified ball at the indicated position
    void Remove(Ball *ball, Vec3f pos);
    
    void PotentialBallWallCollisions(vector<BallWallPair> &cs, Wall w, char coord, int dir);
    
public:
    //c1(minX, minY, minZ)
    //c2(maxX, maxY, maxZ)
    Octree(Vec3f c1, Vec3f c2, int _depth);
    
    ~Octree();
    
    //Adds a ball to this
    void Add(Ball *ball);
    
    //Removes a ball from this
    void Remove(Ball* ball);
    
    //Changes the position of a ball in this from oldPos to ball->pos
    void MoveBall(Ball* ball, Vec3f old_pos);
    
    //Adds potential ball-wall collisions to the specified set
    void PotentialBallWallCollisions(vector<BallWallPair> &collisions);
    
    //Adds potential ball-ball collisions to the specified set
    void PotentialBallBallCollisions(vector<BallPair> &collisions);
};
void PotentialBallBallCollisions(vector<BallPair> &potentialCollisions,
                                 vector<Ball*> &balls, Octree* octree);
void PotentialBallWallCollisions(vector<BallWallPair> &potentialCollisions,
                                 vector<Ball*> &balls, Octree* octree);

//Moves all of the balls by their velocity times dt
void MoveBalls(vector<Ball*> &balls, Octree* octree, float dt);

//Decreases the y coordinate of the velocity of each ball by GRAVITY *
//TIME_BETWEEN_UPDATES
void ApplyGravity(vector<Ball*> &balls);


//Returns whether two balls are colliding
bool TestBallBallCollision(Ball* b1, Ball* b2);

//Handles all ball-ball collisions
void HandleBallBallCollisions(vector<Ball*> &balls, Octree* octree);


//Returns the direction from the origin to the wall
Vec3f WallDirection(Wall wall);

//Returns whether a ball and a wall are colliding
bool TestBallWallCollision(Ball* ball, Wall wall);

//Handles all ball-wall collisions
void HandleBallWallCollisions(vector<Ball*> &balls, Octree* octree);

//Applies gravity and handles all collisions.  Should be called every
//TIME_BETWEEN_UPDATES seconds.
void PerformUpdate(vector<Ball*> &balls, Octree* octree);

//Advances the state of the balls by t.  time_until_update is the amount of time
//until the next call to performUpdate.
void Advance(vector<Ball*> &balls, Octree* octree, float t, float &time_until_update);
#endif /* octree_hpp */
