//
//  LoadImage.cpp
//  OctreeCollisionDetection
//
//  Created by Ydy on 16/3/30.
//  Copyright © 2016年 Ydy. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

#include "LoadImage.h"

#include "octree.hpp"

//ALl balls in play
vector<Ball *> _balls;

//the camera angle
float _angle = 0.0f;

//an octree with all of the balls
Octree* _octree;

//the amount of time until PerformUpdate should be called
float _time_until_update = 0;
GLuint _textureID;

//deletes everything, called when exiting the program
void CleanUp()
{
    for(int i=0; i<_balls.size(); i++)
    {
        delete _balls[i];
    }
    delete _octree;
}

void HandleKeyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
            //Escape key
        case 27:
            CleanUp();
            break;
            
            //add 20 balls
        case ' ':
            for(int i=0; i<20; i++)
            {
                Ball* ball = new Ball();
                ball->position = Vec3f(8 * RandomFloat() - 4,
                                       8 * RandomFloat() - 4,
                                       8 * RandomFloat() - 4);
                ball->velocity = Vec3f(8 * RandomFloat() - 4,
                                       8 * RandomFloat() - 4,
                                       8 * RandomFloat() - 4);
                ball->radius = 0.1f * RandomFloat() + 0.1f;
                ball->color = Vec3f(0.6f * RandomFloat() + 0.2f,
                                    0.6f * RandomFloat() + 0.2f,
                                    0.6f * RandomFloat() + 0.2f);
                _balls.push_back(ball);
                _octree->Add(ball);
            }
        default:
            break;
    }
}

GLuint LoadTexture(Image* image)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    return textureID;
}

void InitRendering()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    
    Image *image = LoadBMP("/Users/daidaidaiyi/Documents/workspace/OctreeCollionDetection/OctreeCollionDetection/vtr.bmp");
    _textureID = LoadTexture(image);
    delete image;
}

void HandleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/(float)h, 1.0, 200.0);
}

void DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -20.0f);
    glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
    
    GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos[] = {1.0f, 0.2f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    //Draw the top and the bottom of the box
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
    
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    
    glEnd();
    glShadeModel(GL_SMOOTH);
    glDisable(GL_TEXTURE_2D);
    
    //Draw the balls
    for(unsigned int i = 0; i < _balls.size(); i++)
    {
        Ball* ball = _balls[i];
        glPushMatrix();
        glTranslatef(ball->position[0], ball->position[1], ball->position[2]);
        glColor3f(ball->color[0], ball->color[1], ball->color[2]);
        glutSolidSphere(ball->radius, 12, 12); //Draw a sphere
        glPopMatrix();
    }
    
    glutSwapBuffers();
}

//Called every TIMER_MS milliseconds
void Update(int value)
{
    Advance(_balls, _octree, (float)TIMER_MS / 1000.0f, _time_until_update);
    _angle += (float)TIMER_MS / 100;
    if (_angle > 360)
    {
        _angle -= 360;
    }
    
    glutPostRedisplay();
    glutTimerFunc(TIMER_MS, Update, 0);
}

int main(int argc, char** argv)
{
    srand((unsigned int)time(0)); //Seed the random number generator
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    
    glutCreateWindow("Octree Collision Detection");
    InitRendering();
    
    _octree = new Octree(Vec3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2),
                         Vec3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2), 1);
    
    glutDisplayFunc(DrawScene);
    glutKeyboardFunc(HandleKeyPress);
    glutReshapeFunc(HandleResize);
    glutTimerFunc(TIMER_MS, Update, 0);
    
    glutMainLoop();
    return 0;
}
 