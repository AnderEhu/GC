#include "definitions.h"

luz global_lights[3];
void set_default_lights()
{
    luz sol, bombilla, foco;
    sol.is_on = 1;
    sol.position[0] = 1.0f;
    sol.position[1] = 1.0f;
    sol.position[2] = 0.0f;
    sol.position[3] = 0.0f;

    global_lights[0] = sol;

    bombilla.position[0] = 5.0f;
    bombilla.position[1] = 5.0f;
    bombilla.position[2] = 5.0f;
    bombilla.position[3] = 1.0f;

    global_lights[1] = bombilla;

    foco.position[0] = 0.0f;
    foco.position[1] = 0.0f;
    foco.position[2] = 0.0f;
    foco.position[3] = 1.0f;

    foco.spot_direction[0] = 0.0f;
    foco.spot_direction[1] = 0.0f;
    foco.spot_direction[2] = 1.0f;

    global_lights[2] = foco;



    

}