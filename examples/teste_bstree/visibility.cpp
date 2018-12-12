#include "BSPTree.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "chimera/core/OpenGLDefs.hpp"
#include <GL/freeglut.h>

#include "List.hpp"
#include "Node.hpp"

#define canvas_width 400
#define canvas_height 400
#define canvas_name "Assignment 3: Visibility : Manish Lal Joshi"

/********************************SYNOPSIS***********************************************

The main motive of the application is to demonstrate the Visibility using OpenGL and GLUT
functions. A globle like stucture, i.e. octahedran is drawn in the center of the screen inside
a room with 5 faces enclosed and we can view the ocrahedron from one side only. The visibility
of the surface is determined by implementing Binary Space Partitioning (BSP) Tree and without
using opnGL depth buffering. The visible surfaces is Phong illuminated and pseduo-Gouraud shaded.
The coordinate of octahedran is (+/-50,0,0),(0,+/-50,0), (0,0,+/-50) from the origin. It is
composed of 8 triangular facets and each of then is red. The(x y plane. The octahedronup each key press will
move the light up or down 5 units,respectively.
3. Key T/t can toggle between moving both "center and eye" or "eye" only. This enables us to view if
  our algorithm is working correctly or not.

Note: I have tested the application with my_setup.h attached with the source.

***************************************************************************************/

bool rotate = false;
float rotationAngle = 0;
float eyeX = 0;
float eyeY = 0;
float centerX = 0;
float centerY = 0;
Octahedron oct;
Cube cub;
int polygon_id = 1;
Node* root = new Node();
Node* node = new Node();
List* polygonList = new List();
GLfloat lightPosition[] = {0, 99.9, 0, 1};
bool moveeye = false;

void my_reshape_handler(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
}

#define STRT_X_POS 25
#define STRT_Y_POS 25

void my_setup(int width, int height, char* window_name_str) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(STRT_X_POS, STRT_Y_POS);
    glutCreateWindow(window_name_str);
    glutReshapeFunc(my_reshape_handler);
}

void orthographicMatrixMul() {
    GLfloat left = -300, right = 300, top = 300, bottom = -300, near = 300, far = -300;
    GLfloat tx = -(right + left) / (right - left);
    GLfloat ty = -(top + bottom) / (top - bottom);
    GLfloat tz = -(far + near) / (far - near);
    GLfloat orthographicsMatrix[16] = {
        2 / (right - left), 0, 0, 0, 0, 2 / (top - bottom), 0, 0, 0, 0, -2 / (far - near), 0, tx, ty, tz, 1};
    glMultMatrixf(orthographicsMatrix);
}

void setCube() {
    Polygon p[10];
    glm::vec3 v[10];
    glm::vec3 n[10];
    glm::vec3 c[10];
    v[0] = glm::vec3(-100, -100, 100);
    v[1] = glm::vec3(-100, 100, 100);
    v[2] = glm::vec3(100, 100, 100);
    v[3] = glm::vec3(100, -100, 100);
    v[4] = glm::vec3(-100, -100, -100);
    v[5] = glm::vec3(-100, 100, -100);
    v[6] = glm::vec3(100, 100, -100);
    v[7] = glm::vec3(100, -100, -100);

    // Right Wall
    setPolygonVertices(&p[0], v[2], v[3], v[7]);
    n[0] = glm::vec3(1, 0, 0);
    c[0] = glm::vec3(1, 1, 1);
    setPolygonVertices(&p[1], v[2], v[7], v[6]);
    n[1] = glm::vec3(1, 0, 0);
    c[1] = glm::vec3(1, 1, 1);

    // Back Wall
    setPolygonVertices(&p[2], v[4], v[5], v[6]);
    n[2] = glm::vec3(0, 0, -1);
    c[2] = glm::vec3(1, 1, 1);
    setPolygonVertices(&p[3], v[4], v[6], v[7]);
    n[3] = glm::vec3(0, 0, -1);
    c[3] = glm::vec3(1, 1, 1);
    // Left Wall
    setPolygonVertices(&p[4], v[5], v[4], v[0]);
    n[4] = glm::vec3(-1, 0, 0);
    c[4] = glm::vec3(1, 1, 1);
    setPolygonVertices(&p[5], v[5], v[0], v[1]);
    n[5] = glm::vec3(-1, 0, 0);
    c[5] = glm::vec3(1, 1, 1);
    // Floor
    setPolygonVertices(&p[6], v[3], v[7], v[4]);
    n[6] = glm::vec3(0, -1, 0);
    c[6] = glm::vec3(0, 0, 0);
    setPolygonVertices(&p[7], v[3], v[4], v[0]);
    n[7] = glm::vec3(0, -1, 0);
    c[7] = glm::vec3(0, 0, 0);
    // Ceiling
    setPolygonVertices(&p[8], v[2], v[6], v[5]);
    n[8] = glm::vec3(0, 1, 0);
    c[8] = glm::vec3(0, 0, 0);
    setPolygonVertices(&p[9], v[2], v[5], v[1]);
    n[9] = glm::vec3(0, 1, 0);
    c[9] = glm::vec3(0, 0, 0);

    cub.setColors(c);
    cub.setFaces(p);
    cub.setNormal(n);

    for (int i = 0; i < 10; i++) {
        p[i].normal = n[i];
        p[i].color = c[i];
        p[i].id = polygon_id++;
        polygonList->Add(&p[i]);
    }
}

