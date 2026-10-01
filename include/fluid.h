#ifndef FLUID_H
#define FLUID_H

#define FIELD_WIDTH (160)
#define FIELD_HEIGHT (120)

void fluid_init();
void fluid_draw(char* data);
void fluid_update(float dt);

#endif