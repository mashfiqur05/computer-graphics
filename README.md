# Village Scenery Using Computer Graphics

This project is an animated **Village Scenery simulation** built using **C++, OpenGL, and GLUT**.  
It demonstrates real-time environmental changes such as **day-night transitions, weather effects, and object animations** in a rural setting.

---

## Overview

The project simulates a dynamic village environment with interactive features.  
Users can control weather conditions and time of day using keyboard inputs while observing smooth animations of natural and man-made objects.

---

## Objectives

- Implement OpenGL and GLUT for graphics rendering  
- Create a complete animated village scenery  
- Simulate **day and night transitions**  
- Implement **rain and snow weather effects**  
- Animate multiple objects (birds, boat, cow, plane, turbine)  
- Apply **2D and 3D transformations**  
- Build an **interactive graphics application**  

---

## Technologies Used

- **Language:** C++  
- **Graphics Library:** OpenGL  
- **Toolkit:** GLUT  
- **Platform:** Windows  
- **IDE:** Code::Blocks / Visual Studio  
- **Compiler:** GCC / MinGW  

---

## Features

###  Day & Night System
- Smooth transition between day and night  
- Dynamic sky color blending  
- Sun visible during day  
- Reduced brightness at night  

---

### Weather System

#### Rain
- Falling raindrops animation  
- Adjustable intensity  
- Dark cloud effect  

#### Snow
- Falling snowflakes  
- Snow accumulation on ground  
- Smooth visual transition  

---

###  Animations
- Birds flying with wing motion  
- Boat moving across river  
- Cow walking naturally  
- Wind turbine rotating  
- 3D airplane flying across sky  

---

### User Interaction

| Key | Action |
|-----|--------|
| R   | Toggle Rain |
| S   | Toggle Snow |
| D   | Day Mode |
| N   | Night Mode |

---

## System Components

- Sky System (day, night, weather blending)  
- Sun & Cloud Animation  
- Weather Engine (rain & snow)  
- Village Elements (houses, trees, river)  
- Moving Objects (boat, cow, birds, airplane)  
- Wind Turbine Animation  
- Keyboard Interaction System  

---

## Implementation Details

- `display()` → Renders the full scene  
- `timer()` → Controls continuous animation  
- Separate functions for each object (tree, cow, cloud, etc.)  
- Uses both **orthographic and perspective projection**  
- Weather intensity controlled using blending techniques  
- Keyboard callbacks manage interaction  

---

## Algorithms & Techniques Used

- DDA Line Drawing Algorithm  
- Midpoint Line Algorithm  
- Midpoint Circle Algorithm  
- Bezier Curve Algorithm  
- Scanline Polygon Filling  
- 2D & 3D Transformations  
- Timer-based Animation  
- Color Interpolation (Blending)  

---

## How to Run

### Prerequisites

- OpenGL  
- GLUT / freeGLUT  
- C++ Compiler (g++)  

---

### ▶️ Steps

1. Clone the repository:
```bash
git clone https://github.com/mashfiqur05/computer-graphics.git 
```
2. Navigate to project folder:
```bash
cd computer-graphics
```
3. Compile:
```bash
g++ main.cpp -lGL -lGLU -lglut -o output
```
4.Run
```bash
./output
```

### Demo Workflow
- Run the program → Village scene appears
- Press D → Day mode
- Press N → Night mode
- Press R → Rain effect
- Press S → Snow effect
- Observe continuous animations (birds, boat, turbine, plane, cow)