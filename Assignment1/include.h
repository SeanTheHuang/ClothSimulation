#pragma once

//Internal Includes
#include <conio.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>

//Binary File Includes
#include <glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\random.hpp>

#include <glew.h>
#include <freeglut.h>

//Global Project Includes
#include "time.h"
#include "input.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BUFFER_OFFSET(i) ((GLvoid *)(i*sizeof(float)))
