//
//  main.cpp
//  GeometryProyection
//
//  Created by Nacho Martinez on 16/11/2018.
//  Copyright © 2018 Nacho Martinez. All rights reserved.
//
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "GLInclude.h"
#include "camera.h"
#include "shapes.h"

void Display(void);
void Init(void);
void Render(void);
void Lighting(void);
void InitCamera(int);
void HandleKeyboard(unsigned char key,int x, int y);
void HandleReshape(int,int);
void HandleMouseMotion(int,int);
void HandleMousePassiveMotion(int,int);
void UpdateEulerOrientation(EULER);
void HandleIdle(void);
void SetCameraPosition(int);
VECTOR3D GetForward(QUATERNION);

int fullscreen = FALSE;

int currentbutton = -1;
double rotatespeed = 3;
double tSpeed = 0.05;
float t = 0;

CAMERA camera;
FRUSTUM centerFrustum;

double rotateangle = 0;


int main(int argc,char **argv)
{
    camera.screenwidth = 600;
    camera.screenheight = 400;

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutCreateWindow("Geometria Proyectiva");
    if (fullscreen)
        glutFullScreen();
    glutDisplayFunc(Display);
    glutReshapeFunc(HandleReshape);
    glutReshapeWindow(camera.screenwidth, camera.screenheight);
    glutIdleFunc(HandleIdle);
    glutKeyboardFunc(HandleKeyboard);
    glutMotionFunc(HandleMouseMotion);
    glutPassiveMotionFunc(HandleMousePassiveMotion);
    
    Init();
    InitCamera(0);
    Lighting();
    
    glutMainLoop();
    return(0);
}

