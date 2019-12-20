#include "definitions.h"
#include <stdio.h>

luz global_lights[8];

void put_light(GLint i) 
{
    switch (i)
    {
    case 0:
        glLightfv(GL_LIGHT0, GL_POSITION, global_lights[i].position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT0, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT0, GL_SPECULAR, global_lights[i].specular); 
        break;
    case 1:
        glLightfv(GL_LIGHT1, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT1, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT1, GL_SPECULAR, global_lights[i].specular); 
        glLightfv(GL_LIGHT1, GL_POSITION, global_lights[i].position); 
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5f); 
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5f); 
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2f);
        break;
    case 2:
        glLightfv(GL_LIGHT2, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT2, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT2, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT2, GL_POSITION, global_lights[i].position); 
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5f);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5f);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.2f);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, global_lights[2].cut_off); //angle of the cone light: value between 0 to 180
        break;
    case 3:
        glLightfv(GL_LIGHT3, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT3, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT3, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT3, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.5f);
            glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5f);
            glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2f);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, global_lights[2].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 4:
        glLightfv(GL_LIGHT4, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT4, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT4, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT4, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.5f);
            glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.5f);
            glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.2f);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, global_lights[2].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 5:
        glLightfv(GL_LIGHT5, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT5, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT5, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT5, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 1.5f);
            glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.5f);
            glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.2f);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, global_lights[2].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 6:
        glLightfv(GL_LIGHT6, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT6, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT6, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT6, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 1.5f);
            glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.5f);
            glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.2f);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, global_lights[2].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 7:
        glLightfv(GL_LIGHT7, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT7, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT7, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT7, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 1.5f);
            glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.5f);
            glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0.2f);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, global_lights[2].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    default:
        break;
    }
}

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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    put_light(0);
    glGetFloatv(GL_MODELVIEW_MATRIX, sol.m_obj);
    sol.type = LUZ_TIPO_SOL;
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
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    put_light(1);
    glGetFloatv(GL_MODELVIEW_MATRIX, bombilla.m_obj);

    bombilla.type = LUZ_TIPO_BOMBILLA;
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    put_light(3);
    glGetFloatv(GL_MODELVIEW_MATRIX, foco.m_obj);
    foco.type = LUZ_TIPO_FOCO;
    foco.is_on = 0;
    global_lights[2] = foco;
}

void insert_light()
{
    luz new;
    GLint light_type, pos, values;
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

    printf("- Insert properties or set default values? (1 -> set, 2 -> default): \n");
    scanf("%d", &values);

    if (values == 1) // set values
    {
        printf("- Insert ambient light [r g b a]: \n");
        scanf("%f %f %f %f", &new.ambient[0], &new.ambient[1], &new.ambient[2], &new.ambient[3]);

        printf("- Insert diffuse light [r g b a]: \n");
        scanf("%f %f %f %f", &new.diffuse[0], &new.diffuse[1], &new.diffuse[2], &new.diffuse[3]);

        printf("- Insert specular light [r g b a]: \n");
        scanf("%f %f %f %f", &new.specular[0], &new.specular[1], &new.specular[2], &new.specular[3]);
    }
    else // default values
    {
        new.ambient[0] = 1.2f;
        new.ambient[1] = 1.2f;
        new.ambient[2] = 1.2f;
        new.ambient[3] = 1.0f;

        new.diffuse[0] = 1.0f;
        new.diffuse[1] = 1.0f;
        new.diffuse[2] = 1.0f;
        new.diffuse[3] = 1.0f;

        new.specular[0] = 1.0f;
        new.specular[1] = 1.0f;
        new.specular[2] = 1.0f;
        new.specular[3] = 1.0f;

        printf("-> Default values: \n");
        printf("---> Ambient: [1.2 1.2 1.2 1.0]\n");
        printf("---> Diffuse: [1.0 1.0 1.0 1.0]\n");
        printf("---> Specular: [1.0 1.0 1.0 1.0]\n");
    }
    
    switch(light_type)
    {
        case 1:
            new.type = LUZ_TIPO_SOL;
            new.position[3] = 0; // positional light
            break;
        case 2:
            new.type = LUZ_TIPO_BOMBILLA;
            new.position[3] = 1; // directional light
            break;
        case 3:
            new.type = LUZ_TIPO_FOCO;
            new.position[3] = 1; // directional light
            printf("- Spot direction and cut-off angle needed...\n");
            printf("- Set values or use default values (1 -> set, 2 -> default): \n");
            scanf("%d", &values);

            if (values == 1)
            {
                printf("- Insert spot direction [x y z]: \n");
                scanf("%f %f %f", &new.spot_direction[0], &new.spot_direction[1], &new.spot_direction[2]);
                printf("- Insert cut-off angle: \n");
                scanf("%f", &new.cut_off);
            }
            else
            {
                new.cut_off = 90.0f;
                new.spot_direction[0] = 0.0f;
                new.spot_direction[1] = 0.0f;
                new.spot_direction[2] = 1.0f;
                printf("-> Default values: \n");
                printf("---> Cut-off: 90.0\n");
                printf("---> Spot direction: [0.0 0.0 1.0]\n");
            }
            break;
        default:
            printf("Incorrect light type!\n");
            break;
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    put_light(pos-1);
    glGetFloatv(GL_MODELVIEW_MATRIX, new.m_obj);
    new.is_on = 0;
    global_lights[pos-1] = new;

    printf("New light inserted at position: %d, F%d to activate it.\n", pos, pos);
}