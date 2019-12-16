#include "definitions.h"
#include <stdio.h>

luz global_lights[8];

void set_default_lights()
{
    luz sol, bombilla, foco;
    
    sol.position[0] = 1.0f;
    sol.position[1] = 1.0f;
    sol.position[2] = 0.0f;
    sol.position[3] = 0.0f;

    sol.ambient[0] = 1.2f;
    sol.ambient[1] = 1.2f;
    sol.ambient[2] = 1.2f;
    sol.ambient[3] = 1.0f;

    sol.diffuse[0] = 1.0f;
    sol.diffuse[1] = 1.0f;
    sol.diffuse[2] = 1.0f;
    sol.diffuse[3] = 1.0f;

    sol.specular[0] = 1.0f;
    sol.specular[1] = 1.0f;
    sol.specular[2] = 1.0f;
    sol.specular[3] = 1.0f;

    sol.is_on = 1;  
    global_lights[0] = sol;

    bombilla.position[0] = 0.0f;
    bombilla.position[1] = 1.0f;
    bombilla.position[2] = 0.0f;
    bombilla.position[3] = 1.0f;
   
    bombilla.ambient[0] = 1.2f;
    bombilla.ambient[1] = 1.2f;
    bombilla.ambient[2] = 1.2f;
    bombilla.ambient[3] = 1.0f;

    bombilla.diffuse[0] = 1.0f;
    bombilla.diffuse[1] = 1.0f;
    bombilla.diffuse[2] = 1.0f;
    bombilla.diffuse[3] = 1.0f;

    bombilla.specular[0] = 1.0f;
    bombilla.specular[1] = 1.0f;
    bombilla.specular[2] = 1.0f;
    bombilla.specular[3] = 1.0f;

    bombilla.is_on = 0;
    global_lights[1] = bombilla;

    foco.position[0] = 0.0f;
    foco.position[1] = 0.0f;
    foco.position[2] = 0.0f;
    foco.position[3] = 1.0f;

    foco.ambient[0] = 0.05f;
    foco.ambient[1] = 0.0f;
    foco.ambient[2] = 0.0f;
    foco.ambient[3] = 1.0f;

    foco.diffuse[0] = 0.50f;
    foco.diffuse[1] = 0.0f;
    foco.diffuse[2] = 0.0f;
    foco.diffuse[3] = 1.0f;

    foco.specular[0] = 0.99f;
    foco.specular[1] = 0.0f;
    foco.specular[2] = 0.0f;
    foco.specular[3] = 1.0f;

    foco.cut_off = 90.0f;

    foco.spot_direction[0] = 0.0f;
    foco.spot_direction[1] = 0.0f;
    foco.spot_direction[2] = 1.0f;

    foco.is_on = 0;
    global_lights[2] = foco;
}

void insert_light()
{
    luz new;
    GLint light_type, pos;
    printf("Inserting new light...\n");
    printf("- Light type: (1 -> Sun, 2 -> Bulb, 3 -> Spotlight)\n");
    scanf("%d", &light_type);
    printf("- Where you want to put the new light? (4-8)\n");
    scanf("%d", &pos);

    if (pos > 8)
    {
        printf("Invalid position!\n");
        return;
    }

    printf("- Insert position [x y z]: \n");
    scanf("%f %f %f", &new.position[0], &new.position[1], &new.position[2]);

    printf("- Insert ambient light [r g b a]: \n");
    scanf("%f %f %f %f", &new.ambient[0], &new.ambient[1], &new.ambient[2], &new.ambient[3]);

    printf("- Insert diffuse light [r g b a]: \n");
    scanf("%f %f %f %f", &new.diffuse[0], &new.diffuse[1], &new.diffuse[2], &new.diffuse[3]);

    printf("- Insert specular light [r g b a]: \n");
    scanf("%f %f %f %f", &new.specular[0], &new.specular[1], &new.specular[2], &new.specular[3]);

    switch(light_type)
    {
        case 1:
            new.position[3] = 0; // positional light
            break;
        case 2:
            new.position[3] = 1; // directional light
            break;
        case 3:
            new.position[3] = 1; // directional light
            break;
        default:
            printf("Incorrect light type!\n");
            break;
    }

    global_lights[pos] = new;
    printf("New light inserted at position: %d\n", pos);
}