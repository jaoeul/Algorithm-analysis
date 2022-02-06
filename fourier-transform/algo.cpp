#include <iostream>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <vector>

#include "algo.hpp"

using std::cout;
using std::endl;

namespace algo {

void
draw_coordinate_system(float screen_width, float screen_height, int unit_size)
{
    const int unit_marker_size = unit_size / 2;

    // Draw coordinate system lines.
    for (float i = 0; i < screen_width; i++) {
        DrawPixel(i, SCREEN_HEIGHT_MID, GRAY);
    }
    for (float i = 0; i < screen_height; i++) {
        DrawPixel(SCREEN_WIDTH_MID, i, GRAY);
    }

    // Draw unit markers.
    for (float i = 0; i < SCREEN_WIDTH_MID; i++) {
        if ((int)i % unit_size == 0) {
            for (float j = -unit_marker_size / 2; j < unit_marker_size / 2; j++) {
                DrawPixel(SCREEN_WIDTH_MID + i, SCREEN_HEIGHT_MID + j, GRAY);
                DrawPixel(SCREEN_WIDTH_MID - i, SCREEN_HEIGHT_MID + j, GRAY);
            }
        }
    }
    for (float i = 0; i < SCREEN_HEIGHT_MID; i++) {
        if ((int)i % unit_size == 0) {
            for (float j = -(unit_marker_size / 2); j < unit_marker_size / 2; j++) {
                DrawPixel(SCREEN_WIDTH_MID + j, SCREEN_HEIGHT_MID + i, GRAY);
                DrawPixel(SCREEN_WIDTH_MID + j, SCREEN_HEIGHT_MID - i, GRAY);
            }
        }
    }
}

std::vector<v2_t>*
wrap_signal(std::vector<v2_t>& signal)
{
    std::vector<v2_t>* wrapped = new std::vector<v2_t>();

    for (int i = 0; i < signal.size(); i++) {
        v2_t entry = {
            .x = sinf(i) * signal[i].y,
            .y = cosf(i) * signal[i].y,
        };
        wrapped->push_back(entry);
    }

    return wrapped;
}

std::vector<v2_t>*
generate_sine_wave(float theta_base, float amplitude)
{
    std::vector<v2_t>* wave = new std::vector<v2_t>();
    for (float x = 0; x < N; x++) {
        v2_t entry = {
            .x = x,
            .y = (sinf(x * theta_base) * amplitude),
        };
        wave->push_back(entry);
    }
    return wave;
}

} // namespace algo
