#ifndef CONSTANTS_H
#define CONSTANTS_H

static constexpr float CELL_SIZE = 64.0f;
static constexpr float PLAYER_SIZE = 16.0f;

static constexpr float RECT_HEIGHT = 600.0f;

static constexpr float SCREEN_HEIGHT = 600.0f;
static constexpr float SCREEN_WIDTH = 800.0f;

static constexpr unsigned int NUM_RAYS = 256;
static constexpr float FOV = 60.0f;
static constexpr float DELTA_THETA = FOV / NUM_RAYS;

#endif