// set lighting parameters
void setLightingParameters() {
    // Sets Global Ambient Lighting with 0.4 if 1 no lighting
    GLfloat ambientColor[] = {0.4, 0.4, 0.4, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    GLfloat diffusedLightColor[] = {0.75, 0.75, 0.75, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusedLightColor);
    GLfloat specularLightColor[] = {0.0, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

// set the octahedran parameters
void setOctahedran() {
    glm::vec3 p[6];
    Polygon t[8];

    p[0] = glm::vec3(0, 50, 0);
    p[1] = glm::vec3(0, 0, -50);
    p[2] = glm::vec3(-50, 0, 0);
    p[3] = glm::vec3(50, 0, 0);
    p[4] = glm::vec3(0, 0, 50);
    p[5] = glm::vec3(0, -50, 0);
    oct.setVertices(p);

    setPolygonVerticesAndCalcNormal(&t[0], p[0], p[1], p[2]);
    setPolygonVerticesAndCalcNormal(&t[1], p[0], p[1], p[3]);
    setPolygonVerticesAndCalcNormal(&t[2], p[0], p[4], p[3]);
    setPolygonVerticesAndCalcNormal(&t[3], p[0], p[4], p[2]);
    setPolygonVerticesAndCalcNormal(&t[4], p[5], p[4], p[3]);
    setPolygonVerticesAndCalcNormal(&t[5], p[5], p[3], p[1]);
    setPolygonVerticesAndCalcNormal(&t[6], p[5], p[1], p[2]);
    setPolygonVerticesAndCalcNormal(&t[7], p[5], p[2], p[4]);

    oct.setFaces(t);
    glm::vec3 c(1, 0, 0);
    for (int i = 0; i < 8; i++) {
        t[i].id = polygon_id++;
        t[i].color = c;
        polygonList->Add(&t[i]);
    }
}

// this function is registered to handle special key function.
void special_handler(int code, int x, int y) {
    if (code == GLUT_KEY_UP) {
        lightPosition[1] += 5.0;
    } else if (code == GLUT_KEY_DOWN) {
        lightPosition[1] -= 5.0;
    }
    glutPostRedisplay();
}
// keyboard handler
void keyboard_handler(unsigned char keypressed, int x, int y) {
    if (keypressed == 'q' || keypressed == 'Q')
        exit(0);
    else if (keypressed == 'R' || keypressed == 'r') {
        eyeX -= 10;
        centerX -= 10;
    } else if (keypressed == 'L' || keypressed == 'l') {
        eyeX += 10;
        centerX += 10;
    } else if (keypressed == 'U' || keypressed == 'u') {
        eyeY -= 10;
        centerY -= 10;
    } else if (keypressed == 'D' || keypressed == 'd') {
        eyeY += 10;
        centerY += 10;
    } else if (keypressed == 't' || keypressed == 'T')
        moveeye = !moveeye;
    glutPostRedisplay();
}
// this is the main display function.
void display_func() {
    // clears the screen.
    glClearColor(1, 1, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    orthographicMatrixMul();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (moveeye)
        gluLookAt(eyeX, eyeY, 75, centerX, centerY, 0, 0, 1, 0);
    else
        gluLookAt(eyeX, eyeY, 75, 0, 0, 0, 0, 1, 0);
    setLightingParameters();
    glm::vec3 eye(eyeX, eyeY, 75);

    List* finalpl = new List();
    node->DrawTree(root, &eye, finalpl);
    finalpl->nextindex = 0;
    Polygon* fi = new Polygon();
    while ((fi = finalpl->Next()) != NULL) {
        glm::vec3 cc = fi->color;
        glColor3f(cc.x, cc.y, cc.z);
        glNormal3f(fi->normal.x, fi->normal.y, fi->normal.z);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
            glVertex3f(fi->vertices[i].x, fi->vertices[i].y, fi->vertices[i].z);
        }
        glEnd();
    }
    glutSwapBuffers();
}
// this initilizes rendering parameters.
void initRendering() {
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
}

void mouse_handler(int button, int state, int x, int y) {}

// this display function displays the instruction.
void displayMessage() {
    printf("%s\n", "Visibility Assignment Usage Instructions.");
    printf("%s\n", "R/r moves the viewpoint 10 units RIGHT.");
    printf("%s\n", "L/l moves the viewpoint 10 units LEFT.");
    printf("%s\n", "U/u moves the viewpoint 10 units UP.");
    printf("%s\n", "D/d moves the viewpoint 10 units DOWN.");
    printf("%s\n", "T/t enable and disable center movement.");
    printf("%s\n", "Arrow UP/DOWN: Moves the light position up/down by 5 units.");
    printf("%s\n", "Q/q quits the program");
}

// main entry point function.
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    my_setup(canvas_width, canvas_height, canvas_name);
    initRendering();
    setLightingParameters();
    glutKeyboardFunc(keyboard_handler);
    glutSpecialFunc(special_handler);
    glutMouseFunc(mouse_handler);
    glutDisplayFunc(display_func);
    setCube();
    setOctahedran();
    node->BuildTree(root, *polygonList);
    displayMessage();
    glutMainLoop();
    return 0;
}