# OBJLoad
> Wavefront .obj loader

## Usage
> Here is how to use it
```C
#include <objload/obj.h>


int main(int argc, char* argv[])
{
    obj_T* obj = obj_load_from_file("shards/teapot.obj");

    for (int i = 0; i < obj->vectors_size; i++)
    {
        obj_vec_T* vec = obj->vectors[i];

        switch (vec->type)
        {
            case VEC_VERTICE: {
                printf("Its a vertice!\n");
            } break;
            case VEC_TEXCOORD: {
                printf("Its a texcoord!\n");
            } break;
            case VEC_NORMAL: {
                printf("Its a normal!\n");
            } break;
        }

        printf("(%0.6f, %0.6f, %0.6f)\n", vec->x, vec->y, vec->z);
    }

    for (int i = 0; i < obj->faces_size; i++)
    {
        obj_face_T* face = obj->faces[i];

        printf(
            "Face vertex_pointers(%d, %d, %d)\n",
            face->vertex_pointers[0],
            face->vertex_pointers[1],
            face->vertex_pointers[2]
        );
        
        printf(
            "Face texcoord_pointers(%d, %d, %d)\n",
            face->texcoord_pointers[0],
            face->texcoord_pointers[1],
            face->texcoord_pointers[2]
        );

        printf(
            "Face normal_pointers(%d, %d, %d)\n",
            face->normal_pointers[0],
            face->normal_pointers[1],
            face->normal_pointers[2]
        );
    }

    // dont forget to free it
    obj_free(obj);

    return 0;
}
```
