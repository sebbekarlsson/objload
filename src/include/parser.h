#ifndef OBJ_LOAD_PARSER_H
#define OBJ_LOAD_PARSER_H
#include "lexer.h"
#include "obj.h"

typedef struct PARSER_STRUCT
{
    lexer_T* lexer;
    token_T* current_token;
    obj_T* obj;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_eat(parser_T* parser, int token_type);

void parser_parse(parser_T* parser);

void parser_parse_vector(parser_T* parser);

void parser_parse_face(parser_T* parser);

void parser_parse_material_reference(parser_T* parser);

void parser_parse_material_usage(parser_T* parser);

void parser_parse_line_elements(parser_T* parser);
#endif
