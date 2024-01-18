#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELL_WIDTH 2
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 810
#define MAX_INPUT_CHARS 3
#define TOTAL_CELLS SCREEN_WIDTH / CELL_WIDTH
#define MIDDLE(a, b) (a / 2.0f - b / 2.0f)

// Differnt views
typedef enum Screen { LOGO = 0, TITLE, RENDER } Screen;

uint8_t rule_value = 60;

int cells[TOTAL_CELLS];
int nextCells[TOTAL_CELLS];

// Set only the middle cell to 1
void setup(void) {
    for (int i = 0; i < TOTAL_CELLS - 1; i++) {
        cells[i] = 0;
    }
    cells[TOTAL_CELLS / 2] = 1;
}

// Update cell state
int calculate_state(int a, int b, int c) {
    int index = (a << 2) + (b << 1) + c;
    return (rule_value >> index) & 1;
}

// Draw one row of cells
void draw_cells(int y) {
    int x = 0;
    for (int i = 0; i < TOTAL_CELLS; i++) {
        x = i * CELL_WIDTH;
        // Draw cells, 1 is black, 0 is white
        DrawRectangle(x, y, CELL_WIDTH, CELL_WIDTH,
                      (Color) {
                          255 - 255 * cells[i],
                          255 - 255 * cells[i],
                          255 - 255 * cells[i],
                          255
                      });
    }
}

// Update state for all cells
void update_cells(void) {
    // Wraparound
    nextCells[0] = calculate_state(cells[TOTAL_CELLS - 1], cells[0], cells[1]);
    nextCells[TOTAL_CELLS - 1] = calculate_state(
        cells[TOTAL_CELLS - 2], cells[TOTAL_CELLS - 1], cells[0]);
    // Calculate neighbors
    for (int i = 1; i < TOTAL_CELLS - 1; i++) {
        int left = cells[i - 1];
        int right = cells[i + 1];
        int state = cells[i];
        int newState = calculate_state(left, state, right);
        nextCells[i] = newState;
    }
    // Apply to current cells
    for (int i = 0; i < TOTAL_CELLS; i++) {
        cells[i] = nextCells[i];
    }
}

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Elementary Cellular Automata");
    Screen currentScreen = LOGO;
    Texture2D title_image = LoadTexture("cellular_automata_paths.png");
    // Initalize vars
    int y = 0;
    char rule[MAX_INPUT_CHARS + 1] = {'\0'};
    int letterCount = 0;
    Rectangle textBox = {SCREEN_WIDTH / 2.0f - 100, 180, 225, 50};
    bool mouseOnText = false;
    int framesCounter = 0;
    SetTargetFPS(120);

    // Main loop
    while (!WindowShouldClose()) {
        switch (currentScreen) {

        case LOGO: {
            framesCounter++;
            if (framesCounter > 240) {
                framesCounter = 0;
                currentScreen = TITLE;
            }
        } break;

        case TITLE: {
            // Input box
            int key = GetCharPressed();
            while (key > 0) {
                // NOTE: Only allow digits
                if ((key >= '0') && (key <= '9') &&
                    (letterCount < MAX_INPUT_CHARS)) {
                    rule[letterCount] = (char)key;
                    rule[letterCount + 1] = '\0';
                    letterCount++;
                }
                key = GetCharPressed(); // check next char
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                rule[letterCount] = '\0';
            }
            framesCounter++;
            rule_value = atoi(rule);
            // Next case
            if (IsKeyPressed(KEY_ENTER)) {
                framesCounter = 0;
                SetTargetFPS(1000);
                currentScreen = RENDER;
            }
        } break;

        case RENDER: {
            if (IsKeyPressed(KEY_ENTER)) {
                SetTargetFPS(120);
                rule[0] = '\0';
                letterCount = 0;
                currentScreen = TITLE;
            }
        } break;

        default:
            break;
        }

        BeginDrawing();
        switch (currentScreen) {
        case LOGO: {
            ClearBackground(RAYWHITE);
            DrawTexture(title_image, MIDDLE(SCREEN_WIDTH, title_image.width),
                        MIDDLE(SCREEN_HEIGHT, title_image.height), WHITE);
        } break;

        case TITLE: {
            y = 0;
            setup();
            ClearBackground(RAYWHITE);
            char *title = "Elementary Cellular Automata";
            char *description = "Press Enter to start";

            /*     RLAPI Vector2 MeasureTextEx(Font font, const char *text,
             * float fontSize, float spacing);    // Measure string size for
             * Font
             */
            DrawText(title, MIDDLE(SCREEN_WIDTH, MeasureText(title, 40)),
                     SCREEN_HEIGHT / 4.0f - 100, 40, DARKGREEN);
            DrawText(description,
                     MIDDLE(SCREEN_WIDTH, MeasureText(description, 30)),
                     SCREEN_HEIGHT / 4 + 100, 30, DARKGREEN);
            DrawRectangleRec(textBox, LIGHTGRAY);

            DrawRectangleLines(textBox.x, textBox.y, textBox.width,
                               textBox.height, DARKGRAY);

            DrawText(rule, textBox.x + 5, textBox.y + 8, 40, MAROON);
            char *enter = "Enter a number between 0-255:";
            DrawText(TextFormat(enter), MeasureText(enter, 20) / 2, 200, 20,
                     DARKGRAY);
            if (letterCount < MAX_INPUT_CHARS) {
                // Draw blinking underscore
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(rule, 40),
                             (int)textBox.y + 12, 40, MAROON);
            }
        } break;
        case RENDER: {
            draw_cells(y);
            update_cells();
            // Move down 1 row
            y += CELL_WIDTH;
        } break;
        default:
            break;
        }
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(title_image);
    CloseWindow();

    return 0;
}