void Init(void)
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);
    glDisable(GL_CULL_FACE);
    
    glLineWidth(1.0);
    glPointSize(1.0);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glFrontFace(GL_CW);
    glClearColor(0.0,0.0,0.0,0.0);         /* Background colour */
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void Display(void)
{
    glDrawBuffer(GL_BACK_LEFT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glDrawBuffer(GL_BACK);
    
	double nearValue = 0.1;
	double farValue = 10000;
    
    double aspectRatio  = camera.screenwidth / (double)camera.screenheight;
    FRUSTUM centerFrustum = makeFrustum(camera.aperture, aspectRatio, nearValue, farValue);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(centerFrustum.left, centerFrustum.right, centerFrustum.bottom, centerFrustum.top, centerFrustum.nearValue, centerFrustum.farValue);
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    VECTOR3D target = Add(camera.position, camera.direction);
    
    // TODO
    MATRIX4 lookAtMatrix = lookAt(camera.position, target, camera.up);
    glLoadMatrixf(lookAtMatrix.m);

    glViewport(0,0,camera.screenwidth,camera.screenheight);
    
    Render();
    
    glutSwapBuffers();
}


void Render(void)
{
    GLfloat specular[4] = {1.0,1.0,1.0,1.0};
    GLfloat shiny[1] = {5.0};
    
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    
    glPushMatrix();
    glRotatef(rotateangle,0.0,1.0,0.0);

    drawAxis();
    
//    // SHOW CUBE
//    LINE lineToShow;
//    lineToShow.P.push_back({1, 0, 1});
//    lineToShow.P.push_back({1, 0, 2});
//    lineToShow.P.push_back({1, 1, 2});
//    lineToShow.P.push_back({1, 1, 1});
//
//    lineToShow.P.push_back({2, 1, 1});
//    lineToShow.P.push_back({2, 1, 2});
//    lineToShow.P.push_back({2, 0, 1});
//    lineToShow.P.push_back({2, 0, 2});
//    drawLine(lineToShow, blue, true);
    
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    
    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f( 1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    
    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
    
    glPopMatrix();
}

void Lighting(void)
{
    GLfloat fullambient[4] = {1.0,1.0,1.0,1.0};
    GLfloat position[4] = {0.0,0.0,0.0,0.0};
    GLfloat ambient[4]  = {0.2,0.2,0.2,1.0};
    GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
    GLfloat specular[4] = {0.0,0.0,0.0,1.0};
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
    glLightfv(GL_LIGHT0,GL_POSITION,position);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glEnable(GL_LIGHT0);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
}

void HandleKeyboard(unsigned char key,int x, int y)
{
    switch (key) {
        case ESC:
        case 'Q':
        case 'q':
            exit(0);
            break;
        case 'R':
        case 'r':
            rotateangle += rotatespeed;
            break;
            
        case '+':
            t += tSpeed;
            t = t > 1 ? 1:t;
            break;

        case '-':
            t -= tSpeed;
            t = t <0 ? 0:t;
            break;

        case 'h':
        case 'H':
            InitCamera(0);
            break;
        // TOP
        case 'w':
        case 'W':
            SetCameraPosition(1);
            break;
        //BOTTOM
        case 's':
        case 'S':
            SetCameraPosition(-1);
            break;
        // LEFT
        case 'a':
        case 'A':
            SetCameraPosition(-2);
            break;
        // RIGHT
        case 'd':
        case 'D':
            SetCameraPosition(2);
            break;
    }
}

void HandleIdle(void)
{
//    rotateangle += rotatespeed;
    glutPostRedisplay();
}

void HandleReshape(int w,int h)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    camera.screenwidth = w;
    camera.screenheight = h;
}

void HandleMouseMotion(int x, int y){
    std::cout << "Mouse Motion" << std::endl;
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    
    camera.position.x = x;
    camera.position.y = y;
}
void HandleMousePassiveMotion(int x, int y){
    std::cout << "Mouse Passive Motion" << std::endl;

    std::cout << x << std::endl;
    std::cout << y << std::endl;
    
    std::cout << "Camera Position" << std::endl;
    std::cout << camera.position.x << std::endl;
    std::cout << camera.position.y << std::endl;
    std::cout << camera.position.z << std::endl;
    
    std::cout << "Camera Direction" << std::endl;
    std::cout << camera.direction.x << std::endl;
    std::cout << camera.direction.y << std::endl;
    std::cout << camera.direction.z << std::endl;
    
}
void UpdateEulerOrientation(EULER euler) {
//    MATRIX3 spinX;
//    spinX.column0.x = cos(euler.yaw);
//    spinX.column0.y = (-1)*sin(euler.yaw);
//    spinX.column0.z = 0;
//
//    spinX.column1.x = sin(euler.yaw);
//    spinX.column1.y = cos(euler.yaw);
//    spinX.column1.z = 0;
//
//    spinX.column2.x = 0;
//    spinX.column2.y = 0;
//    spinX.column2.z = 1;
//
//    MATRIX3 spinY;
//    spinY.column0.x = cos(euler.pitch);
//    spinY.column0.y = 0;
//    spinY.column0.z = sin(euler.pitch);
//
//    spinY.column1.x = 0;
//    spinY.column1.y = 1;
//    spinY.column1.z = 0;
//
//    spinY.column2.x = (-1)*sin(euler.pitch);
//    spinY.column2.y = 0;
//    spinY.column2.z = cos(euler.pitch);
//
//    MATRIX3 spinZ;
//    spinZ.column0.x = 1;
//    spinZ.column0.y = 0;
//    spinZ.column0.z = 0;
//
//    spinZ.column1.x = 0;
//    spinZ.column1.y = cos(euler.roll);
//    spinZ.column1.z = (-1)*sin(euler.roll);
//
//    spinZ.column2.x = 0;
//    spinZ.column2.y = sin(euler.roll);
//    spinZ.column2.z = cos(euler.roll);
    
    euler.orientation = ToQuaternion(euler.yaw, euler.pitch, euler.roll);
}

void SetCameraPosition(int move) {
    switch (move) {
        // TOP
        case 1:
            camera.position.z -= camera.position.z*tSpeed;
            break;
        // BOTTOM
        case -1:
            camera.position.z += camera.position.z*tSpeed;
            break;
        // LEFT
        case 2:
            camera.direction.x -= 1 + tSpeed;
            break;
        // RIGHT
        case -2:
            camera.direction.x += 1 + tSpeed;
            break;
    }
}

VECTOR3D GetForward(QUATERNION qua) {
    VECTOR3D v;
    v.x = 0;
    v.y = 0;
    v.z = -1;
    return RotateWithQuaternion(v, qua);
}

void InitCamera(int mode)
{
    camera.aperture = 45;

    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 20;
    
    camera.direction.x = -camera.position.x;
    camera.direction.y = -camera.position.y;
    camera.direction.z = -camera.position.z;
    
    camera.up.x = 0;
    camera.up.y = 1;
    camera.up.z = 0;

}

