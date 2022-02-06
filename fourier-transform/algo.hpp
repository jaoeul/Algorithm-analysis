#ifndef ALGO_HPP
#define ALGO_HPP

namespace algo {

// Nmuber of samples in the waves and signals.
#define N 200

const int   SCREEN_WIDTH      = 1800;
const int   SCREEN_HEIGHT     = 900;
const float SCREEN_WIDTH_MID  = SCREEN_WIDTH  / 2;
const float SCREEN_HEIGHT_MID = SCREEN_HEIGHT / 2;

typedef struct {
    float x;
    float y;
} v2_t;

void draw_coordinate_system(float screen_width, float screen_height, int unit_size);
std::vector<v2_t>* generate_sine_wave(float theta, float amplitude);
std::vector<v2_t>* wrap_signal(std::vector<v2_t>& signal);

} // namespace algo

# endif
