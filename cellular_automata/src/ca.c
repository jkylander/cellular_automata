#include <raylib.h>

#define CELL_WIDTH 16
#define CELL_LENGTH 10
#define SCREEN_WIDTH 810
#define SCREEN_HEIGHT 810

#define TOTAL_CELLS SCREEN_WIDTH / CELL_LENGTH

int cells[TOTAL_CELLS];
int nextCells[TOTAL_CELLS];

void setup(void) {
    cells[TOTAL_CELLS / 2 ] = 1;
}

// Update cell state
int calculate_state(int a, int b, int c) {
    if (a == 1 && b == 1 && c == 1) return 1;
    if (a == 1 && b == 1 && c == 0) return 0;
    if (a == 1 && b == 0 && c == 1) return 1;
    if (a == 1 && b == 0 && c == 0) return 1;
    if (a == 0 && b == 1 && c == 1) return 0;
    if (a == 0 && b == 1 && c == 0) return 1;
    if (a == 0 && b == 0 && c == 1) return 1;
    if (a == 0 && b == 0 && c == 0) return 0;
    return 0;
}
void drawCells(int y) {

    int x = 0;
    for (int i = 0; i < TOTAL_CELLS; i++) {
        x = i * CELL_WIDTH;
        // Draw cells, 1 is black, 0 is white
        DrawRectangle(
            x,
            y,
            CELL_WIDTH,
            CELL_WIDTH,
            (Color) {255 - 255*cells[i], 255 -255*cells[i], 255 - 255*cells[i], 255}
        );
        DrawRectangleLines(i * CELL_WIDTH, 0, CELL_WIDTH, CELL_WIDTH, BLACK);
    }
}
void updateCells(void) {

    nextCells[0] = cells[0];
    nextCells[TOTAL_CELLS - 1] = cells[TOTAL_CELLS - 1];
    for (int i = 1; i < TOTAL_CELLS - 1; i++) {
        int left = cells[i - 1];
        int right = cells[i + 1];
        int state = cells[i];
        int newState = calculate_state(left, state, right);
        nextCells[i] = newState;
    }
    for (int i = 0; i < TOTAL_CELLS; i++){
        cells[i] = nextCells[i];
    }
}
int main(void) {
    // Window loop
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cellular Automata");
    int y = 0;
    setup();
    while(!WindowShouldClose())
    {
        ClearBackground(RAYWHITE);
        BeginDrawing();
            drawCells(y);
            updateCells();
            // Move down 1 row
            //y += CELL_WIDTH;
        EndDrawing();
    }
    return 0;
}
