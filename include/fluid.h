#ifndef FLUID_H
#define FLUID_H

#define FIELD_WIDTH (320)
#define FIELD_HEIGHT (240)

void fluid_init();
void fluid_draw(char* data);
void fluid_update(float dt);

#endif