#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <stdlib.h>
#include<bits/stdc++.h>
using namespace std;

const double eps = 1e-6;
float birdX = -70.0f;
float birdWingAngle = 0.0f;
bool wingUp = true;
bool keyState[256];
bool specialKey[256];
//float cowTx  = 0.0f;
float planeX = -100.0f;
float planeY = -5.0f;
//rain system
bool isRaining = false;
float rainBlend = 0.0f;
const int RAIN_COUNT = 400;
float rainX[RAIN_COUNT];
float rainY[RAIN_COUNT];
float rainSpeed[RAIN_COUNT];
float rainIntensity = 0.0f;
const float RAIN_STEP = 0.01f;

const int GROUND_WIDTH = 130;
float groundSnow[GROUND_WIDTH];

//snow system
bool isSnowing = false;
const int SNOW_COUNT = 200;
float snowX[SNOW_COUNT];
float snowY[SNOW_COUNT];
float snowSpeed[SNOW_COUNT];
float snowDrift[SNOW_COUNT];
float snowIntensity = 0.0f;
const float SNOW_STEP = 0.005f;
//cow and day night
float cowTx  = -25;
int cowDir = 1;
bool is_day = true;
float dayNightBlend = 1.0f;
const float DAY_NIGHT_STEP = 0.01f;
float sunangle = 0.0f;


void initGL() {
    glClearColor(1,1,1,1);
    glutReshapeWindow(1300, 700);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-65, 65, -35, 35,-35, 35);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPointSize(10.0f);
}
void initSnow() {
    for (int i = 0; i < SNOW_COUNT; i++) {
        snowX[i] = -65 + rand() % 130;
        snowY[i] = -35 + rand() % 70;
        snowSpeed[i] = 0.1f + (rand() % 100) / 500.0f;
        snowDrift[i] = ((rand() % 100) - 50) / 500.0f;
    }
    for(int i = 0; i < GROUND_WIDTH; i++) groundSnow[i] = 0.0f;

}

void keyDown(unsigned char key, int x, int y) {
    keyState[key] = true;
    if (key == 'r' || key == 'R') {
        isRaining = !isRaining;
        glutPostRedisplay();
    }
    if (key == 'D' or key == 'd') is_day = true;
    if (key == 'N' or key == 'n') is_day = false;
    if (key == 's' || key == 'S') {
        isSnowing = !isSnowing;
        glutPostRedisplay();
    }

}

void keyUp(unsigned char key, int x, int y) {
    keyState[key] = false;
}

void specialDown(int key, int x, int y) {
    specialKey[key] = true;
}

void specialUp(int key, int x, int y) {
    specialKey[key] = false;
}

int cnt = 0;
int down = 0;

float turbine_angle = 0;
float turbine_sx = 1.0;
float turbine_sy = 1.0;

// for cloud
float cloud1_tx = 0, cloud2_tx = 35, cloud3_tx = -35;

bool done1, done2, done3;
float sxcloud = 1,sycloud = 1;
float tx = 0 , ty = 0, angle = 0, sx = 1, sy = 1;
void update() {
     turbine_angle += 6;
    if (turbine_angle >= 360) turbine_angle = 0;

    cloud1_tx -= 0.2;
    if (cloud1_tx <= -80) cloud1_tx = 80;

    cloud2_tx -= 0.2;
    if (cloud2_tx <= -80) cloud2_tx = 80;

    cloud3_tx -= 0.2;
    if (cloud3_tx <= -80) cloud3_tx = 80;
    glutPostRedisplay();

    tx -= 1;

    if (tx <= -150) tx = 20;
    if (isRaining) {
        for (int i = 0; i < RAIN_COUNT; i++) {
            rainY[i] -= rainSpeed[i];

            if (rainY[i] < -35) {
                rainY[i] = 35;
                rainX[i] = -65 + rand() % 130;
            }
        }
    }
    if (isRaining && rainBlend < 1.0f) {
        rainBlend += 0.01f;
        sxcloud+=0.002;
        sycloud+=0.002;
        if (rainBlend > 1.0f) rainBlend = 1.0f;
    }
    else if (!isRaining && rainBlend > 0.0f) {
        rainBlend -= 0.01f;
        sxcloud-=0.002;
        sycloud-=0.002;
        if (rainBlend < 0.0f) rainBlend = 0.0f;
    }
    if (isRaining) {
    if (rainIntensity < 1.0f)
        rainIntensity += RAIN_STEP;
    } else {
        if (rainIntensity > 0.0f)
            rainIntensity -= RAIN_STEP;
    }

    rainIntensity = std::max(0.0f, std::min(1.0f, rainIntensity));
    //cow
    cowTx += 0.1f * cowDir;
    if (cowTx >= 9) {
        cowDir = -1;
    }
    if (cowTx <= -25) {
         cowDir = 1;
    }

    if (is_day and dayNightBlend < 1.0f) {
        dayNightBlend += DAY_NIGHT_STEP;
        sunangle+=5;
        if (dayNightBlend > 1.0f) dayNightBlend = 1.0f;
    } else if (!is_day and dayNightBlend > 0.0f) {
        dayNightBlend -= DAY_NIGHT_STEP;
        sunangle-=5;
        if (dayNightBlend < 0.0f) dayNightBlend = 0.0f;
    }
    if (isSnowing) {
        for (int i = 0; i < SNOW_COUNT; i++) {
            snowY[i] -= snowSpeed[i];
            snowX[i] += snowDrift[i];

            if (snowX[i] < -65) snowX[i] = 65;
            if (snowX[i] > 65) snowX[i] = -65;

            int xIndex = int(snowX[i] + 65);
            if (snowY[i] <= -35 + groundSnow[xIndex]) {

                groundSnow[xIndex] += 0.05f;
                if (groundSnow[xIndex] > 5.0f)
                    groundSnow[xIndex] = 5.0f;

                // Reset snowflake to top
                snowY[i] = 35;
                snowX[i] = -65 + rand() % 130;
            }
        }
    }

    if (isSnowing && snowIntensity < 1.0f) {
        snowIntensity += SNOW_STEP;
        if (snowIntensity > 1.0f) snowIntensity = 1.0f;
    } else if (!isSnowing && snowIntensity > 0.0f) {
        snowIntensity -= SNOW_STEP;
        for (int i = 0; i < 130; i++) {
            groundSnow[i] = 0.0f;
        }
        if (snowIntensity < 0.0f) snowIntensity = 0.0f;
    }
    birdX += 0.15f;
    if (birdX > 75.0f) birdX = -75.0f;
    // Flapping
    if (wingUp) {
        birdWingAngle += 0.05f;
        if (birdWingAngle > 0.8f) wingUp = false;
    } else {
        birdWingAngle -= 0.05f;
        if (birdWingAngle < -0.4f) wingUp = true;
    }
}

