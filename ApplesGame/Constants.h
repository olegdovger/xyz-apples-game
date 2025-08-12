#pragma once

#include <string>

const std::string RESOURCES_PATH = "Resources/";
const int DEFAULT_WINDOW_WIDTH   = 720;
const int DEFAULT_WINDOW_HEIGHT  = 480;

const float INITIAL_PLAYER_SPEED = 100.f;
const float PLAYER_SPEED_DELTA   = 10.f;
const float PLAYER_SIZE          = 30.f;

const int NUM_APPLES			 = 20;
const float APPLE_SIZE           = 30.f;

const int MODE_FINITE_APPLES     = 0x01;
const int MODE_INFINITE_APPLES   = 0x02;
const int MODE_SPEED_BOOST       = 0x04;
const int MODE_NO_SPEED_BOOST    = 0x08;

const float SPEED_BOOST_FACTOR   = 1.1f;
const int APPLES_FOR_SPEED_BOOST = 3; 