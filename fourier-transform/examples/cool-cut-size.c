#include <math.h>
#include <raylib.h>
#include <stdio.h>

const int screen_width  = 1800;
const int screen_height = 900;

const float screen_mid_x = screen_width  / 2;
const float screen_mid_y = screen_height / 2;

const int   PIXELS_PER_UNIT           = 20;
const int   UNIT                      = PIXELS_PER_UNIT;
const float UNIT_MARKER_LENGTH_PIXELS = 10;
const float SIN_AMPLITUDE             = 10 * UNIT;


float user_delta = 0;
float cut_size   = 100;

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
update_drawing(void)
{
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
        if (IsKeyDown(KEY_RIGHT)) {
            user_delta += 0.2f;
            cut_size   += 0.5f;
        }
        if (IsKeyDown(KEY_LEFT)) {
            user_delta -= 0.2f;
            cut_size   -= 0.5f;
        }

        BeginDrawing();

        DrawText("Change frequency with arrow keys", 10, 10, 20, DARKGRAY);

        ClearBackground(RAYWHITE);

        draw_coordinate_system();

        for (float x = 0; x < cut_size; x++) {

            float theta      = x / (30 + user_delta);
            float sin_y      = (sinf(theta) * SIN_AMPLITUDE);
            float sin_plot_y = screen_mid_y + sin_y;
            //DrawPixel(x, y, RED);
            DrawCircle(x, sin_plot_y, 3, RED);

            //char freq_text[20] = {0};
            //DrawText(freq_text, 10, 40, 20, DARKGRAY);

            // Spin graph around a point.
            float spin_origo_x = screen_mid_x;
            float spin_origo_y = screen_mid_y + 10 * UNIT;

            float spin_x = spin_origo_x + (sinf(x) * sin_y);
            float spin_y = spin_origo_y + (cosf(x) * sin_y);

            printf("sin_y: %.0f\n", sin_y);
            printf("x: %.0f\n", x);

            DrawCircle(spin_x, spin_y, 3, BLUE);

        }
        //----------------------------------------------------------------------------------
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
