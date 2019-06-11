#include "include/parser.h"
#include <stdlib.h>
#include <string.h>


parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->objects = calloc(1, sizeof(struct OBJ_STRUCT));
    parser->objects_size = 1;
    parser->objects[0] = init_obj();
    parser->obj_pointer = 0;

    return parser;
}

void parser_eat(parser_T* parser, int token_type)
{
    if (parser->current_token->type != token_type)
    {
        printf(
            "(Line %d): Unexpected token `%s`, was expecting `%d`.\n",
            parser->lexer->line_n + 1,
            parser->current_token->value,
            token_type
        );
        exit(1);
    }
    else if (parser->current_token->type == token_type)
    {
        free(parser->current_token->value);
        free(parser->current_token);
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
}

void parser_parse(parser_T* parser)
{
    obj_T* obj; 
    obj_polygon_group_T* polygon_group = (void*) 0;

    while (parser->current_token->type != TOKEN_EOF)
    {
        obj = parser->objects[parser->obj_pointer];

        // a new polygon_group is about to be created
        if (strcmp(parser->current_token->value, "g") == 0)
        {
            polygon_group = parser_parse_polygon_group(parser); 
            parser_eat(parser, TOKEN_NEWLINE);
        }

        // making sure we always have a polygon_group to work with.
        if (!polygon_group)
        {
            polygon_group = calloc(1, sizeof(struct OBJ_POLYGON_GROUP_STRUCT));
            polygon_group->faces = calloc(1, sizeof(struct OBJ_FACE_STRUCT));
            polygon_group->faces_size = 0;
            polygon_group->name = "global";
        } 

        parser_parse_statement(parser, polygon_group);

        while (parser->current_token->type == TOKEN_NEWLINE)
        {
            parser_eat(parser, TOKEN_NEWLINE);
            parser_parse_statement(parser, polygon_group);
        } 

        // adding the current polygon_group to the object.
        obj->polygon_groups_size += 1;
        obj->polygon_groups = realloc(obj->polygon_groups, obj->polygon_groups_size * sizeof(struct OBJ_POLYGON_GROUP_STRUCT));
        obj->polygon_groups[obj->polygon_groups_size - 1] = polygon_group;
    }
}

void parser_parse_statement(parser_T* parser, obj_polygon_group_T* polygon_group)
{
    obj_T* obj = parser->objects[parser->obj_pointer];

    if (parser->current_token->type == TOKEN_ID)
    {
       if (strcmp(parser->current_token->value, "o") == 0)
       {
           obj->name = parser_parse_object_name(parser);
       }
       else
       if (strcmp(parser->current_token->value, "v") == 0)
       {
           parser_parse_vertice_vector(parser); 
       }
       else
       if (strcmp(parser->current_token->value, "vt") == 0)
       {
           parser_parse_texcoord_vector(parser);
       }
       else
       if (strcmp(parser->current_token->value, "vn") == 0)
       {
           parser_parse_normal_vector(parser);
       }
       else
       if (strcmp(parser->current_token->value, "f") == 0)
       {
           parser_parse_face(parser, polygon_group);
       }
       else
       if (strcmp(parser->current_token->value, "mtllib") == 0)
       {
           parser_parse_material_reference(parser);
       }
       else
       if (strcmp(parser->current_token->value, "usemtl") == 0)
       {
           parser_parse_material_use(parser, polygon_group);
       }
       else
       if (strcmp(parser->current_token->value, "s") == 0)
       {
           parser_parse_smooth_bool(parser, polygon_group);
       }
       else
       if (strcmp(parser->current_token->value, "l") == 0)
       {
           parser_parse_polyline(parser);
       }
       else
       {
           printf("Unhandled ID value=%s\n", parser->current_token->value);
       }
    }
    else if (parser->current_token->type != TOKEN_NEWLINE)
    {
        printf("(Line: %d) Unhandled token type=%d value=%s\n", parser->lexer->line_n, parser->current_token->type, parser->current_token->value);
    }
}

/**
 * Parses:
 *     v 0.123 0.234 0.345 1.0
 */
void parser_parse_vertice_vector(parser_T* parser)
{
    obj_T* obj = parser->objects[parser->obj_pointer];

    parser_eat(parser, TOKEN_ID);
    obj_vec_T* vec = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->x = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->y = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->z = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->w = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    obj->vertice_vectors_size += 1;
    obj->vertice_vectors = realloc(obj->vertice_vectors, obj->vertice_vectors_size * sizeof(struct OBJ_VECTOR_STRUCT));
    obj->vertice_vectors[obj->vertice_vectors_size - 1] = vec;
}

/**
 * Parses:
 *     vt 0.123 0.234 0.345 1.0
 */
void parser_parse_texcoord_vector(parser_T* parser)
{
    obj_T* obj = parser->objects[parser->obj_pointer];

    parser_eat(parser, TOKEN_ID);
    obj_vec_T* vec = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->x = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->y = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->z = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->w = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    obj->texcoord_vectors_size += 1;
    obj->texcoord_vectors = realloc(obj->texcoord_vectors, obj->texcoord_vectors_size * sizeof(struct OBJ_VECTOR_STRUCT));
    obj->texcoord_vectors[obj->texcoord_vectors_size - 1] = vec;
}

/**
 * Parses:
 *     vn 0.123 0.234 0.345 1.0
 */
void parser_parse_normal_vector(parser_T* parser)
{
    obj_T* obj = parser->objects[parser->obj_pointer];

    parser_eat(parser, TOKEN_ID);
    obj_vec_T* vec = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->x = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->y = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->z = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE || parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        vec->w = atof(parser->current_token->value);
        parser_eat(parser, parser->current_token->type);
    }

    obj->normal_vectors_size += 1;
    obj->normal_vectors = realloc(obj->normal_vectors, obj->normal_vectors_size * sizeof(struct OBJ_VECTOR_STRUCT));
    obj->normal_vectors[obj->normal_vectors_size - 1] = vec;
}

