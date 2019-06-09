#include "include/obj.h"
#include "include/lexer.h"
#include "include/parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


obj_T* init_obj()
{
    obj_T* obj = calloc(1, sizeof(struct OBJ_STRUCT));

    obj->vectors = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));
    obj->vectors_size = 0;

    obj->texture_vectors = calloc(1, sizeof(struct OBJ_VECTOR_STRUCT));
    obj->texture_vectors_size = 0;

    obj->faces = calloc(1, sizeof(struct OBJ_FACE_STRUCT));
    obj->faces_size = 0;

    return obj;
}

void obj_free(obj_T* obj)
{
    for (int i = 0; i < obj->vectors_size; i++)
    {
        free(obj->vectors[i]);
    }

    for (int i = 0; i < obj->faces_size; i++)
    {
        free(obj->faces[i]);
    }

    free(obj->vectors);

    free(obj->faces);

    free(obj);
}

obj_T* obj_load_from_file(const char* filename)
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

    printf("Done loading %s\n.", filename);

    obj_T* obj = parser->obj;

    free(buff);
    lexer_free(lexer);

    if (strlen(parser->current_token->value))
        free(parser->current_token->value);

    free(parser->current_token);
    free(parser);

    return obj;
}
