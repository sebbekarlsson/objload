#ifndef OBJ_LOAD_H
#define OBJ_LOAD_H
#include <stdio.h>


typedef struct OBJ_VECTOR_STRUCT
{
    enum
    {
        VEC_VERTICE,
        VEC_TEXCOORD,
        VEC_NORMAL
    } type;

    float x;
    float y;
    float z;
} obj_vec_T;

typedef struct OBJ_FACE_STRUCT
{
    unsigned int* vertex_pointers;    // v1, v2, v3
    size_t vertex_pointers_size;

    unsigned int* texcoord_pointers;  // vt1, vt2, vt3
    size_t texcoord_pointers_size;

    unsigned int* normal_pointers;    // vt1, vt2, vt3
    size_t normal_pointers_size;
} obj_face_T;

typedef struct OBJ_STRUCT
{
    obj_vec_T** vectors;
    size_t vectors_size;

    obj_vec_T** texture_vectors;
    size_t texture_vectors_size;

    obj_face_T** faces;
    size_t faces_size;
} obj_T;

obj_T* init_obj();

void obj_free(obj_T* obj);

obj_T* obj_load_from_file(const char* filename);
#endif
