#include "include/obj.h"
#include "include/lexer.h"
#include "include/parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


obj_T* init_obj()
{
    obj_T* obj = calloc(1, sizeof(struct OBJ_STRUCT));

    obj->vertice_vectors = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));
    obj->vertice_vectors_size = 0;

    obj->texcoord_vectors = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));
    obj->texcoord_vectors_size = 0;

    obj->normal_vectors = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));
    obj->normal_vectors_size = 0;

    // not implemented right now
    /*obj->parameter_space_vectors = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));
    obj->parameter_space_vectors_size = 0;*/

    obj->polygon_groups = calloc(1, sizeof(struct OBJ_POLYGON_GROUP_STRUCT)); // g <group_name>
    obj->polygon_groups_size = 0;

    obj->line_polygons = calloc(1, sizeof(struct OBJ_LINE_POLYGON_STRUCT));
    obj->line_polygons_size = 0;

    obj->name = 0;

    return obj;
}

void obj_free(obj_T* obj)
{
    for (int i = 0; i < obj->vertice_vectors_size; i++)
        free(obj->vertice_vectors[i]);

    for (int i = 0; i < obj->texcoord_vectors_size; i++)
        free(obj->texcoord_vectors[i]);

    for (int i = 0; i < obj->normal_vectors_size; i++)
        free(obj->normal_vectors[i]);

    for (int i = 0; i < obj->polygon_groups_size; i++)
    {
        obj_polygon_group_T* polygon_group = obj->polygon_groups[i];

        for (int j = 0; j < polygon_group->faces_size; j++)
        {
            obj_face_T* face = polygon_group->faces[j];

            if (face->vertex_pointers_size)
                free(face->vertex_pointers);

            if (face->normal_pointers_size)
                free(face->normal_pointers);

            if (face->texcoord_pointers_size)
                free(face->texcoord_pointers);

            free(face);
        }

        free(polygon_group->faces);
        free(polygon_group);
    }

    free(obj->polygon_groups);

    for (int i = 0; i < obj->line_polygons_size; i++)
    {
        free(obj->line_polygons[i]->vertex_pointers);
        free(obj->line_polygons[i]);
    }

    free(obj->line_polygons);

    free(obj->vertice_vectors);

    free(obj->texcoord_vectors);

    free(obj->normal_vectors);

    if (obj->name)
        free(obj->name);

    free(obj);
}

void obj_list_free(obj_list_T* object_list)
{
    for (int i = 0; i < object_list->size; i++)
        obj_free(object_list->objects[i]);

    free(object_list->objects);
    free(object_list);
}

obj_list_T* obj_load_from_file(const char* filename)
{
    char* buff = calloc(1, sizeof(char));
    buff[0] = '\0';

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("Opening %s for reading.\n", filename);

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(1);

    printf("Reading %s line by line...\n", filename);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        buff = realloc(buff, (sizeof(char) * (strlen(buff) + strlen(line) + 1)));
        strcat(buff, line);
    }

    free(line);

    fclose(fp);

    lexer_T* lexer = init_lexer(buff);
    parser_T* parser = init_parser(lexer);

    printf("Parsing %s...\n", filename);
    parser_parse(parser);

    printf("Done loading %s.\n", filename);

    obj_T** objects = parser->objects;
    obj_list_T* list = calloc(1, sizeof(struct OBJ_LIST_STRUCT));
    list->objects = objects;
    list->size = parser->objects_size;

    free(buff);
    lexer_free(lexer);

    if (strlen(parser->current_token->value))
        free(parser->current_token->value);

    free(parser->current_token);
    free(parser);

    return list;
}
