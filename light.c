#include "definitions.h"
#include <stdio.h>
#define C_ATT 1.0f
#define L_ATT 0.2f
#define Q_ATT 0.1f

luz global_lights[8];
extern object3d *_selected_object;

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
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, C_ATT); 
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, L_ATT); 
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Q_ATT);
        break;
    case 2:
        glLightfv(GL_LIGHT2, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT2, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT2, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT2, GL_POSITION, global_lights[i].position); 
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, C_ATT);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, L_ATT);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Q_ATT);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, global_lights[i].cut_off); //angle of the cone light: value between 0 to 180
        break;
    case 3:
        glLightfv(GL_LIGHT3, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT3, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT3, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT3, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, C_ATT);
            glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, L_ATT);
            glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, Q_ATT);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, global_lights[i].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 4:
        glLightfv(GL_LIGHT4, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT4, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT4, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT4, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, C_ATT);
            glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, L_ATT);
            glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, Q_ATT);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, global_lights[i].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 5:
        glLightfv(GL_LIGHT5, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT5, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT5, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT5, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, C_ATT);
            glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, L_ATT);
            glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, Q_ATT);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, global_lights[i].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 6:
        glLightfv(GL_LIGHT6, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT6, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT6, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT6, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, C_ATT);
            glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, L_ATT);
            glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, Q_ATT);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, global_lights[i].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    case 7:
        glLightfv(GL_LIGHT7, GL_AMBIENT, global_lights[i].ambient); 
        glLightfv(GL_LIGHT7, GL_DIFFUSE, global_lights[i].diffuse); 
        glLightfv(GL_LIGHT7, GL_SPECULAR, global_lights[i].specular);
        glLightfv(GL_LIGHT7, GL_POSITION, global_lights[i].position); 
        
        if (global_lights[i].type == LUZ_TIPO_BOMBILLA || global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, C_ATT);
            glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, L_ATT);
            glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, Q_ATT);
        }
        
        if (global_lights[i].type == LUZ_TIPO_FOCO)
        {
            glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, global_lights[i].spot_direction);
            glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, global_lights[i].cut_off); //angle of the cone light: value between 0 to 180
        }
        break;
    default:
        break;
    }
}

void set_default_lights()
{
    global_lights[0].position[0] = 0.0f;
    global_lights[0].position[1] = 1.0f;
    global_lights[0].position[2] = 0.0f;
    global_lights[0].position[3] = 0.0f;

    global_lights[0].ambient[0] = 1.2f;
    global_lights[0].ambient[1] = 1.2f;
    global_lights[0].ambient[2] = 1.2f;
    global_lights[0].ambient[3] = 1.0f;

    global_lights[0].diffuse[0] = 1.0f;
    global_lights[0].diffuse[1] = 1.0f;
    global_lights[0].diffuse[2] = 1.0f;
    global_lights[0].diffuse[3] = 1.0f;

    global_lights[0].specular[0] = 1.0f;
    global_lights[0].specular[1] = 1.0f;
    global_lights[0].specular[2] = 1.0f;
    global_lights[0].specular[3] = 1.0f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    put_light(0);
    glGetFloatv(GL_MODELVIEW_MATRIX, global_lights[0].m_obj);
    global_lights[0].type = LUZ_TIPO_SOL;
    global_lights[0].is_on = 1;
    
    global_lights[1].position[0] = 1.0f;
    global_lights[1].position[1] = 1.0f;
    global_lights[1].position[2] = 0.0f;
    global_lights[1].position[3] = 1.0f;
   
    global_lights[1].ambient[0] = 1.2f;
    global_lights[1].ambient[1] = 1.2f;
    global_lights[1].ambient[2] = 1.2f;
    global_lights[1].ambient[3] = 1.0f;

    global_lights[1].diffuse[0] = 1.0f;
    global_lights[1].diffuse[1] = 1.0f;
    global_lights[1].diffuse[2] = 1.0f;
    global_lights[1].diffuse[3] = 1.0f;

    global_lights[1].specular[0] = 1.0f;
    global_lights[1].specular[1] = 1.0f;
    global_lights[1].specular[2] = 1.0f;
    global_lights[1].specular[3] = 1.0f;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    put_light(1);
    glGetFloatv(GL_MODELVIEW_MATRIX, global_lights[1].m_obj);

    global_lights[1].type = LUZ_TIPO_BOMBILLA;
    global_lights[1].is_on = 0;
}

void set_m_spotlight()
{
    int i;
    for (i = 0; i < 16; i++) {
        global_lights[2].m_obj[i] = _selected_object->list_matrix->m[i];
    }
}

void init_obj_spotlight()
{
    global_lights[2].position[0] = _selected_object->max.x / 2;
    global_lights[2].position[1] = _selected_object->max.y / 2;
    global_lights[2].position[2] = _selected_object->max.z;
    global_lights[2].position[3] = 1;

    global_lights[2].ambient[0] = 1.5f;
    global_lights[2].ambient[1] = 1.5f;
    global_lights[2].ambient[2] = 1.5f;
    global_lights[2].ambient[3] = 1.0f;

    global_lights[2].diffuse[0] = 1.5f;
    global_lights[2].diffuse[1] = 1.5f;
    global_lights[2].diffuse[2] = 1.5f;
    global_lights[2].diffuse[3] = 1.0f;

    global_lights[2].specular[0] = 1.0f;
    global_lights[2].specular[1] = 1.0f;
    global_lights[2].specular[2] = 1.0f;
    global_lights[2].specular[3] = 1.0f;

    global_lights[2].cut_off = 45.0f;

    global_lights[2].spot_direction[0] = 0.0f;
    global_lights[2].spot_direction[1] = 0.0f;
    global_lights[2].spot_direction[2] = 1.0f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    put_light(2);
    set_m_spotlight();
    global_lights[2].type = LUZ_TIPO_FOCO_OBJ;
    global_lights[2].is_on = 0;
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

    if (pos < 4 || pos > 8)
    {
        printf("Invalid position!\n");
        return;
    }

    if (light_type != 1)
    {
        printf("- Insert position [x y z]: \n");
        scanf("%f %f %f", &new.position[0], &new.position[1], &new.position[2]);
    }
    
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
            new.position[0] = 0.0f;
            new.position[1] = 1.0f;
            new.position[2] = 0.0f;
            new.position[3] = 0.0f; // positional light
            break;
        case 2:
            new.type = LUZ_TIPO_BOMBILLA;
            new.position[3] = 1.0f; // directional light
            break;
        case 3:
            new.type = LUZ_TIPO_FOCO;
            new.position[3] = 1.0f; // directional light
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
                new.cut_off = 45.0f;
                new.spot_direction[0] = 0.0f;
                new.spot_direction[1] = 0.0f;
                new.spot_direction[2] = 1.0f;
                printf("-> Default values: \n");
                printf("---> Cut-off: 45.0\n");
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