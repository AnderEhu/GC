#include "definitions.h"
#include <stdlib.h>
#include <stdio.h>

material *m_bronze, *m_emerald;

void set_global_materials()
{
    m_bronze = (material*)malloc(sizeof(material));
    m_emerald = (material*)malloc(sizeof(material));

    // BRONZE
    m_bronze->Ka[0] = 0.25f;
    m_bronze->Ka[1] = 0.148f;
    m_bronze->Ka[2] = 0.06475f;
    m_bronze->Ka[3] = 1.0f;

    m_bronze->Kd[0] = 0.4f;
    m_bronze->Kd[1] = 0.2368f;
    m_bronze->Kd[2] = 0.1036f;
    m_bronze->Kd[3] = 1.0f;

    m_bronze->Ks[0] = 0.774597f;
    m_bronze->Ks[1] = 0.458561f;
    m_bronze->Ks[2] = 0.200621f;
    m_bronze->Ks[3] = 1.0f;

    m_bronze->ns[0] = 76.8f;

    // EMERALD
    m_emerald->Ka[0] = 0.0215f;
    m_emerald->Ka[1] = 0.1745f;
    m_emerald->Ka[2] = 0.0215f;
    m_emerald->Ka[3] = 0.55f;

    m_emerald->Kd[0] = 0.07568f;
    m_emerald->Kd[1] = 0.61424f;
    m_emerald->Kd[2] = 0.07568f;
    m_emerald->Kd[3] = 0.55f;

    m_emerald->Ks[0] = 0.633f;
    m_emerald->Ks[1] = 0.727811f;
    m_emerald->Ks[2] = 0.633f;
    m_emerald->Ks[3] = 0.55f;

    m_emerald->ns[0] = 76.8f;
}

void set_material(object3d *_selected_object)
{
    _selected_object->material = m_bronze;
}

void change_material(object3d *_selected_object)
{
    if (_selected_object->material == m_bronze)
    {
        printf("Cambio material: BRONZE -> EMERALD\n");
        _selected_object->material = m_emerald;
    }
    else
    {
        printf("Cambio material: EMERALD -> BRONZE\n");
        _selected_object->material = m_bronze;
    }
}