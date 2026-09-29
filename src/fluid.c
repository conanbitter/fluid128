#include "fluid.h"
#include <stdint.h>

typedef struct Cell {
    float u;
    float v;
    int fluid;
    float smoke;
    float new_u;
    float new_v;
} Cell;

static Cell field[FIELD_HEIGHT][FIELD_WIDTH];

#define TUBE_MIN ((FIELD_HEIGHT-FIELD_HEIGHT/3)/2)
#define TUBE_MAX ((FIELD_HEIGHT-FIELD_HEIGHT/3)/2+FIELD_HEIGHT/3)
#define ITER_MAX (100)
#define OVERRELAXATION (1.9)

void fluid_init() {
    for (int y = 0;y < FIELD_HEIGHT;y++) {
        for (int x = 0;x < FIELD_WIDTH;x++) {
            int fluid = x == 0 || y == 0 || y == FIELD_HEIGHT - 1 ? 0 : 1;
            //field[y][x].u = x == 1 && fluid == 1 ? 2.0f : 0.0f;
            field[y][x].v = 0.0f;
            field[y][x].fluid = fluid;
            field[y][x].smoke = x == 1 && y >= TUBE_MIN && y <= TUBE_MAX ? 1.0f : 0.0f;
            field[y][x].u = x == 1 && field[y][x].smoke > 0.5 ? 2.0f : 0.0f;
        }
    }
}

void fluid_draw(char* data) {
    for (int y = 0;y < FIELD_HEIGHT;y++) {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            size_t index = (x + y * FIELD_WIDTH) * 4;
            data[index] = 255;

            if (field[y][x].fluid == 0) {
                data[index + 1] = 50;
                data[index + 2] = 200;
                data[index + 3] = 50;
            } else {
                /*int color = field[y][x].smoke * 255;
                if (color < 0) color = 0;
                if (color > 255) color = 255;*/
                int cu = field[y][x].u * 255;
                if (cu < 0) cu = -cu;
                if (cu > 255) cu = 255;
                int cv = field[y][x].v * 255;
                if (cv < 0) cv = -cv;
                if (cv > 255) cv = 255;

                data[index + 1] = cu;
                data[index + 2] = 0;
                data[index + 3] = cv;
            }
        }
    }
}

void incompress() {
    for (int i = 0;i < ITER_MAX;i++) {
        for (int y = 1;y < FIELD_HEIGHT - 1;y++) {
            for (int x = 1;x < FIELD_WIDTH - 1;x++) {
                if (field[y][x].fluid == 0) continue;

                int s = field[y - 1][x].fluid +
                    field[y + 1][x].fluid +
                    field[y][x - 1].fluid +
                    field[y][x + 1].fluid;

                float d = field[y][x + 1].u - field[y][x].u + field[y + 1][x].v - field[y][x].v;
                float p = -d / s * OVERRELAXATION;
                field[y][x].u -= p * field[y][x - 1].fluid;
                field[y][x + 1].u += p * field[y][x + 1].fluid;
                field[y][x].v -= p * field[y - 1][x].fluid;
                field[y + 1][x].v += p * field[y + 1][x].fluid;
            }
        }
    }
}

void fluid_update(float dt) {
    incompress();
}