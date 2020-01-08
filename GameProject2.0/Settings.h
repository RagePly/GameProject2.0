#pragma once
#include <iostream>
#include <math.h>
#include <limits>
#include <cmath>
#include <typeinfo>


/*
Window settings
1920 x 1080
1280 x 720
320
200
*/
#define WIDTH 1280
#define HEIGHT 720
#define NUMBER_OF_OBJECTS 2
#define FULLSCREEN false
#define TESTING false

/*
World settings
*/
#define WORLD_EXPAND_INCREMENT 10
#define WORLD_STARTING_SIZE 10

/*
Ray Settings
*/

#define STEP_LENGTH_THRESHOLD 0.1f
#define ESCAPE_THRESHOLD 1.02f
#define PERCENT_WITHDRAWAL 0.99;
#define MAX_LENGTH 5000
#define MAX_BOUNCE_LIMIT 100


enum shape_id {
	id_empty,
	id_sphere,
	id_total
};
