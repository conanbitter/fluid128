#include "fluid.h"
#include <stdint.h>

typedef struct Cell {
    float u;
    float v;
    int fluid;
    float smoke;
    float new_smoke;
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
            field[y][x].smoke = x == 0 && y >= TUBE_MIN && y <= TUBE_MAX ? 1.0f : 0.0f;
            field[y][x].u = x == 1 && fluid == 1 ? 2.0f : 0.0f;
        }
    }

    for (int y = TUBE_MIN;y < TUBE_MAX;y++) {
        for (int x = TUBE_MIN;x < TUBE_MIN * 2;x++) {
            field[y][x].fluid = 0;
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
                int color = field[y][x].smoke * 255;
                if (color < 0) color = 0;
                if (color > 255) color = 255;
                int cu = field[y][x].u * 255;
                if (cu < 0) cu = -cu;
                if (cu > 255) cu = 255;
                int cv = field[y][x].v * 255;
                if (cv < 0) cv = -cv;
                if (cv > 255) cv = 255;

                data[index + 1] = cu;
                data[index + 2] = color;
                data[index + 3] = cv;
                //data[index + 1] = color;
                //data[index + 2] = color;
                //data[index + 3] = color;
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

float avg_u(int x, int y) {
    return (field[y][x].u +
        field[y][x + 1].u +
        field[y - 1][x].u +
        field[y - 1][x + 1].u) / 4.0f;
}

float avg_v(int x, int y) {
    return (field[y][x].u +
        field[y][x - 1].u +
        field[y + 1][x].u +
        field[y + 1][x - 1].u) / 4.0f;
}

float sample_u(float x, float y) {
    if (x < 1.0f) x = 1.0f;
    if (x > FIELD_WIDTH - 1.0f) x = FIELD_WIDTH - 1.0f;
    if (y < 1.0f) y = 1.0f;
    if (y > FIELD_HEIGHT - 1.0f) y = FIELD_HEIGHT - 1.0f;

    int xi = x;
    int yi = y - 0.5f;
    float dx = x - xi;
    float dy = y - 0.5f - yi;

    return field[yi][xi].u * (1.0f - dx) * (1.0f - dy) +
        field[yi][xi + 1].u * dx * (1.0f - dy) +
        field[yi + 1][xi].u * (1.0f - dx) * dy +
        field[yi + 1][xi + 1].u * dx * dy;
}

float sample_v(float x, float y) {
    if (x < 1.0f) x = 1.0f;
    if (x > FIELD_WIDTH - 1.0f) x = FIELD_WIDTH - 1.0f;
    if (y < 1.0f) y = 1.0f;
    if (y > FIELD_HEIGHT - 1.0f) y = FIELD_HEIGHT - 1.0f;

    int xi = x - 0.5f;
    int yi = y;
    float dx = x - 0.5f - xi;
    float dy = y - yi;

    return field[yi][xi].v * (1.0f - dx) * (1.0f - dy) +
        field[yi][xi + 1].v * dx * (1.0f - dy) +
        field[yi + 1][xi].v * (1.0f - dx) * dy +
        field[yi + 1][xi + 1].v * dx * dy;
}

float sample_smoke(float x, float y) {
    if (x < 1.0f) x = 1.0f;
    if (x > FIELD_WIDTH - 1.0f) x = FIELD_WIDTH - 1.0f;
    if (y < 1.0f) y = 1.0f;
    if (y > FIELD_HEIGHT - 1.0f) y = FIELD_HEIGHT - 1.0f;

    int xi = x - 0.5f;
    int yi = y - 0.5f;
    float dx = x - 0.5 - xi;
    float dy = y - 0.5 - yi;

    return field[yi][xi].smoke * (1.0f - dx) * (1.0f - dy) +
        field[yi][xi + 1].smoke * dx * (1.0f - dy) +
        field[yi + 1][xi].smoke * (1.0f - dx) * dy +
        field[yi + 1][xi + 1].smoke * dx * dy;
}

void advect_velocity(float dt) {
    for (int y = 0;y < FIELD_HEIGHT;y++) {
        for (int x = 0;x < FIELD_WIDTH;x++) {
            if (x == 0 || y == 0 || x == FIELD_WIDTH - 1 || y == FIELD_HEIGHT - 1) {
                field[y][x].new_u = field[y][x].u;
                field[y][x].new_v = field[y][x].v;
                continue;
            }

            // update u
            if (field[y][x].fluid == 1 && field[y][x - 1].fluid == 1) {
                float u = field[y][x].u;
                float v = avg_v(x, y);
                float old_x = x - dt * u;
                float old_y = y + 0.5f - dt * v;
                field[y][x].new_u = sample_u(old_x, old_y);
            } else {
                field[y][x].new_u = field[y][x].u;
            }

            //update v
            if (field[y][x].fluid == 1 && field[y - 1][x].fluid == 1) {
                float u = avg_u(x, y);
                float v = field[y][x].v;
                float old_x = x + 0.5f - dt * u;
                float old_y = y - dt * v;
                field[y][x].new_v = sample_v(old_x, old_y);
            } else {
                field[y][x].new_v = field[y][x].v;
            }
        }
    }

    for (int y = 0;y < FIELD_HEIGHT;y++) {
        for (int x = 0;x < FIELD_WIDTH;x++) {
            field[y][x].u = field[y][x].new_u;
            //field[y][x].v = field[y][x].new_v;
        }
    }
}

void advect_smoke(float dt) {
    for (int y = 1;y < FIELD_HEIGHT - 1;y++) {
        for (int x = 1;x < FIELD_WIDTH - 1;x++) {
            float u = (field[y][x].u + field[y][x + 1].u) / 2.0f;
            float v = (field[y][x].v + field[y + 1][x].v) / 2.0f;

            float old_x = x + 0.5f - dt * u;
            float old_y = y + 0.5f - dt * v;

            field[y][x].new_smoke = sample_smoke(old_x, old_y);
        }
    }
    for (int y = 1;y < FIELD_HEIGHT - 1;y++) {
        for (int x = 1;x < FIELD_WIDTH - 1;x++) {
            field[y][x].smoke = field[y][x].new_smoke;
        }
    }
}

void fluid_update(float dt) {
    incompress();
    advect_velocity(dt);
    advect_smoke(dt * 100.0f);
}