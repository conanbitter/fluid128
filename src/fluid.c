#include "fluid.h"
#include <stdint.h>

typedef struct Cell {
    float u;
    float v;
    int solid;
    float smoke;
    float new_u;
    float new_v;
} Cell;

static Cell field[FIELD_HEIGHT][FIELD_WIDTH];

#define TUBE_MIN ((FIELD_HEIGHT-FIELD_HEIGHT/3)/2)
#define TUBE_MAX ((FIELD_HEIGHT-FIELD_HEIGHT/3)/2+FIELD_HEIGHT/3)

void fluid_init() {
    for (int y = 0;y < FIELD_HEIGHT;y++) {
        for (int x = 0;x < FIELD_WIDTH;x++) {
            int solid = x == 0 || y == 0 || y == FIELD_HEIGHT - 1 ? 1 : 0;
            field[y][x].u = x == 1 && solid == 0 ? 2.0f : 0.0f;
            field[y][x].v = 0.0f;
            field[y][x].solid = solid;
            field[y][x].smoke = x == 1 && y >= TUBE_MIN && y <= TUBE_MAX ? 1.0f : 0.0f;
        }
    }
}

void fluid_draw(char* data) {
    for (int y = 0;y < FIELD_HEIGHT;y++) {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            size_t index = (x + y * FIELD_WIDTH) * 4;
            data[index] = 255;

            if (field[y][x].solid == 1) {
                data[index + 1] = 50;
                data[index + 2] = 200;
                data[index + 3] = 50;
            } else {
                int color = field[y][x].smoke * 255;
                if (color < 0) color = 0;
                if (color > 255) color = 255;

                data[index + 1] = color;
                data[index + 2] = color;
                data[index + 3] = color;
            }
        }
    }
}

void fluid_update(float dt) {

}