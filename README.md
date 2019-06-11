# OBJLoad
> Wavefront .obj loader

## Usage
> Here is how to use it
```C
#include <objload/obj.h>


int main(int argc, char* argv[])
{
    obj_list_T* object_list = obj_load_from_file(argv[1]);
    obj_T* obj = object_list->objects[0];


    for (int i = 0; i < obj->polygon_groups_size; i++)
    {
        obj_polygon_group_T* group = obj->polygon_groups[i];
        printf("Face group: %s\n", group->name);

        for (int j = 0; j < group->faces_size; j++)
        {
            obj_face_T* face = group->faces[j];

            printf("vertex_pointers:\n");

            for (int k = 0; k < face->vertex_pointers_size; k++)
            {
                unsigned int pointer = face->vertex_pointers[k];
                obj_vec_T* vec = obj->vertice_vectors[pointer - 1];

                printf("pointer %d pointing to vertex: {%0.6f, %0.6f, %0.6f}\n", pointer, vec->x, vec->y, vec->z);
            }
        }
    }

    obj_list_free(object_list);

    return 0;
}
```

## Compile & Install
> Here is how to install it on your system:

    make && sudo make install

> Enjoy!
