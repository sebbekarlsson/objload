#ifndef OBJ_LOAD_H
#define OBJ_LOAD_H
#include <stdio.h>


typedef struct OBJ_VECTOR_STRUCT
{
    float x;
    float y;
    float z;
    float w;
} obj_vec_T;

typedef struct OBJ_FACE_STRUCT
{
    unsigned int* vertex_pointers;    // v1, v2, v3
    size_t vertex_pointers_size;

    unsigned int* texcoord_pointers;  // vt1, vt2, vt3
    size_t texcoord_pointers_size;

    unsigned int* normal_pointers;    // vn1, vn2, vn3
    size_t normal_pointers_size;
} obj_face_T;

typedef struct OBJ_POLYGON_GROUP_STRUCT
{
    char* name; // g <group name>
    char* material_name; // usemtl <material name>
    unsigned int smooth;

    obj_face_T** faces; // f v1, v2, v3 ...
    size_t faces_size;
} obj_polygon_group_T;

typedef struct OBJ_LINE_POLYGON_STRUCT
{
    unsigned int* vertex_pointers;
    size_t vertex_pointers_size;
} obj_line_polygon_T;

typedef struct OBJ_STRUCT
{
    char* name;

    obj_vec_T** vertice_vectors;
    size_t vertice_vectors_size;

    obj_vec_T** texcoord_vectors;
    size_t texcoord_vectors_size;

    obj_vec_T** normal_vectors;
    size_t normal_vectors_size;

    // not implemented right now
    /*obj_vec_T** parameter_space_vectors;
    size_t parameter_space_vectors_size;*/

    obj_polygon_group_T** polygon_groups; // g <group_name>
    size_t polygon_groups_size;

    obj_line_polygon_T** line_polygons;
    size_t line_polygons_size;
} obj_T;

typedef struct OBJ_LIST_STRUCT
{
    obj_T** objects;
    size_t size;
} obj_list_T;

obj_T* init_obj();

void obj_free(obj_T* obj);

void obj_list_free(obj_list_T* object_list);

obj_list_T* obj_load_from_file(const char* filename);
#endif
