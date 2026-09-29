#include "fluid.h"
#include <stdint.h>

void fluid_draw(char* data) {
    for (int y = 0;y < FIELD_HEIGHT;y++) {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            size_t index = (x + y * FIELD_WIDTH) * 4;
            char color = 128;

            data[index] = 255;
            data[index + 1] = color;
            data[index + 2] = color;
            data[index + 3] = color;
        }
    }
}

void fluid_update(float dt) {

}