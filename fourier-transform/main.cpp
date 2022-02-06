#include <cstdio>
#include <cfloat>
#include <iostream>
#include <raylib.h>
#include <sstream>
#include <vector>

#include "algo.hpp"

using namespace algo;
using namespace std;

using std::cout;
using std::endl;

// Adds each element of two v2 vectors together. Stops at the end of the shortest vector.
static std::vector<v2_t>*
v2_add(std::vector<v2_t>& a, std::vector<v2_t>& b)
{
    std::vector<v2_t>* res = new std::vector<v2_t>();
    for (float i = 0; i < a.size() && i < b.size(); i++) {
        v2_t entry = {
            .x = a[i].x + b[i].x,
            .y = a[i].y + b[i].y,
        };
        res->push_back(entry);
    }
    return res;
}

static void
draw_signal(std::vector<float>& signal, int unit_size, Color color)
{
    // Draw signal from origno -> x infinity.
    for (int i = 0; i < signal.size() / 2 - 1; i++) {
        DrawLine((i             * unit_size) + SCREEN_WIDTH_MID,
                 (signal[i]     * unit_size) + SCREEN_HEIGHT_MID,
                 (i + 1)        * unit_size  + SCREEN_WIDTH_MID,
                 (signal[i + 1] * unit_size) + SCREEN_HEIGHT_MID,
                 color);
    }
}

static void
draw_signal_v2(std::vector<v2_t>& signal, int unit_size, Color color)
{
    // Draw signal from origno -> x infinity.
    for (int i = 0; i < signal.size() / 2 - 1; i++) {
        DrawLine((signal[i].x      * unit_size) + SCREEN_WIDTH_MID,
                 (signal[i].y      * unit_size) + SCREEN_HEIGHT_MID,
                 (signal[i + 1].x) * unit_size  + SCREEN_WIDTH_MID,
                 (signal[i + 1].y  * unit_size) + SCREEN_HEIGHT_MID,
                 color);
    }
}

static v2_t
signal_sum(std::vector<v2_t>& signal)
{
    v2_t res = v2_t();
    for (size_t i = 0; i < signal.size(); i++) {
        res.x += signal[i].x;
        res.y += signal[i].y;
    }
    return res;
}

int
main(void)
{
    // Mutable globals.
    float user_delta = 0;
    float cut_size   = 0;
    float amplitude  = 10;
    int   unit_size  = 50;

    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - keyboard input");
    SetTargetFPS(144);

    // Main loop.
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Input checking.
        if (IsKeyDown(KEY_UP) && cut_size < FLT_MAX - 5.0f) {
            cut_size   += 5.0f;
        }
        if (IsKeyDown(KEY_DOWN) && cut_size > (-FLT_MAX - 5.0f)) {
            cut_size   -= 5.0f;
        }

        if (IsKeyDown(KEY_RIGHT) && user_delta < FLT_MAX - 1.0f) {
            user_delta += 0.001f;
        }
        if (IsKeyDown(KEY_LEFT) && user_delta > (-FLT_MAX - 1.0f)) {
            user_delta -= 0.001f;
        }

        // Zooming.
        if (GetMouseWheelMove() > 0 && unit_size < FLT_MAX - 1.0f) {
            unit_size++;
        }
        // Cap scrolling out to units of 1 pixel small.
        else if (GetMouseWheelMove() < 0 && unit_size > 1.0f) {
            unit_size--;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw
        //----------------------------------------------------------------------------------
        algo::draw_coordinate_system(SCREEN_WIDTH, SCREEN_HEIGHT, unit_size);

        DrawText("Change frequency with arrow keys", 10, 10, 20, DARKGRAY);

        std::stringstream text_cut_size;
        text_cut_size << "Cut size: " << cut_size / unit_size << " units" << endl;
        DrawText(text_cut_size.str().c_str(), 10, 40, 20, DARKGRAY);

        std::stringstream text_user_delta;
        text_user_delta << "User delta: " << user_delta;
        DrawText(text_user_delta.str().c_str(), 10, 60, 20, DARKGRAY);

        std::stringstream text_cursor_pos;
        text_cursor_pos << "Cursor x: " << -((SCREEN_WIDTH_MID  - GetMouseX()) / unit_size) <<
                           " y: "       <<  ((SCREEN_HEIGHT_MID - GetMouseY()) / unit_size);
        DrawText(text_cursor_pos.str().c_str(), 10, 80, 20, DARKGRAY);

        // Draw the sweeping cut size line.
        for (int i = 0; i < 40; i++) {
            DrawPixel(cut_size, (SCREEN_HEIGHT_MID     - 20) + i, BLACK);
            DrawPixel(cut_size + 1, (SCREEN_HEIGHT_MID - 20) + i, BLACK);
        }

        // Generate some waves.
        std::vector<v2_t>* sin1 = generate_sine_wave(1 + user_delta, amplitude); // Period: 2 PI
        std::vector<v2_t>* sin2 = generate_sine_wave(2 + user_delta, amplitude); // Period: PI
        std::vector<v2_t>* sin3 = generate_sine_wave(4 + user_delta, amplitude); // Period: ~2.7

        // Add the waves together and draw the result.
        std::vector<v2_t>* signal = v2_add(*sin1,   *sin2);
        signal                    = v2_add(*signal, *sin3);
        draw_signal_v2(*sin3, unit_size, RED);

        // Wrap the signal around origo and draw it.
        std::vector<v2_t>* wrapped = wrap_signal(*signal);
        draw_signal_v2(*wrapped, unit_size, BLUE);

        // Calculate the sum of the wrapped signal and draw it.
        v2_t wrap_sum = signal_sum(*wrapped);
        DrawCircle(SCREEN_WIDTH_MID  + (wrap_sum.x / 10) * unit_size,
                   SCREEN_HEIGHT_MID + (wrap_sum.y / 10) * unit_size,
                   10, DARKBLUE);

        //----------------------------------------------------------------------------------
        delete sin1;
        delete sin2;
        delete sin3;
        delete signal;
        delete wrapped;
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
