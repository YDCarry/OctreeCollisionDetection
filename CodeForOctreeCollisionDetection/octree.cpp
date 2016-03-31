//
//  octree.cpp
//  OctreeCollisionDetection
//
//  Created by Ydy on 16/3/31.
//  Copyright © 2016年 Ydy. All rights reserved.
//

#include "octree.hpp"
/*
float RandomFloat()
{
    return (float)rand() / ((float)RAND_MAX + 1);
}
 */

Octree::Octree(Vec3f c1, Vec3f c2, int _depth)
{
    corner1 = c1;
    corner2 = c2;
    center = (c1+c2) / 2;
    depth = _depth;
    num_balls = 0;
    has_children = false;
}

Octree::~Octree()
{
    if(has_children)
    {
        DestroyChildren();
    }
}

void Octree::Add(Ball *ball)
{
    num_balls++;
    if(!has_children && depth < MAX_OCTREE_DEPTH && num_balls > MAX_BALLS_PER_OCTREE)
    {
        CreateChildren();
    }
    
    if(has_children)
    {
        FileBall(ball, ball->position, true);
    }
    else
    {
        balls.insert(ball);
    }
}

void Octree::Remove(Ball* ball)
{
    Remove(ball, ball->position);
}

void Octree::MoveBall(Ball* ball, Vec3f old_pos)
{
    Remove(ball, old_pos);
    Add(ball);
}

void Octree::FileBall(Ball *ball, Vec3f pos, bool add_ball)
{
    for(int x=0; x<2; x++)
    {
        if(x == 0)
        {
            if(pos[0] - ball->radius > center[0])
            {
                continue;
            }
        }
        else if(pos[0] + ball->radius < center[0])
        {
            continue;
        }
        
        for(int y=0; y<2; y++)
        {
            if(y == 0)
            {
                if(pos[1] - ball->radius > center[1])
                {
                    continue;
                }
            }
            else if(pos[1] + ball->radius < center[1])
            {
                continue;
            }
            
            for(int z=0; z<2; z++)
            {
                if(z == 0)
                {
                    if(pos[2] - ball->radius > center[2])
                    {
                        continue;
                    }
                }
                else if(pos[2] + ball->radius < center[2])
                {
                    continue;
                }
                
                if(add_ball)
                {
                    children[x][y][z]->Add(ball);
                }
                else
                {
                    children[x][y][z]->Remove(ball, pos);
                }
            }
        }
    }
}


void Octree::CreateChildren()
{
    for(int x=0; x<2; x++)
    {
        float minX, maxX;
        if(x == 0)
        {
            minX = corner1[0];
            maxX = center[0];
        }
        else
        {
            minX = center[0];
            maxX = corner2[0];
        }
        
        for(int y=0; y<2; y++)
        {
            float minY, maxY;
            if(y == 0)
            {
                minX = corner1[1];
                maxX = center[1];
            }
            else
            {
                minX = center[1];
                maxX = corner2[1];
            }
            
            for(int z=0; z<2; z++)
            {
                float minZ, maxZ;
                if(z == 0)
                {
                    minX = corner1[2];
                    maxX = center[2];
                }
                else
                {
                    minX = center[2];
                    maxX = corner2[2];
                }
                
                children[x][y][z] = new Octree(Vec3f(minX, minY, minZ),
                                               Vec3f(maxX, maxY, maxZ),
                                               depth+1);
            }
        }
    }
    
    for(set<Ball*>::iterator it=balls.begin(); it!=balls.end(); it++)
    {
        Ball* ball = *it;
        FileBall(ball, ball->position, true);
    }
    balls.clear();
    has_children = true;
}

void Octree::CollectBalls(set<Ball *> &bs)
{
    if(has_children)
    {
        for(int x=0; x<2; x++)
        {
            for(int y=0; y<2; y++)
            {
                for(int z=0; z<2; z++)
                {
                    children[x][y][z]->CollectBalls(bs);
                }
            }
        }
    }
    else
    {
        for(set<Ball*>::iterator it=balls.begin(); it!=balls.end(); it++)
        {
            bs.insert(*it);
        }
    }
}

void Octree::DestroyChildren()
{
    CollectBalls(balls);
    
    for(int x=0; x<2; x++)
    {
        for(int y=0; y<2; y++)
        {
            for(int z=0; z<2; z++)
            {
                delete children[x][y][z];
            }
        }
    }
    
    has_children = false;
}

void Octree::Remove(Ball *ball, Vec3f pos)
{
    num_balls--;
    
    if(has_children && num_balls < MIN_BALLS_PER_OCTREE)
    {
        DestroyChildren();
    }
    
    if(has_children)
    {
        FileBall(ball, pos, false);
    }
    else
    {
        balls.erase(ball);
    }
}


void Octree::PotentialBallWallCollisions(vector<BallWallPair> &cs, Wall w, char coord, int dir)
{
    if(has_children)
    {
        for(int dir2=0; dir2<2; dir2++)
        {
            for(int dir3=0; dir3<2; dir3++)
            {
                Octree *child = NULL;
                switch (coord)
                {
                    case 'x':
                        child = children[dir][dir2][dir3];
                        break;
                    case 'y':
                        child = children[dir2][dir][dir3];
                        break;
                    case 'z':
                        child = children[dir2][dir3][dir];
                        break;
                    default:
                        break;
                }
                
                child->PotentialBallWallCollisions(cs, w, coord, dir);
            }
        }
    }
    else
    {
        for(set<Ball *>::iterator it=balls.begin(); it!=balls.end(); it++)
        {
            BallWallPair bwp;
            bwp.wall = w;
            bwp.ball = *it;
            cs.push_back(bwp);
        }
    }
}

