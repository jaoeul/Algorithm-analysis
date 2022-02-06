// TODO: Create a single signal, containing multiple sine waves and some noise, and plot it.
//       Try to change the theta base of the signal, to see if that creates a valid FFT pattern
//       in the wrapping graph.
#include <math.h>
#include <raylib.h>
#include <stdio.h>

// Constanst.
#define N 2000

const int screen_width  = 1800;
const int screen_height = 900;

const float screen_mid_x = screen_width  / 2;
const float screen_mid_y = screen_height / 2;

const int   PIXELS_PER_UNIT           = 20;
const int   UNIT                      = PIXELS_PER_UNIT;
const float UNIT_MARKER_LENGTH_PIXELS = 10;
const float SIN_AMPLITUDE             = 5 * UNIT;


// Mutable globals.
float user_delta = 0;
float cut_size   = 0;

static void
draw_coordinate_system(void)
{
    // Draw coordinate system lines.
    for (float i = 0; i < screen_width; i++) {
        DrawPixel(i, screen_mid_y, GRAY);
    }
    for (float i = 0; i < screen_height; i++) {
        DrawPixel(screen_mid_x, i, GRAY);
    }

    // Draw unit markers.
    for (float i = 0; i < screen_mid_x; i++) {
        if ((int)i % UNIT == 0) {
            for (float j = -(UNIT_MARKER_LENGTH_PIXELS / 2); j < UNIT_MARKER_LENGTH_PIXELS / 2; j++) {
                DrawPixel(screen_mid_x + i, screen_mid_y + j, GRAY);
                DrawPixel(screen_mid_x - i, screen_mid_y + j, GRAY);
            }
        }
    }
    for (float i = 0; i < screen_mid_y; i++) {
        if ((int)i % UNIT == 0) {
            for (float j = -(UNIT_MARKER_LENGTH_PIXELS / 2); j < UNIT_MARKER_LENGTH_PIXELS / 2; j++) {
                DrawPixel(screen_mid_x + j, screen_mid_y + i, GRAY);
                DrawPixel(screen_mid_x + j, screen_mid_y - i, GRAY);
            }
        }
    }
}

static void
draw_sin_wave(float theta_base, float amplitude, float cut_size, float user_delta)
{
    draw_coordinate_system();
    for (float x = 0; x < 2000; x++) {

        float theta      = x / (theta_base + user_delta);
        float sin_y      = (sinf(theta) * amplitude);
        float sin_plot_y = screen_mid_y + sin_y;

        //DrawPixel(x, y, RED);
        DrawCircle(x, sin_plot_y, 3, RED);

        //char freq_text[20] = {0};
        //DrawText(freq_text, 10, 40, 20, DARKGRAY);
        if (x < cut_size) {
            float spin_origo_x = screen_mid_x;
            float spin_origo_y = screen_mid_y + 10 * UNIT;

            float spin_x = spin_origo_x + (sinf(x) * sin_y);
            float spin_y = spin_origo_y + (cosf(x) * sin_y);

            printf("sin_y: %.0f\n", sin_y);
            printf("x: %.0f\n", x);

            DrawCircle(spin_x, spin_y, 3, BLUE);
        }

        // Spin graph around a point.
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screen_width, screen_height, "raylib [core] example - keyboard input");
    SetTargetFPS(144);
    //--------------------------------------------------------------------------------------

    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Prepare
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_UP)) {
            cut_size   += 5.0f;
        }
        if (IsKeyDown(KEY_DOWN)) {
            cut_size   -= 5.0f;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            user_delta += 0.02f;
        }
        if (IsKeyDown(KEY_LEFT)) {
            user_delta -= 0.02f;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw
        //----------------------------------------------------------------------------------
        DrawText("Change frequency with arrow keys", 10, 10, 20, DARKGRAY);

        char text_cut_size[100] = {0};
        sprintf(text_cut_size, "Cut size: %.0f units\n", cut_size / UNIT);
        DrawText(text_cut_size, 10, 40, 20, DARKGRAY);

        // Draw the sweeping cut size line.
        for (int i = 0; i < 300; i++) {
            DrawPixel(cut_size, (screen_mid_y - 150) + i, BLACK);
            DrawPixel(cut_size - 1, (screen_mid_y - 150) + i, BLACK);
        }

        float signal[N] = {0};

        // Draw some sine waves.
        draw_sin_wave(10, SIN_AMPLITUDE,     cut_size, user_delta);
        //draw_sin_wave(20, SIN_AMPLITUDE * 2, cut_size, user_delta);

        //----------------------------------------------------------------------------------
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
