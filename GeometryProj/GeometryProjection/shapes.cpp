
#include <math.h>
#include "GLInclude.h"
#include "shapes.h"
#include <iostream>

void drawAxis() {
	COLOUR colorsAvailable [3] = {red, green, blue};
	VECTOR3D axis [3] = { {100, 0, 0}, {0, 100, 0}, {0, 0, 100} };
	LINE line;
	line.P.push_back({ 0,0,0 });
	for (int i = 0; i < 3; i++) {
		line.P.push_back(axis[i]);
		drawLine(line, colorsAvailable[i], false);
		line.P.pop_back();
	}
}

void drawDot(VECTOR3D position, float sradius, COLOUR color)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);

	VECTOR3D p[4], n[4];
	int STEP = 30;
	for (int i = 0; i<360; i += STEP) {
		for (int j = -90; j<90; j += STEP) {

			p[0].x = sradius * cos(j*DTOR) * cos(i*DTOR);
			p[0].y = sradius * sin(j*DTOR);
			p[0].z = sradius * cos(j*DTOR) * sin(i*DTOR);
			n[0] = p[0];

			p[1].x = sradius * cos((j + STEP)*DTOR) * cos(i*DTOR);
			p[1].y = sradius * sin((j + STEP)*DTOR);
			p[1].z = sradius * cos((j + STEP)*DTOR) * sin(i*DTOR);
			n[1] = p[1];

			p[2].x = sradius * cos((j + STEP)*DTOR) * cos((i + STEP)*DTOR);
			p[2].y = sradius * sin((j + STEP)*DTOR);
			p[2].z = sradius * cos((j + STEP)*DTOR) * sin((i + STEP)*DTOR);
			n[2] = p[2];

			p[3].x = sradius * cos(j*DTOR) * cos((i + STEP)*DTOR);
			p[3].y = sradius * sin(j*DTOR);
			p[3].z = sradius * cos(j*DTOR) * sin((i + STEP)*DTOR);
			n[3] = p[3];

			glBegin(GL_POLYGON);
			if (i % (STEP * 4) == 0)
				glColor3f(color.r, color.g, color.b);
			else
				glColor3f(color.r*0.5, color.g*0.5, color.b*0.5);
			for (int k = 0; k<4; k++) {
				glNormal3f(n[k].x, n[k].y, n[k].z);
				glVertex3f(p[k].x, p[k].y, p[k].z);
			}
			glEnd();
		}
	}

	glPopMatrix();
}

void drawLine(LINE line, COLOUR color, bool doDrawDots)
{
	glColor3f(color.r, color.g, color.b);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < line.P.size(); i++) {
        VECTOR3D vec = line.P[i];
        glVertex3f(vec.x, vec.y, vec.z);
    }
    glEnd();
	if (doDrawDots) {
        for (int i = 0; i < line.P.size(); i++) {
            VECTOR3D vec = line.P[i];
            drawDot(vec, 0.1, blue);
        }
	}
}
