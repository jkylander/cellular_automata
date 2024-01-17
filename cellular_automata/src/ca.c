#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CELL_WIDTH 4
#define SCREEN_WIDTH 810
#define SCREEN_HEIGHT 810

#define TOTAL_CELLS SCREEN_WIDTH / CELL_WIDTH
const uint8_t rule_value = 110;

int cells[TOTAL_CELLS];
int nextCells[TOTAL_CELLS];
void reverse(char*);
char* int_to_binStr(uint8_t);
int binStr_to_int(const char*);

// Set only the middle cell to 1
void setup(void) {
    cells[ TOTAL_CELLS / 2 ] = 1;
}

// Update cell state
int calculate_state(int a, int b, int c, const char* rule) {
    char s[4] = {a + '0', b + '0', c + '0', '\0'};
    int index = binStr_to_int(s);
    return rule[7 - index] - '0';
}

// Draw one row of cells
void draw_cells(int y) {
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
    }
}

// Update state for all cells
void update_cells(const char* rule) {
    // Wraparound
    nextCells[0] = calculate_state(cells[TOTAL_CELLS -1], cells[0], cells[1], rule);
    nextCells[TOTAL_CELLS - 1] = calculate_state(cells[TOTAL_CELLS -2], cells[TOTAL_CELLS -1], cells[0], rule);

    for (int i = 1; i < TOTAL_CELLS - 1; i++) {
        int left = cells[i - 1];
        int right = cells[i + 1];
        int state = cells[i];
        int newState = calculate_state(left, state, right, rule);
        nextCells[i] = newState;
    }
    for (int i = 0; i < TOTAL_CELLS; i++){
        cells[i] = nextCells[i];
    }
}

// Convert int to string
char *int_to_binStr(uint8_t num) {
    static char binStr[9];
    snprintf(binStr, sizeof(binStr), "%08b", num);
    return binStr;
}
// Convert string to int
int binStr_to_int(const char* str) {
    int len = strlen(str);
    int result = 0;

    for (int i = 0; i < len; i++) {
        if (str[len - 1 - i] != '0' && str[len - 1 - i] != '1') {
            return -1; // Invalid binary number
        }

        result += (str[len - 1 - i] - '0') * (1 << i);
    }
    return result;
}
void render_ca(const char* rule) {
    // Window loop
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Elementary Cellular Automata");
    int y = 0;
    setup();
    while(!WindowShouldClose())
    {
        /* ClearBackground(RAYWHITE); */
        BeginDrawing();
            draw_cells(y);
            update_cells(rule);
            // Move down 1 row
            y += CELL_WIDTH;
        EndDrawing();
    }
}
int main(void) {

    const char *rule = int_to_binStr(rule_value);
    render_ca(rule);
    return 0;
}