void timer(int) {
    update();
    planeX += 0.1f;
    planeY += 0.05f;

    if (planeX > 90.0f || planeY > 50.0f) {
        planeX = -100.0f;
        planeY = -5.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(50, timer, 100);
}
void drawSingleBird(float x, float y) {
    glLineWidth(2.0f);
    // Birds look black or dark gray against the sky
    glColor3f(0.1f * dayNightBlend, 0.1f * dayNightBlend, 0.1f * dayNightBlend);

    glBegin(GL_LINE_STRIP);
        // Left wing
        glVertex2f(x - 1.5f, y + birdWingAngle);
        // Body center
        glVertex2f(x, y);
        // Right wing
        glVertex2f(x + 1.5f, y + birdWingAngle);
    glEnd();
    glLineWidth(1.0f);
}

void drawBirds() {
    drawSingleBird(birdX, 20.0f);
    drawSingleBird(birdX - 4.0f, 22.0f);
    drawSingleBird(birdX - 3.0f, 18.0f);
}
void ddaLine(float x1, float y1, float x2, float y2){

    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps = (int)ceil(max(fabs(dx), fabs(dy)));
    if(steps == 0) return;

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1, y = y1;

    glBegin(GL_POINTS);
    for(int i = 0; i <= steps; i++){
        glVertex2f(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();
}


void filledPolygonWithDDA(vector<pair<float,float>> poly){
    int n = poly.size();

    for(int i = 0; i < n; i++){
        ddaLine(
            poly[i].first, poly[i].second,
            poly[(i+1)%n].first, poly[(i+1)%n].second
        );
    }

    float ymin = poly[0].second, ymax = poly[0].second;
    for(auto p : poly){
        ymin = min(ymin, p.second);
        ymax = max(ymax, p.second);
    }

    for(float y = ymin; y <= ymax; y += 0.1f){
        vector<float> xints;

        for(int i = 0; i < n; i++){
            float x1 = poly[i].first;
            float y1 = poly[i].second;
            float x2 = poly[(i+1)%n].first;
            float y2 = poly[(i+1)%n].second;

            if(fabs(y1 - y2) < 1e-6) continue;

            if(y >= min(y1,y2) && y < max(y1,y2)){
                float x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
                xints.push_back(x);
            }
        }

        sort(xints.begin(), xints.end());

        for(int i = 0; i + 1 < xints.size(); i += 2){
            ddaLine(xints[i], y, xints[i+1], y);
        }
    }
}

void drawPixel(float x, float y){
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

void midpointLine(float x1, float y1, float x2, float y2){
    float dx = x2 - x1;
    float dy = y2 - y1;

    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;

    dx = fabs(dx);
    dy = fabs(dy);

    float x = x1;
    float y = y1;

    // slope |m| <= 1
    if(dx >= dy){
        float d = 2*dy - dx;
        float incrE = 2*dy;
        float incrNE = 2*(dy - dx);

        for(int i = 0; i <= dx; i++){
            drawPixel(x, y);
            if(d <= 0){
                d += incrE;
            }else{
                d += incrNE;
                y += sy;
            }
            x += sx;
        }
    }
    // slope |m| > 1
    else{
        float d = 2*dx - dy;
        float incrE = 2*dx;
        float incrNE = 2*(dx - dy);

        for(int i = 0; i <= dy; i++){
            drawPixel(x, y);
            if(d <= 0){
                d += incrE;
            }else{
                d += incrNE;
                x += sx;
            }
            y += sy;
        }
    }
}
void filledPolygonWithDDAMidpoint(const vector<pair<float,float>>& poly){
    int n = poly.size();

    for(int i = 0; i < n; i++){
        midpointLine(
            poly[i].first, poly[i].second,
            poly[(i+1)%n].first, poly[(i+1)%n].second
        );
    }


    float ymin = poly[0].second;
    float ymax = poly[0].second;

    for(auto &p : poly){
        ymin = min(ymin, p.second);
        ymax = max(ymax, p.second);
    }


    for(float y = ymin; y <= ymax; y += 0.1f){
        vector<float> xints;

        for(int i = 0; i < n; i++){
            float x1 = poly[i].first;
            float y1 = poly[i].second;
            float x2 = poly[(i+1)%n].first;
            float y2 = poly[(i+1)%n].second;

            if(fabs(y1 - y2) < 1e-6) continue;

            if(y >= min(y1,y2) && y < max(y1,y2)){
                float x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
                xints.push_back(x);
            }
        }

        sort(xints.begin(), xints.end());

        for(int i = 0; i + 1 < xints.size(); i += 2){
            midpointLine(xints[i], y, xints[i+1], y);
        }
    }
}

void drawAeroplane() {

    //Body
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex3f(-1.5f, -0.2f, -0.2f);
        glVertex3f( 1.5f, -0.2f, -0.2f);
        glVertex3f( 1.5f,  0.2f, -0.2f);
        glVertex3f(-1.5f,  0.2f, -0.2f);

        glVertex3f(-1.5f, -0.2f,  0.2f);
        glVertex3f( 1.5f, -0.2f,  0.2f);
        glVertex3f( 1.5f,  0.2f,  0.2f);
        glVertex3f(-1.5f,  0.2f,  0.2f);
    glEnd();

    //nose
    glColor3f(1.0f, 0.3f, 0.3f);
    glBegin(GL_TRIANGLES);
        glVertex3f(1.5f, -0.2f, -0.2f);
        glVertex3f(1.5f,  0.2f,  0.2f);
        glVertex3f(2.2f,  0.0f,  0.0f);
    glEnd();

    //wings
    glColor3f(0.2f, 0.2f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f,  3.0f);
        glVertex3f( 0.5f, 0.0f,  3.0f);

        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f, -3.0f);
        glVertex3f( 0.5f, 0.0f, -3.0f);
    glEnd();

    // Tail (LEFT SIDE)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-1.5f, 0.0f, 0.0f);
        glVertex3f(-2.2f, 1.2f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
    glEnd();
}
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / (float)h;
    float worldWidth = 130.0f;
    float worldHeight = 70.0f;

    if (aspect >= worldWidth / worldHeight) {
        float newWidth = worldHeight * aspect;
        glOrtho(-newWidth/2, newWidth/2, -35, 35, -10, 10);
    } else {
        float newHeight = worldWidth / aspect;
        glOrtho(-65, 65, -newHeight/2, newHeight/2, -10, 10);
    }
    glMatrixMode(GL_MODELVIEW);
}


void drawBezier(float x0, float y0, float x1, float y1,float x2, float y2,float r,float g,float b) {
    glBegin(GL_POLYGON);
    glColor3f(r,g,b);
    for(float t = 0.0; t <= 1.0; t += 0.0001) {
        float x =
            (1 - t) * (1 - t) * x0 +
            2 * t * (1 - t) * x1 +
            t * t * x2;

        float y =
            (1 - t) * (1 - t) * y0 +
            2 * t * (1 - t) * y1 +
            t * t * y2;

        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}
void initRain() {
    for (int i = 0; i < RAIN_COUNT; i++) {
        rainX[i] = -65 + rand() % 130;
        rainY[i] = -35 + rand() % 70;
        rainSpeed[i] = 1.0f + (rand() % 100) / 80.0f;
    }
}

void drawRain() {
    int dropsToDraw = int(RAIN_COUNT * rainIntensity);
    glLineWidth(2.0f);
    glColor3f(0.4f, 0.4f, 0.9f); // blue rain

    glBegin(GL_LINES);
    for (int i = 0; i < dropsToDraw; i++) {
        glVertex3f(rainX[i], rainY[i], 0.0f);
        glVertex3f(rainX[i], rainY[i] - 2.5f,0.0f);
    }
    glEnd();
    glLineWidth(1.0f);
}

#include <cmath>

#define PI 3.14159265f
//faysal vai er circle
void drawCircle(float x_c, float y_c, float r, float cr, float cg, float cb, float start_deg, float end_deg) {
    glColor3f(cr, cg, cb);
    glBegin(GL_POLYGON);
        glVertex3f(x_c, y_c, 0.0f);
        for (float i = start_deg; i <= end_deg; i++) {
            float theta = i * (PI / 180.0f);
            float x = x_c + r * cos(theta);
            float y = y_c + r * sin(theta);
            glVertex3f(x, y, 0.0f);
        }
    glEnd();
}

void drawTurbine(float x, float y) {
    glLoadIdentity();
    glTranslatef(0, 0, 0);
    glScalef(1, 1, 1);
    glRotatef(0, 0, 0, 1);
    glColor3f(0.6, 0.6, 0.6);
    vector<pair<float, float>> polygonPoints = {
        {x, y},{x + 2, y},{x + 1, y + 20},{x, y + 20}
    };
    filledPolygonWithDDA(polygonPoints);
    /*
    glBegin(GL_QUADS);
        glColor3f(0.6, 0.6, 0.6);
        glVertex3f(x, y, 0.0f);
        glVertex3f(x + 2, y, 0.0f);
        glVertex3f(x + 1, y + 20, 0.0f);
        glVertex3f(x, y + 20, 0.0f);
    glEnd();
    */

    glLoadIdentity();
    glTranslatef(x + 1, y + 20, 0);
    glRotatef(turbine_angle, 0, 0, 1);

    glColor3f(0.9, 0.9, 0.9);
    for (int i = 1; i <= 3; i++) {
            polygonPoints = {
        {0, 1},{12, 1},{12, -1}, {0, -1}
    };
    filledPolygonWithDDA(polygonPoints);
        //glBegin(GL_QUADS);
            //glColor3f(0.9, 0.9, 0.9);
            //glVertex3f(0, 1, 0.0f);
          //  glVertex3f(12, 1, 0.0f);
        //glVertex3f(12, -1, 0.0f);
          //  glVertex3f(0, -1, 0.0f);
        //glEnd();

        glRotatef(120, 0, 0, 1);
    }

    glLoadIdentity();
    glTranslatef(0, 0, 0.0f);
    glScalef(1, 1, 1);
    glRotatef(0, 0.0, 0.0, 1.0);

    drawCircle(x + 1 - 0.35, y + 20, 1.8, 0, 0, 0, 0, 360);
}

void drawCloud(float x_c, float y_c, float &tx) {
    glLoadIdentity();
    glTranslatef(tx,1,0.0f);
    glRotatef(0,0.0,0.0,1.0);
    glScalef(sxcloud,sycloud,1);

    float cloudShade = 1.0f - 0.5f * rainIntensity; // white → gray
    drawCircle(x_c - 4, y_c, 3, cloudShade, cloudShade, cloudShade, 0, 360);
    drawCircle(x_c - 1, y_c + 1, 4, cloudShade, cloudShade, cloudShade, 0, 360);
    drawCircle(x_c + 3, y_c, 3.5, cloudShade, cloudShade, cloudShade, 0, 360);
    drawCircle(x_c + 1, y_c - 1.5, 3, cloudShade, cloudShade, cloudShade, 0, 360);
    drawCircle(x_c + 2, y_c + 2.5, 2.2, cloudShade, cloudShade, cloudShade, 0, 360);
    drawCircle(x_c - 2, y_c + 2, 2.5, cloudShade, cloudShade, cloudShade, 0, 360);
    drawCircle(x_c + 4, y_c + 1.5, 2, cloudShade, cloudShade, cloudShade, 0, 360);
}

void drawSunGlobe(float x, float y, float r) {
    int steps = 100;


    for(int i = 0; i < steps; i++) {
        float t = (float)i / steps;
        float radius = r * (1.0f - t * 0.8f);

        float rainFactor = 1.0f - rainIntensity * 0.6f;
        float nightFactor = dayNightBlend;

        float red   = (0.886f * rainFactor) * nightFactor;
        float green = (0.416f * rainFactor) * nightFactor;
        float blue  = (0.200f * rainFactor) * nightFactor;

        glColor3f(red, green, blue);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(x, y, 0.0f);
            for(float angle = 0; angle <= 360; angle += 5) {
                float theta = angle * 3.14159265 / 180.0f;
                glVertex3f(x + radius * cos(theta), y + radius * sin(theta), 0.0f);
            }
        glEnd();
    }
}

//amar circle
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * PI / 180.0f;
        float x = cx + r * cos(theta);
        float y = cy + r * sin(theta);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}
void drawCircleMidpoint(float xc, float yc, float r) {
    int x = 0;
    int y = (int)r;
    int d = 1 - (int)r;

    glBegin(GL_POINTS);

    auto plot8 = [&](int X, int Y){
        glVertex3f(xc + X, yc + Y, 0.0f);
        glVertex3f(xc - X, yc + Y, 0.0f);
        glVertex3f(xc + X, yc - Y, 0.0f);
        glVertex3f(xc - X, yc - Y, 0.0f);
        glVertex3f(xc + Y, yc + X, 0.0f);
        glVertex3f(xc - Y, yc + X, 0.0f);
        glVertex3f(xc + Y, yc - X, 0.0f);
        glVertex3f(xc - Y, yc - X, 0.0f);
    };

    while (x <= y) {
        for(int i = -x; i <= x; i++) {
            glVertex3f(xc + i, yc + y, 0.0f);
            glVertex3f(xc + i, yc - y, 0.0f);
        }
        for(int i = -y; i <= y; i++) {
            glVertex3f(xc + i, yc + x, 0.0f);
            glVertex3f(xc + i, yc - x, 0.0f);
        }

        if(d < 0) {
            d += 2*x + 3;
        } else {
            d += 2*(x - y) + 5;
            y--;
        }
        x++;
    }

    glEnd();
}
//mahir er circle
void drawCirclem(float x_c, float y_c, float r, float cr, float cg, float cb, float start_deg=0.0f, float end_deg=360.0f) {
    glColor3f(cr, cg, cb);
    glBegin(GL_POLYGON);
        glVertex3f(x_c, y_c, 0.0f);
        for (float i = start_deg; i <= end_deg; i++) {
            float theta = i * (PI / 180.0f);
            float x = x_c + r * cosf(theta);
            float y = y_c + r * sinf(theta);
            glVertex3f(x, y, 0.0f);
        }
    glEnd();
}
void drawGhamla(float x, float y) {

    glColor3f(60.0f/255.0f, 35.0f/255.0f, 6.0f/255.0f);
    glBegin(GL_POLYGON);
        float shadowOffsetX = 1.5f;
        float shadowOffsetY = -1.0f;
        glVertex3f(x + shadowOffsetX, y + shadowOffsetY, 0.0f);
        for (float i = 180; i <= 360; i+=1.0f) {
            float theta = i * PI / 180.0f;
            float px = x + shadowOffsetX + 6.0f * cosf(theta);
            float py = y + shadowOffsetY + 4.0f * sinf(theta);
            glVertex3f(px, py, 0.0f);
        }
    glEnd();

    glColor3f(141.0f/255.0f, 90.0f/255.0f, 31.0f/255.0f);
    glBegin(GL_POLYGON);
        glVertex3f(x, y, 0.0f);
        for (float i = 180; i <= 360; i+=1.0f) {
            float theta = i * PI / 180.0f;
            float px = x + 6.0f * cosf(theta);
            float py = y + 4.0f * sinf(theta);
            glVertex3f(px, py, 0.0f);
        }
    glEnd();
}
void drawBoat(float &tx) {
    glLoadIdentity();
    glTranslatef(tx,1,0.0f);
    glRotatef(0,0.0,0.0,1.0);
    glScalef(1,1,1);


    // Boat
    glColor3f(0.0f,0.0f,0.0f);
    /*
    glBegin(GL_POLYGON);
        glVertex3f(75.0f + 0, -17.0f, 0.0f);
        glVertex3f(35.0f + 0, -17.0f, 0.0f);
        glVertex3f(45.0f + 0, -24.0f, 0.0f);
        glVertex3f(65.0f + 0, -24.0f, 0.0f);
    glEnd();
    */
    vector<pair<float, float>> polygonPoints = {
        {75.0f + 0, -17.0f},{35.0f + 0, -17.0f},{45.0f + 0, -24.0f},{65.0f + 0, -24.0f}
    };filledPolygonWithDDA(polygonPoints);


    // Boat (lathi)
    glColor3f(0.55f, 0.27f, 0.07f);
    polygonPoints = {
        {50.0f + 0, -17.0f},{50.0f + 0, -2.0f}
    };filledPolygonWithDDA(polygonPoints);

    /*glBegin(GL_LINES);
        glVertex3f(50.0f + 0, -17.0f, 0.0f);
        glVertex3f(50.0f + 0, -2.0f, 0.0f);
    glEnd();*/

    // (pal)
    glColor3f(1.0f,0.0f,0.0f);
     polygonPoints = {
        {50.0f + 0, -2.0f},{50.0f + 0, -17.0f}, {60.0f + 0, -17.0f}
    };filledPolygonWithDDA(polygonPoints);
    /*
    glBegin(GL_TRIANGLES);
        glVertex3f(50.0f + 0, -2.0f, 0.0f);
        glVertex3f(50.0f + 0, -17.0f, 0.0f);
        glVertex3f(60.0f + 0, -17.0f, 0.0f);
    glEnd();*/
}
void drawCow(float x, float y, float cowTx) {
    // Body
    drawCirclem(x + cowTx, y+1.0f, 3.5f, 0.55f, 0.27f, 0.07f);
    // Head
    drawCirclem(x + cowDir * 4.0f + cowTx, y+1.5f, 1.5f, 0.55f, 0.27f, 0.07f);
    // Horns
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(x +cowDir * 4.5f + cowTx, y+2.5f, 0.0f);
        glVertex3f(x +cowDir * 4.7f + cowTx, y+2.0f, 0.0f);
        glVertex3f(x + cowDir *4.3f + cowTx, y+2.0f, 0.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex3f(x + cowDir * 3.5f + cowTx, y+2.5f, 0.0f);
        glVertex3f(x +cowDir * 3.7f + cowTx, y+2.0f, 0.0f);
        glVertex3f(x + cowDir *3.3f + cowTx, y+2.0f, 0.0f);
    glEnd();
    // Legs
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_POLYGON);
        glVertex3f(x-2.5f + cowTx, y-1.0f, 0.0f);
        glVertex3f(x-2.0f + cowTx, y-1.0f, 0.0f);
        glVertex3f(x-2.0f + cowTx, y-2.5f, 0.0f);
        glVertex3f(x-2.5f + cowTx, y-2.5f, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x+2.0f + cowTx, y-1.0f, 0.0f);
        glVertex3f(x+2.5f + cowTx, y-1.0f, 0.0f);
        glVertex3f(x+2.5f + cowTx, y-2.5f, 0.0f);
        glVertex3f(x+2.0f + cowTx, y-2.5f, 0.0f);
    glEnd();
}
void RiverShading()
{
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.4f, 0.8f);
    glVertex3f(-65.0f, -12.0f, 0.0f);
    glVertex3f(65.0f, -12.0f, 0.0f);

    glColor3f(0.4f, 0.7f, 1.0f);
    glVertex3f(65.0f, -24.0f, 0.0f);
    glVertex3f(-65.0f, -24.0f, 0.0f);
    glEnd();
}
void drawTree(float x, float y) {
    float trunkWidth = 1.2f;
    float trunkHeight = 6.0f;
    float baseH = y + trunkHeight;


    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y - 0.2f, 0.0f);
        for (float angle = 0; angle <= 360; angle += 5) {
            float rad = angle * PI / 180.0f;
            float sx = x + cos(rad) * trunkWidth * 2.0f;
            float sy = y - 0.2f + sin(rad) * 0.5f;
            glVertex3f(sx, sy, 0.0f);
        }
    glEnd();

    glColor3f(0.6f, 0.35f, 0.1f);
    glBegin(GL_POLYGON);
        glVertex3f(x - trunkWidth/2, y, 0.0f);
        glVertex3f(x, y, 0.0f);
        glVertex3f(x + trunkWidth/4, y + trunkHeight, 0.0f);
        glVertex3f(x - trunkWidth/2, y + trunkHeight, 0.0f);
    glEnd();

    glColor3f(0.35f, 0.2f, 0.05f);
    glBegin(GL_POLYGON);
        glVertex3f(x, y, 0.0f);
        glVertex3f(x + trunkWidth/2, y, 0.0f);
        glVertex3f(x + trunkWidth/2, y + trunkHeight, 0.0f);
        glVertex3f(x + trunkWidth/4, y + trunkHeight, 0.0f);
    glEnd();

    //Leaves base color
    int numLeaves = 5;
    float leafR = 2.8f;
    float leafXOffsets[5] = {-2.2f, 2.2f, -1.8f, 1.8f, 0};
    float leafYOffsets[5] = {1.0f, 1.0f, 3.5f, 3.5f, 5.5f};

    for(int i = 0; i < numLeaves; i++){
        float leafX = x + leafXOffsets[i];
        float leafY = baseH + leafYOffsets[i];

        glColor3f(0.1f, 0.7f, 0.2f); // apnar ager halka sobuj
        drawCircleMidpoint(leafX, leafY, leafR);

        glEnd();
    }
}

void drawGroundSnow() {
    glColor3f(1.0f, 1.0f, 1.0f); // white
    for(int i = 0; i < GROUND_WIDTH-1; i++) {
        glBegin(GL_QUADS);
            glVertex2f(i - 65, -35);
            glVertex2f(i - 64, -35);
            glVertex2f(i - 64, -35 + groundSnow[i+1]);
            glVertex2f(i - 65, -35 + groundSnow[i]);
        glEnd();
    }
}

//House building
void drawFilledPolygon(const vector<pair<float, float>>& points) {
    glBegin(GL_POLYGON);
    for (auto &p : points) {
        glVertex3f(p.first, p.second, 0.0f);
    }
    glEnd();
}
void drawSnow() {
    int flakesToDraw = int(SNOW_COUNT * snowIntensity);

    glColor3f(1.0f, 1.0f, 1.0f); // white snow

    for (int i = 0; i < flakesToDraw; i++) {
        drawCircle(snowX[i], snowY[i], 0.5f);
    }
}

void Home(){
    // body of hut
    glColor3f(223.0f/255.0f, 184.0f/255.0f, 129.0f/255.0f);
    vector<pair<float, float>> polygonPoints = {
    {50.0f, -10.0f},
    { 39.0f, -10.0f},
    { 39.0f, -5.0f},
    { 50.0f, -5.0f},
    };

    filledPolygonWithDDA (polygonPoints);

    glColor3f(253.0f/255.0f, 221.0f/255.0f, 163.0f/255.0f);
    polygonPoints = {
        {50.0f, -10.0f},
        {50.0f, -5.0f},
        {51.0f, -5.0f},
        {55.0f, -1.0f},
        {58.0f, -5.0f},
        {58.0f, -10.0f}
    };
    filledPolygonWithDDA (polygonPoints);

    // roof of hut
    glColor3f(164.0f/255.0f, 86.0f/255.0f, 38.0f/255.0f);
    polygonPoints = {
        {38.0f, -5.0f},
        {43.0f, 0.0f},
        {56.0f, 0.0f},
        {51.0f, -5.0f},
    };
    filledPolygonWithDDA (polygonPoints);

    polygonPoints = {
        {56.0f, 0.0f},
        {60.0f, -5.0f},
        {58.0f, -5.0f},
        {55.0f, -1.0f},
    };filledPolygonWithDDA (polygonPoints);

    //window and door of hut
    polygonPoints = {
        {44.0f, -10.0f},
        {44.0f, -7.0f},
        {47.0f, -7.0f},
        {47.0f, -10.0f},
    };filledPolygonWithDDA (polygonPoints);


    polygonPoints = {
        {54.0f, -7.0f},
        {54.0f, -5.0f},
        {56.0f, -5.0f},
        {56.0f, -7.0f},
    };filledPolygonWithDDA (polygonPoints);

    glColor3f(213.0f/255.0f, 111.0f/255.0f, 22.0f/255.0f);
    polygonPoints = {
        {-15.0f, -5.0f},
        {0.0f, -5.0f},
        {-7.5f, 1.5f}
    };drawFilledPolygon (polygonPoints);

    glColor3f(223.0f/255.0f, 184.0f/255.0f, 129.0f/255.0f);
    polygonPoints = {
        {-14.0f, -10.0f},
        {-14.0f, -5.0f},
        {-1.0f, -5.0f},
        {-1.0f, -10.0f}
    };drawFilledPolygon (polygonPoints);

    glColor3f(164.0f/255.0f, 86.0f/255.0f, 38.0f/255.0f);
    polygonPoints = {
        {-3.5f, -6.0f},
        {-3.5f, -7.5f},
        {-5.0f, -7.5f},
        {-5.0f, -6.0f}
    };drawFilledPolygon (polygonPoints);
     polygonPoints = {
        {-10.5f, -6.0f},
        {-10.5f, -7.5f},
        {-12.0f, -7.5f},
        {-12.0f, -6.0f}
    };drawFilledPolygon (polygonPoints);
    polygonPoints = {
        {-6.0f, -10.0f},
        {-6.0f, -7.0f},
        {-9.5f, -7.0f},
        {-9.5f, -10.0f}
    };drawFilledPolygon (polygonPoints);

    // body of hut
    glColor3f(223.0f/255.0f, 184.0f/255.0f, 129.0f/255.0f);
    polygonPoints = {
    {-43.0f, -10.0f},
    { -32.0f, -10.0f},
    { -32.0f, -5.0f},
    { -43.0f, -5.0f},
    };
    filledPolygonWithDDAMidpoint (polygonPoints);

    glColor3f(253.0f/255.0f, 221.0f/255.0f, 163.0f/255.0f);
    polygonPoints = {
        {-43.0f, -10.0f},
        {-43.0f, -5.0f},
        {-44.0f, -5.0f},
        {-48.0f, -1.0f},
        {-51.0f, -5.0f},
        {-51.0f, -10.0f}
    };
    filledPolygonWithDDAMidpoint (polygonPoints);

    // roof of hut
    glColor3f(164.0f/255.0f, 86.0f/255.0f, 38.0f/255.0f);
    polygonPoints = {
        {-31.0f, -5.0f},
        {-36.0f, 0.0f},
        {-49.0f, 0.0f},
        {-44.0f, -5.0f},
    };
    filledPolygonWithDDA (polygonPoints);

    polygonPoints = {
        {-49.0f, 0.0f},
        {-53.0f, -5.0f},
        {-51.0f, -5.0f},
        {-48.0f, -1.0f},
    };filledPolygonWithDDA (polygonPoints);

    //window and door of hut
    polygonPoints = {
        {-37.0f, -10.0f},
        {-37.0f, -7.0f},
        {-40.0f, -7.0f},
        {-40.0f, -10.0f},
    };filledPolygonWithDDAMidpoint (polygonPoints);


    polygonPoints = {
        {-47.0f, -7.0f},
        {-47.0f, -5.0f},
        {-49.0f, -5.0f},
        {-49.0f, -7.0f},
    };filledPolygonWithDDAMidpoint (polygonPoints);

    glColor3f(122.0f/255.0f, 0.0f/255.0f, 3.0f/255.0f);
    polygonPoints = {
        {10.0f, -10.0f},
        {10.0f, 0.0f},
        {30.0f, 0.0f},
        {30.0f, -10.0f},
    };drawFilledPolygon (polygonPoints);

    glColor3f(241.0f/255.0f, 162.0f/255.0f, 95.0f/255.0f);
    polygonPoints = {
        {12.0f, -10.0f},
        {12.0f, -2.0f},
        {16.0f, -2.0f},
        {16.0f, -10.0f},
    };drawFilledPolygon (polygonPoints);

    glColor3f(241.0f/255.0f, 162.0f/255.0f, 95.0f/255.0f);
    polygonPoints = {
        {18.0f, -10.0f},
        {18.0f, -2.0f},
        {22.0f, -2.0f},
        {22.0f, -10.0f},
    };drawFilledPolygon (polygonPoints);

    polygonPoints = {
        {24.0f, -10.0f},
        {24.0f, -2.0f},
        {28.0f, -2.0f},
        {28.0f, -10.0f},
    };drawFilledPolygon (polygonPoints);

    glColor3f(142.0f/255.0f, 68.0f/255.0f, 19.0f/255.0f);
    polygonPoints = {
        {8.0f, 0.0f},
        {10.0f, 2.0f},
        {30.0f, 2.0f},
        {32.0f, 0.0f},
    };drawFilledPolygon (polygonPoints);

    // 1st floor
    glColor3f(122.0f/255.0f, 0.0f/255.0f, 3.0f/255.0f);
    polygonPoints = {
        {14.0f, 2.0f},
        {14.0f, 7.0f},
        {26.0f, 7.0f},
        {26.0f, 2.0f},
    };drawFilledPolygon (polygonPoints);

    glColor3f(142.0f/255.0f, 68.0f/255.0f, 19.0f/255.0f);
    polygonPoints = {
        {12.0f, 7.0f},
        {14.0f, 9.0f},
        {26.0f, 9.0f},
        {28.0f, 7.0f},
    };drawFilledPolygon (polygonPoints);

    // -------door and window---------
    // window
    glColor3f(180.0f/255.0f, 245.0f/255.0f, 214.0f/255.0f);
    polygonPoints = {
        {12.5f, -7.0f},
        {12.5f, -4.0f},
        {15.5f, -4.0f},
        {15.5f, -7.0f},
    };drawFilledPolygon (polygonPoints);

    polygonPoints = {
        {24.5f, -7.0f},
        {24.5f, -4.0f},
        {27.5f, -4.0f},
        {27.5f, -7.0f},
    };drawFilledPolygon (polygonPoints);

    polygonPoints = {
        {15.0f, 3.0f},
        {15.0f, 6.0f},
        {17.5f, 6.0f},
        {17.5f, 3.0f},
    };drawFilledPolygon (polygonPoints);

    polygonPoints = {
        {18.5f, 3.0f},
        {18.5f, 6.0f},
        {21.0f, 6.0f},
        {21.0f, 3.0f},
    };drawFilledPolygon (polygonPoints);

    polygonPoints = {
        {22.0f, 3.0f},
        {22.0f, 6.0f},
        {24.5f, 6.0f},
        {24.5f, 3.0f},
    };drawFilledPolygon (polygonPoints);

    // door----
    glColor3f(205.0f/255.0f, 205.0f/255.0f, 207.0f/255.0f);
    polygonPoints = {
        {18.5f, -10.0f},
        {18.5f, -5.0f},
        {21.5f, -5.0f},
        {21.5f, -10.0f},
    };drawFilledPolygon (polygonPoints);

}

void sky(){
    // Base colors
    float skyDayR = 1.0f, skyDayG = 0.835f, skyDayB = 0.541f;
    float skyRainR = 0.55f, skyRainG = 0.55f, skyRainB = 0.6f;
    float skyNightR = 0.05f, skyNightG = 0.05f, skyNightB = 0.15f;
    float skySnowR = 0.6f, skySnowG = 0.8f, skySnowB = 1.0f;

    // Blend day with rain first
    float dayR = skyDayR + rainBlend * (skyRainR - skyDayR);
    float dayG = skyDayG + rainBlend * (skyRainG - skyDayG);
    float dayB = skyDayB + rainBlend * (skyRainB - skyDayB);

    // Interpolate between day (with rain) and night using dayNightBlend
    float skyR = dayR * dayNightBlend + skyNightR * (1.0f - dayNightBlend);
    float skyG = dayG * dayNightBlend + skyNightG * (1.0f - dayNightBlend);
    float skyB = dayB * dayNightBlend + skyNightB * (1.0f - dayNightBlend);

    //blend with snow
    skyR = skyR * (1.0f - snowIntensity) + skySnowR * snowIntensity;
    skyG = skyG * (1.0f - snowIntensity) + skySnowG * snowIntensity;
    skyB = skyB * (1.0f - snowIntensity) + skySnowB * snowIntensity;

    glBegin(GL_QUADS);
        glColor3f(skyR, skyG, skyB);
        glVertex3f(-65,-10,0.0);
        glVertex3f(-65,35,0);
        glVertex3f(65,35,0);
        glVertex3f(65,-10,0);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // sky
    sky();
    drawBirds();
    // sun and cloud
    //drawCircle(-35, 27, 5, 0.886 , 0.416 , 0.200, 0, 360);
    if (dayNightBlend > 0.1f) {
        drawSunGlobe(-35, 27, 5);
    }
    drawCloud(0, 18, cloud1_tx);
    drawCloud(0, 21, cloud2_tx);
    drawCloud(0, 23, cloud3_tx);

    // turbine
    //tasin er Start
    glLoadIdentity();
    glTranslatef(0,0,0.0f);
    glRotatef(0,0.0f,0.0f,1.0f);
    glScalef(1,1,1.0f);
    //Middle line
    glBegin(GL_LINES);
        glColor3f(1.0,1.0,1.0);
        glVertex3f(-65.0,-10.0, 0.0f);
        glVertex3f(65.0,-10.0, 0.0f);
    glEnd();
    //glColor3f(0.0,1.0,0.0);
    drawBezier(-63.5,-10.0,-50.0,38.0,-33.0,-10.0,0.016,0.365,0.294);
    drawBezier(-65.0,-10.0,-52.5,5.0,-45,-10,0.22,0.60,0.525);
    drawBezier(-45.0,-10.0,-32.5,18.0,-20.0,-10.0,0.23137, 0.44314, 0.14902);
    drawBezier(-20.0,-10.0,-5.0,16.0,6.0,-10.0,0.53725, 0.64314, 0.19216);
    drawBezier(6.0,-10.0,23.5,38.0,35.0,-10.0,0.016,0.365,0.294);
    drawBezier(35.0,-10.0,54.0,38.0,65.0,-10.0,0.23137, 0.44314, 0.14902);

    //Turbine
    drawTurbine(-20, -10);
    drawTurbine(35, -10);

    //Upper Ground
    glBegin(GL_POLYGON);
        float dayR = 0.1f, dayG = 0.5f, dayB = 0.0f;
        float nightR = 0.05f, nightG = 0.2f, nightB = 0.05f;

        float r = dayR * dayNightBlend + nightR * (1.0f - dayNightBlend);
        float g = dayG * dayNightBlend + nightG * (1.0f - dayNightBlend);
        float b = dayB * dayNightBlend + nightB * (1.0f - dayNightBlend);

        glColor3f(r,g,b);
        glVertex3f(-65.0f,-12.0f,0.0f);
        glVertex3f(65.0f,-12.0f,0.0f);
        glVertex3f(65.0f,-10.0f,0.0f);
        glVertex3f(-65.0f,-10.0f,0.0f);
    glEnd();

    drawTree(-58.0,-10.0);
    drawTree(-22.5,-10.0);
    drawTree(3.5,-10.0);
    drawTree(60.0,-10.0);
    Home();
    // lower body

    glBegin(GL_POLYGON);
      glColor3f(0.2f, 0.5f, 0.9f);
      glVertex3f(-65.0f, -12.0f, 0.0f);
      glVertex3f(65.0f, -12.0f, 0.0f);

      glColor3f(0.4f, 0.7f, 1.0f);
      glVertex3f(65.0f, -26.0f, 0.0f);
      glVertex3f(-65.0f, -26.0f, 0.0f);
      glEnd();

      // Ground
    glBegin(GL_POLYGON);
        float groundTopR = dayNightBlend * 0.1f + (1 - dayNightBlend) * 0.05f;
        float groundTopG = dayNightBlend * 0.5f + (1 - dayNightBlend) * 0.2f;
        float groundTopB = dayNightBlend * 0.0f + (1 - dayNightBlend) * 0.05f;

        float groundBottomR = dayNightBlend * 0.4f + (1 - dayNightBlend) * 0.1f;
        float groundBottomG = dayNightBlend * 0.8f + (1 - dayNightBlend) * 0.3f;
        float groundBottomB = dayNightBlend * 0.0f + (1 - dayNightBlend) * 0.1f;

        glColor3f(groundTopR, groundTopG, groundTopB);
        glVertex3f(-65.0f, -26.0f, 0.0f);
        glVertex3f(65.0f, -26.0f, 0.0f);
        glColor3f(groundBottomR, groundBottomG, groundBottomB);
        glVertex3f(65.0f, -35.0f, 0.0f);
        glVertex3f(-65.0f, -35.0f, 0.0f);
    glEnd();
    drawBoat(tx);

    glLoadIdentity();
    glTranslatef(0, 0, 0);
    glScalef(1, 1, 1);
    glRotatef(0, 0, 0, 1);
    drawCirclem(-52.0f, -20.0f, 2.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_POLYGON);
        glVertex3f(-51.5f, -22.0f, 0.0f);
        glVertex3f(-51.1f, -22.8f, 0.0f);
        glVertex3f(-50.7f, -23.6f,0.0f);
        glVertex3f(-50.2f, -24.4f, 0.0f);
        glVertex3f(-50.0f, -25.2f,0.0f);
        glVertex3f(-50.2f, -26.0f,0.0f);
        glVertex3f(-50.7f, -26.4f,0.0f);
        glVertex3f(-50.4f, -26.8f,0.0f);
        glVertex3f(-50.5f, -27.0f,0.0f);
        glVertex3f(-49.7f, -27.0f,0.0f);
        glVertex3f(-53.1f, -27.0f,0.0f);
        glVertex3f(-53.5f, -27.0f,0.0f);
        glVertex3f(-53.8f, -27.3f,0.0f);
        glVertex3f(-54.3f, -27.2f,0.0f);
        glVertex3f(-54.4f, -27.4f,0.0f);
        glVertex3f(-54.3f, -26.8f,0.0f);
        glVertex3f(-53.8f, -26.0f,0.0f);
        glVertex3f(-54.0f, -25.2f,0.0f);
        glVertex3f(-53.8f, -24.4f,0.0f);
        glVertex3f(-53.3f, -23.6f,0.0f);
        glVertex3f(-52.9f, -22.8f,0.0f);
        glVertex3f(-52.5f, -22.0f,0.0f);
    glEnd();
    // lungi
    drawCirclem(-53.0f, -27.0f, 1.5f, 0.55f, 0.27f, 0.07f, 180.0f, 360.0f);
    drawCirclem(-51.0f, -27.0f, 1.5f, 0.55f, 0.27f, 0.07f, 180.0f, 360.0f);

    // borshi-
    glLineWidth(3.0f);
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_LINES);
        glVertex3f(-51.0f, -23.0f,0.0f);
        glVertex3f(-45.0f, -20.0f,0.0f);
    glEnd();
    glLineWidth(1.0f);

      glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(-45.0f, -20.0f, 0.0f);
    glVertex3f(-40.0f, -23.0f, 0.0f);

    glEnd();
    // ghamla
    drawGhamla(0.0f, -27.0f);

    drawCow(-20.0f, -30.0f, cowTx);
    glLoadIdentity();
    drawSnow();
    drawGroundSnow();

    //drawCow(-20.0f, -30.0f);
    glLoadIdentity();
    if (rainIntensity > 0.0f) {
        drawRain();
    }

    // plane er part
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(60.0, 1300.0/700.0, 1.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -80.0f);
    glTranslatef(planeX, planeY, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(15.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
    glScalef(4.0f, 4.0f, 4.0f);

    drawAeroplane();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Village Sinary");
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(50,50);
    glutDisplayFunc(display);
    initGL();
    initRain();
    initSnow();


    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutReshapeFunc(reshape);

    glutTimerFunc(0, timer, 0);
    glutMainLoop();


    return 0;
}
