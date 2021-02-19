/*
 * There is no purpose of this file other than to test the functionality of
 * the clib files
*/

#include <stdlib.h>
#include <stdio.h>
#include "../std/directory.h"
#include "../std/configLoader.h"
#include <time.h>
#include "../std/array.h"

#define END_SUCCESS 0
#define MULTIPLE 1
#define APPENDTEST 50000
#define TESTING_C_TIME_PROGRAM TRUE

#define APPEND '1'
#define REMOVE '2'
#define DISPLAY '3'
#define TRAVERSE '4'
#define LIST_INFO '5'
#define EXIT '6'

int code(int argc, char** argv);
void bar(char* toPrint);
void alist_test();
void list_test();
void strinfo(string_t info);
void test_alist_append(void* ptr, void* data);
void test_list_append(void* ptr, void* data);
void append_test(void* list, void (*append)(void* ptr, void* data));
void arr_append_test();
void clearscreen();
void print_string_alist(alist_t* list);

#define STR_TEST 48

/*
 * okay so it looks to me like an alist is about 0.5-0.75x faster than using a normal list
 * but it seems to be 1/3 slower than using just a normal dynamic array
 * -> im assuming this is because of the patching
*/
// Write your code here

void clearscreen()
{
    for (int i = 0; i < 5; i++)
    {
        printf("\n");
    }
}

void traverse_string_alist(alist_t* list)
{
    if (!list->size)
    {
        return;
    }
    
    string_t buffer = new_string(DEFAULT_BUFFER_LEN);
    uint32_t elemsize = alist_node_size(list);

    alist_node_t* node = array_get_element(list->list_start, list->first, elemsize);
    uint32_t pos = 0;

    while (TRUE)
    {
        printf("CURRENT NODE: %u, %s\n", pos, cstr((string_t*)&node[1]));
        fileio_next_line(stdin, &buffer);

        if (cstring_equals(cstr(&buffer), "b"))
        {
            if (node->prev != ALIST_NULL)
            {
                printf("GOING BACK\n");
                node = array_get_element(list->list_start, node->prev, elemsize);
            }
            else
            {
                printf("AT START OF LIST\n");
            }
        }
        else if (cstring_equals(cstr(&buffer), "n"))
        {
            if (node->next != ALIST_NULL)
            {
                printf("GOING FORWARD\n");
                node = array_get_element(list->list_start, node->next, elemsize);
            }
            else
            {
                printf("AT END OF LIST\n");
            }
            
        }
        else if (cstring_equals(cstr(&buffer), "exit"))
        {
            break;
        }
    }
}
void print_string_alist(alist_t* list)
{
    if (list->first == ALIST_NULL)
    {
        return;
    }

    printf("First: %u\n", list->first);

    unsigned int elemsize = alist_node_size(list);

    uint32_t i = 0;

    alist_node_t* node = array_get_element(list->list_start, i, elemsize);

    while (TRUE)
    {
        printf("%d) Entry:\n\tnext: %u\n\tprev %u\n", i, node->next, node->prev);

        strinfo( *((string_t*)(&node[1])) );

        if (i + 1 == list->size)
        {
            printf("endof list\n");
            break;
        }

        i++;
        node = array_get_element(list->list_start, i, elemsize);
    }
}

int code(int argc, char** argv){
    alist_t list = new_alist(sizeof(string_t));
    list.compare = string_void_compare;

    char input;
    string_t buffer = new_string(DEFAULT_BUFFER_LEN);

    while (TRUE)
    {
        clearscreen();
        printf("%c = APPEND, %c = REMOVE, %c = DISPLAY, %c = TRAVERSE, %c = LIST INFO, %c = EXIT\n", APPEND, REMOVE, DISPLAY, TRAVERSE, LIST_INFO, EXIT);

        if (fileio_next_line(stdin, &buffer) && buffer.len == 1)
        {
            input = cstr(&buffer)[0];
            switch (input)
            {
                case APPEND:
                    printf("Enter what you would like to append:\n");
                    if (fileio_next_line(stdin, &buffer))
                    {
                        printf("APPENDING \"%s\"\n\n", cstr(&buffer));
                        string_t input = string_copy(&buffer);
                        alist_append(&list, &input);
                    }
                    break;
                case REMOVE:
                    printf("What would you like to remove?\n");
                    if (fileio_next_line(stdin, &buffer))
                    {
                        if (alist_remove(&list, &buffer))
                        {
                            printf("REMOVED NODE\n");
                        }
                        else
                        {
                            printf("COULD NOT FIND OR REMOVE\n");
                        }
                    }
                    break;
                case DISPLAY:
                    print_string_alist(&list);
                    break;
                case TRAVERSE:
                    traverse_string_alist(&list);
                    break;
                case LIST_INFO:
                    printf("List Information:\n\tCAPACITY:\n\t\t%u\n\tSIZE:\n\t\t%u\n", list.capacity, list.size);
                    print_string_alist(&list);
                    break;
                case EXIT:
                    return EXIT_SUCCESS;
                default:
                    printf("INVALID INPUT\n");
                    break;
            }
        }
        else
        {
            printf("\nError - invalid input\n\n");
        }
    }





    

    return END_SUCCESS;
}




















void strinfo(string_t info)
{
    printf("\tlen: %d\n\tmax: %d\n\tstr: %s\n", info.len, info.max_len, cstr(&info));
}

// For formatting program output
void bar(char* toPrint){
    int len = strlen(toPrint);
    for (int i = 0; i < len + 1; i++){
        printf("-");
    }
    printf("\n");
    printf("%s\n",toPrint);
    for (int i = 0; i < len + 1; i++){
        printf("-");
    }
    printf("\n");
}

// Entrypoint of the program. Prints headers before and after entering the user's code
int main(int argc, char** argv){
    bar("Program Start");
    clock_t start_t, end_t;

    if (TESTING_C_TIME_PROGRAM == TRUE)
    {
        start_t = clock();
        printf("Starting of the program, start_t = %ld\n", start_t);
    }
    
    int errorCode = code(argc, argv);
    
    if (TESTING_C_TIME_PROGRAM == TRUE)
    {
        end_t = clock();
        printf("End time end_t = %ld\n", end_t);
        printf("Difference: %ld\n", end_t-start_t);
        double timetaken = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("Total time taken by CPU: %lf\n", timetaken);
        printf("Exiting of the program...\n");
    }

    bar("Program End");
    return errorCode;
}