void Octree::PotentialBallWallCollisions(vector<BallWallPair> &collisions)
{
    PotentialBallWallCollisions(collisions, WALL_LEFT, 'x', 0);
    PotentialBallWallCollisions(collisions, WALL_RIGHT, 'x', 1);
    PotentialBallWallCollisions(collisions, WALL_BOTTOM, 'y', 0);
    PotentialBallWallCollisions(collisions, WALL_TOP, 'y', 1);
    PotentialBallWallCollisions(collisions, WALL_FAR, 'z', 0);
    PotentialBallWallCollisions(collisions, WALL_NEAR, 'z', 1);
}

void Octree::PotentialBallBallCollisions(vector<BallPair> &collisions)
{
    if(has_children)
    {
        for(int x=0; x<2; x++)
        {
            for(int y=0; y<2; y++)
            {
                for(int z=0; z<2; z++)
                {
                    children[x][y][z]->PotentialBallBallCollisions(collisions);
                }
            }
        }
    }
    else
    {
        for(set<Ball *>::iterator it=balls.begin(); it!=balls.end(); it++)
        {
            for(set<Ball *>::iterator it2=balls.begin(); it2!=balls.end(); it2++)
            {
                if(*it < *it2)
                {
                    BallPair bp;
                    bp.ball1 = *it;
                    bp.ball2 = *it2;
                    collisions.push_back(bp);
                }
            }
            
        }
    }
}

void PotentialBallBallCollisions(vector<BallPair> &potentialCollisions,
                                 vector<Ball*> &balls, Octree* octree)
{
    octree->PotentialBallBallCollisions(potentialCollisions);
}

void PotentialBallWallCollisions(vector<BallWallPair> &potentialCollisions,
                                 vector<Ball*> &balls, Octree* octree)
{
    octree->PotentialBallWallCollisions(potentialCollisions);
}

void MoveBalls(vector<Ball*> &balls, Octree* octree, float dt)
{
    for(int i=0; i<balls.size(); i++)
    {
        Ball *ball = balls[i];
        Vec3f old_position = ball->position;
        ball->position += ball->velocity*dt;
        octree->MoveBall(ball, old_position);
    }
}

void ApplyGravity(vector<Ball*> &balls)
{
    for(int i=0; i<balls.size(); i++)
    {
        Ball* ball = balls[i];
        ball->velocity -= Vec3f(0, GRAVITY * TIME_BETWEEN_UPDATES, 0);
    }
}

bool TestBallBallCollision(Ball* b1, Ball* b2)
{
    float r = b1->radius + b2->radius;
    if ((b1->position - b2->position).MagnitudeSquared() < r * r)
    {
        Vec3f net_velocity = b1->velocity - b2->velocity;
        Vec3f displacement = b1->position - b2->position;
        return net_velocity.Dot(displacement) < 0;
    }
    else
        return false;
}

void HandleBallBallCollisions(vector<Ball*> &balls, Octree* octree)
{
    vector<BallPair> bps;
    PotentialBallBallCollisions(bps, balls, octree);
    for(unsigned int i = 0; i < bps.size(); i++)
    {
        BallPair bp = bps[i];
        
        Ball* b1 = bp.ball1;
        Ball* b2 = bp.ball2;
        if (TestBallBallCollision(b1, b2))
        {
            Vec3f displacement = (b1->position - b2->position).Normalize();
            b1->velocity -= 2 * displacement * b1->velocity.Dot(displacement);
            b2->velocity -= 2 * displacement * b2->velocity.Dot(displacement);
        }
    }
}

Vec3f WallDirection(Wall wall)
{
    switch (wall)
    {
        case WALL_LEFT:
            return Vec3f(-1, 0, 0);
        case WALL_RIGHT:
            return Vec3f(1, 0, 0);
        case WALL_FAR:
            return Vec3f(0, 0, -1);
        case WALL_NEAR:
            return Vec3f(0, 0, 1);
        case WALL_TOP:
            return Vec3f(0, 1, 0);
        case WALL_BOTTOM:
            return Vec3f(0, -1, 0);
        default:
            return Vec3f(0, 0, 0);
    }
}

bool TestBallWallCollision(Ball* ball, Wall wall)
{
    Vec3f dir = WallDirection(wall);
    return ball->position.Dot(dir) + ball->radius > BOX_SIZE / 2 &&
    ball->velocity.Dot(dir) > 0;
}

void HandleBallWallCollisions(vector<Ball*> &balls, Octree* octree)
{
    vector<BallWallPair> bwps;
    PotentialBallWallCollisions(bwps, balls, octree);
    for(unsigned int i = 0; i < bwps.size(); i++)
    {
        BallWallPair bwp = bwps[i];
        
        Ball* b = bwp.ball;
        Wall w = bwp.wall;
        if (TestBallWallCollision(b, w))
        {
            Vec3f dir = (WallDirection(w)).Normalize();
            b->velocity -= 2 * dir * b->velocity.Dot(dir);
        }
    }
}

void PerformUpdate(vector<Ball*> &balls, Octree* octree)
{
    ApplyGravity(balls);
    HandleBallBallCollisions(balls, octree);
    HandleBallWallCollisions(balls, octree);
}

void Advance(vector<Ball*> &balls, Octree* octree, float t, float &time_until_update)
{
    while (t > 0)
    {
        if (time_until_update <= t)
        {
            MoveBalls(balls, octree, time_until_update);
            PerformUpdate(balls, octree);
            t -= time_until_update;
            time_until_update = TIME_BETWEEN_UPDATES;
        }
        else
        {
            MoveBalls(balls, octree, t);
            time_until_update -= t;
            t = 0;
        }
    }
}