/**
 * Parses:
 *     f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
 */
void parser_parse_face(parser_T* parser, obj_polygon_group_T* polygon_group)
{
    parser_eat(parser, TOKEN_ID);
    obj_face_T* face = calloc(1, sizeof(struct OBJ_FACE_STRUCT));

    face->vertex_pointers_size = 0;
    face->texcoord_pointers_size = 0;
    face->normal_pointers_size = 0;

    face->vertex_pointers = calloc(1, sizeof(unsigned int));
    face->texcoord_pointers = calloc(1, sizeof(unsigned int));
    face->normal_pointers = calloc(1, sizeof(unsigned int));

    while (parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        face->vertex_pointers_size += 1;
        face->vertex_pointers = realloc(face->vertex_pointers, face->vertex_pointers_size * sizeof(unsigned int));
        face->vertex_pointers[face->vertex_pointers_size - 1] = atoi(parser->current_token->value);
        parser_eat(parser, TOKEN_INTEGER_VALUE);

        unsigned int div_count = 0;

        while (parser->current_token->type == TOKEN_DIV)
        {
            parser_eat(parser, TOKEN_DIV);

            if (div_count == 0 && parser->current_token->type == TOKEN_INTEGER_VALUE)
            {
                face->texcoord_pointers_size += 1;
                face->texcoord_pointers = realloc(face->texcoord_pointers, face->texcoord_pointers_size * sizeof(unsigned int));
                face->texcoord_pointers[face->texcoord_pointers_size - 1] = atoi(parser->current_token->value);
                parser_eat(parser, TOKEN_INTEGER_VALUE);
            }
            else if (div_count == 1 && parser->current_token->type == TOKEN_INTEGER_VALUE)
            {
                face->normal_pointers_size += 1;
                face->normal_pointers = realloc(face->normal_pointers, face->normal_pointers_size * sizeof(unsigned int));
                face->normal_pointers[face->normal_pointers_size - 1] = atoi(parser->current_token->value);
                parser_eat(parser, TOKEN_INTEGER_VALUE);
            }
            else if (div_count >= 2)
            {
                printf("(Line: %d) Found too many `/` while parsing faces.\n", parser->lexer->line_n);
                exit(1);
            }

            div_count += 1;
        }
    }

    if (!face->vertex_pointers_size)
        free(face->vertex_pointers);

    if (!face->texcoord_pointers_size)
        free(face->texcoord_pointers);

    if (!face->normal_pointers_size)
        free(face->normal_pointers);

    polygon_group->faces_size += 1;
    polygon_group->faces = realloc(polygon_group->faces, polygon_group->faces_size * sizeof(struct OBJ_POLYGON_GROUP_STRUCT));
    polygon_group->faces[polygon_group->faces_size - 1] = face;
}

