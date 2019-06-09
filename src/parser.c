#include "include/parser.h"
#include <stdlib.h>
#include <string.h>


parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->obj = init_obj();

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
    while (parser->current_token->type != TOKEN_EOF)
    {
        if (parser->current_token->type == TOKEN_ID)
        {
            if (strcmp(parser->current_token->value, "mtllib") == 0)
                parser_parse_material_reference(parser);

            if (strcmp(parser->current_token->value, "usemtl") == 0)
                parser_parse_material_usage(parser);
        }

        // lets just ignore this for now
        if (parser->current_token->type == TOKEN_OBJECT_NAME)
            parser_eat(parser, TOKEN_OBJECT_NAME);

        if (
            strcmp(parser->current_token->value, "v") == 0 ||
            strcmp(parser->current_token->value, "vt") == 0 ||
            strcmp(parser->current_token->value, "vn") == 0
        )
        {
            parser_parse_vector(parser);
        }

        if (strcmp(parser->current_token->value, "f") == 0)
        {
            parser_parse_face(parser);
        }

        if (strcmp(parser->current_token->value, "s") == 0)
        {
            parser_eat(parser, TOKEN_ID);
            parser_eat(parser, TOKEN_ID);
        }
    }
}

void parser_parse_vector(parser_T* parser)
{
    char* vector_typename = parser->current_token->value;
    int vector_type = 0;

    if (strcmp(vector_typename, "v") == 0)
        vector_type = VEC_VERTICE;
    if (strcmp(vector_typename, "vt") == 0)
        vector_type = VEC_TEXCOORD;
    if (strcmp(vector_typename, "vn") == 0)
        vector_type = VEC_NORMAL;

    parser_eat(parser, TOKEN_ID);

    obj_vec_T* vec = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));

    if (parser->current_token->type == TOKEN_FLOAT_VALUE)
    {
        vec->x = atof(parser->current_token->value);
        parser_eat(parser, TOKEN_FLOAT_VALUE);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE)
    {
        vec->y = atof(parser->current_token->value);
        parser_eat(parser, TOKEN_FLOAT_VALUE);
    }

    if (parser->current_token->type == TOKEN_FLOAT_VALUE)
    {
        vec->z = atof(parser->current_token->value);
        parser_eat(parser, TOKEN_FLOAT_VALUE);
    }

    obj_T* obj = parser->obj;

    obj->vectors_size += 1;
    obj->vectors = realloc(obj->vectors, obj->vectors_size * sizeof(struct OBJ_VECTOR_STRUCT));
    obj->vectors[obj->vectors_size - 1] = vec;
}

void parser_parse_face(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID);

    obj_face_T* face = calloc(1, sizeof(struct OBJ_FACE_STRUCT));
    face->vertex_pointers = calloc(0, sizeof(float));
    face->vertex_pointers_size = 0;
    face->texcoord_pointers = calloc(0, sizeof(float));
    face->texcoord_pointers_size = 0;
    face->normal_pointers = calloc(0, sizeof(float));
    face->normal_pointers_size = 0;

    int vertex_pointer_index = 0;
    int texcoord_pointer_index = 0;
    int normal_pointer_index = 0;

    while (parser->current_token->type == TOKEN_INTEGER_VALUE)
    {
        face->vertex_pointers_size += 1;
        face->vertex_pointers = realloc(face->vertex_pointers, face->vertex_pointers_size * sizeof(unsigned int));
        face->vertex_pointers[vertex_pointer_index] = atoi(parser->current_token->value);
        parser_eat(parser, TOKEN_INTEGER_VALUE);

        vertex_pointer_index += 1;

        if (parser->current_token->type == TOKEN_DIV)
        {
            parser_eat(parser, TOKEN_DIV);

            if (parser->current_token->type == TOKEN_INTEGER_VALUE)
            {
                face->texcoord_pointers_size += 1;
                face->texcoord_pointers = realloc(face->texcoord_pointers, face->texcoord_pointers_size * sizeof(unsigned int));
                face->texcoord_pointers[texcoord_pointer_index] = atoi(parser->current_token->value);
                parser_eat(parser, TOKEN_INTEGER_VALUE);
                texcoord_pointer_index += 1;
            }
        }

        if (parser->current_token->type == TOKEN_DIV)
        {
            parser_eat(parser, TOKEN_DIV);

            if (parser->current_token->type == TOKEN_INTEGER_VALUE)
            {
                face->normal_pointers_size += 1;
                face->normal_pointers = realloc(face->normal_pointers, face->normal_pointers_size * sizeof(unsigned int));
                face->normal_pointers[normal_pointer_index] = atoi(parser->current_token->value);
                parser_eat(parser, TOKEN_INTEGER_VALUE);
                normal_pointer_index += 1;
            }
        }
    } 

    obj_T* obj = parser->obj;

    obj->faces_size += 1;
    obj->faces = realloc(obj->faces, obj->faces_size * sizeof(struct OBJ_FACE_STRUCT));
    obj->faces[obj->faces_size - 1] = face;
}

void parser_parse_material_reference(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID); // mtllib
    parser_eat(parser, TOKEN_ID); // external .mtl filename
}

void parser_parse_material_usage(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID); // usemtl
    parser_eat(parser, TOKEN_ID); // external .mtl filename
}
