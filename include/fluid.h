#ifndef FLUID_H
#define FLUID_H

#define FIELD_WIDTH (80)
#define FIELD_HEIGHT (60)

void fluid_init();
void fluid_draw(char* data);
void fluid_update(float dt);

#endif