/**
 * Parses:
 *     g [group name]
 */
obj_polygon_group_T* parser_parse_polygon_group(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID);
    obj_polygon_group_T* polygon_group = calloc(1, sizeof(struct OBJ_POLYGON_GROUP_STRUCT));
    polygon_group->faces = calloc(1, sizeof(struct OBJ_FACE_STRUCT));
    polygon_group->faces_size = 0;
    char* group_name = calloc(1, sizeof(char));
    group_name[0] = '\0';

    while (parser->current_token->type == TOKEN_ID)
    {
        group_name = realloc(group_name, sizeof(char) * (strlen(group_name) + strlen(parser->current_token->value) + 1));
        strcat(group_name, parser->current_token->value);
        parser_eat(parser, TOKEN_ID);

    }

    polygon_group->name = group_name;
    polygon_group->smooth = 0;

    return polygon_group;
}

/**
 * Parses:
 *     mtllib [external .mtl file name]
 */
void parser_parse_material_reference(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID);

    char* file_name = calloc(1, sizeof(char));
    file_name[0] = '\0';

    while (parser->current_token->type != TOKEN_NEWLINE)
    {
        file_name = realloc(file_name, sizeof(char) * (strlen(file_name) + strlen(parser->current_token->value) + 1));
        strcat(file_name, parser->current_token->value);
        parser_eat(parser, TOKEN_ID);
    }
}

/**
 * Parses:
 *     usemtl [material name]
 */
void parser_parse_material_use(parser_T* parser, obj_polygon_group_T* polygon_group)
{
    parser_eat(parser, TOKEN_ID);

    char* material_name = calloc(1, sizeof(char));
    material_name[0] = '\0';

    while (parser->current_token->type != TOKEN_NEWLINE)
    {
        material_name = realloc(material_name, sizeof(char) * (strlen(material_name) + strlen(parser->current_token->value) + 1));
        strcat(material_name, parser->current_token->value);
        parser_eat(parser, TOKEN_ID);
    }

    polygon_group->material_name = material_name;
}

/**
 * Parses:
 *     o [object name]
 */
char* parser_parse_object_name(parser_T* parser)
{
    char* object_name = calloc(1, sizeof(char));

    object_name[0] = '\0';

    while (parser->current_token->type == TOKEN_ID)
    {
        object_name = realloc(object_name, sizeof(char) * (strlen(object_name) + strlen(parser->current_token->value) + 1));
        strcat(object_name, parser->current_token->value);
        parser_eat(parser, TOKEN_ID);
    }
    
    return object_name;
}

void parser_parse_smooth_bool(parser_T* parser, obj_polygon_group_T* polygon_group)
{
    parser_eat(parser, TOKEN_ID); // s

    if (strcmp(parser->current_token->value, "off") == 0 || strcmp(parser->current_token->value, "0") == 0)
    {
        polygon_group->smooth = 0;
        parser_eat(parser, parser->current_token->type);
    }
    else
    if (strcmp(parser->current_token->value, "on") == 0 || strcmp(parser->current_token->value, "1") == 0)
    {
        polygon_group->smooth = 1;
        parser_eat(parser, parser->current_token->type);
    }
}

/***
 * Parses:
 *     l v1 v2 v3 ...
 */
void parser_parse_polyline(parser_T* parser)
{
    obj_T* obj = parser->objects[parser->obj_pointer];

    parser_eat(parser, TOKEN_ID); // l
    obj_line_polygon_T* line_polygon = calloc(1, sizeof(struct OBJ_LINE_POLYGON_STRUCT));
    line_polygon->vertex_pointers = calloc(1, sizeof(unsigned int));
    line_polygon->vertex_pointers_size = 0;
    
    while (parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        line_polygon->vertex_pointers_size += 1;
        line_polygon->vertex_pointers = realloc(line_polygon->vertex_pointers, line_polygon->vertex_pointers_size * sizeof(unsigned int));
        line_polygon->vertex_pointers[line_polygon->vertex_pointers_size - 1] = atoi(parser->current_token->value);
        
        parser_eat(parser, TOKEN_INTEGER_VALUE);
    }

    obj->line_polygons_size += 1;
    obj->line_polygons = realloc(obj->line_polygons, obj->line_polygons_size * sizeof(struct OBJ_LINE_POLYGON_STRUCT));
    obj->line_polygons[obj->line_polygons_size - 1] = line_polygon;
}
