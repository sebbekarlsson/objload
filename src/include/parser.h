#ifndef OBJ_LOAD_PARSER_H
#define OBJ_LOAD_PARSER_H
#include "lexer.h"
#include "obj.h"

typedef struct PARSER_STRUCT
{
    lexer_T* lexer;
    token_T* current_token;
    obj_T** objects;
    size_t objects_size;
    unsigned int obj_pointer;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_eat(parser_T* parser, int token_type);

void parser_parse(parser_T* parser);

void parser_parse_statement(parser_T* parser, obj_polygon_group_T* polygon_group);

void parser_parse_vertice_vector(parser_T* parser);

void parser_parse_texcoord_vector(parser_T* parser);

void parser_parse_normal_vector(parser_T* parser);

void parser_parse_face(parser_T* parser,  obj_polygon_group_T* polygon_group);

obj_polygon_group_T* parser_parse_polygon_group(parser_T* parser);

void parser_parse_material_reference(parser_T* parser);

void parser_parse_material_use(parser_T* parser, obj_polygon_group_T* polygon_group);

char* parser_parse_object_name(parser_T* parser);

void parser_parse_smooth_bool(parser_T* parser, obj_polygon_group_T* polygon_group);

void parser_parse_polyline(parser_T* parser);
#endif
