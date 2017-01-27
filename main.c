#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// NOTE: For now, the number of words & maximum lengths of each word is predefined.
// TODO: Dynamically reallocate 2D array as file is parsed word-by-word (move semantics instead of copying?!)
// TODO:   ...and figure out if lengths of each dimension of array are necessary (possibly get away with just checking for null-terminating character?)

const int NUMBER_OF_WORDS = 7;
const int MAX_WORD_LENGTH = 20;

//The basic definition of the nodes in the Binary Tree
typedef struct node {
    char*data;
    struct node*left;
    struct node*right;
    int count;
} str_node;

/* ERROR_FLAG CODES:
 *      0 => No errors.
 *      1 => File I/O errors.
 *      2 => main argument count error.
 *      Specify more when necessary.
 */
int ERROR_FLAG = 0;

//Note, for our purpose there aren't any exceptions that we could handle that I can think of except SEGFAULT, which we have been careful to check for,
//so the current error handling seems to suffice...

char** readFile(char* fileLocation) {
    FILE* fp = NULL;
    char**wordArray = NULL;

    // NOTE: fopen looks for the file in one of two ways:
    //      1) An explicit path to the file (e.g. "C:\wordlist.txt")
    //      2) An implicit search in the current execution folder of the binary through the dot qualifier (e.g. ".\wordlist.txt")
    //      You will need to copy the supplied wordlist.txt file to wherever it is convenient
    //            (either in your clone of the cmake-build folder for an implicit search,
    //             or to anywhere else for an explicit search).
    fp = fopen(fileLocation, "r");

    //This checks to make sure the file was successfully loaded (fopen() returns a NULL pointer if unsuccessful)
    if (fp == NULL) {
        return NULL;
    }

    // TODO: Potentially replace with single-line malloc/free & use pointer arithmetic for navigation.
    // TODO: Determine the effects of having a 2D array allocated as such being utilized in subscript format in other functions (is it even possible?)
    wordArray = (char **) malloc(NUMBER_OF_WORDS * sizeof(char *));
    for (int i = 0; i < NUMBER_OF_WORDS; ++i) {
        wordArray[i] = (char *) malloc(MAX_WORD_LENGTH * sizeof(char));
    }

    int i = 0;
    while (!feof(fp) && i < NUMBER_OF_WORDS) {
        if (fscanf(fp, "%s", wordArray[i]) != 1) {
            break;
        }
        ++i;
    }

    fclose(fp);
    return wordArray;
}

//Takes argument of the input string and either the left/right pointer of the parent
void insertNode(char* string, str_node*leafptr) {
    //Declare a dynamically created node
    str_node * tmpnode = NULL;
    tmpnode = malloc(sizeof(str_node));

    //Ensure that the node was created successfully
    if(tmpnode != NULL) {
        //Copy the string data instead of implicit sharing or move semantics...
        strcpy(tmpnode->data, string);

        //Set the node pointers to NULl since we followed the paradigm of adding new nodes beyond the leaf without rearranging
        tmpnode->left  = NULL;
        tmpnode->right = NULL;

        //Set the count to an intial value
        tmpnode->count = 1;

        //Allow the parent to point to this newly created node
        leafptr = tmpnode;
    }
}

int main(int argc, char *argv[]) {
    printf("Number of arguments: %d\n", argc);
    if (argc != 2) {
        printf("ERROR: Illegal number of arguments.\n");
        exit(2);
    }
    char** returnedArray = NULL;
    returnedArray = readFile(argv[1]);

    if (returnedArray != NULL) {
        // Check to see that returnedArray was properly returned.
        for (int i = 0; i < NUMBER_OF_WORDS; ++i) {
            if (returnedArray[i] != NULL) {
                printf("%s\n", returnedArray[i]);
            }
        }

        // INSERT ARRAY PARSING FUNCTIONALITY HERE (use predefined globals for now to determine each dimension's length, if necessary)
    } else {
        ERROR_FLAG = 1;
    }

    if (ERROR_FLAG == 1) {
        printf("ERROR: %s could not be found.", argv[1]);
    }

    // TODO: Replace with the TODO's noted in the readFile function, if necessary for optimization.
    if (returnedArray != NULL) {
        for (int i = 0; i < NUMBER_OF_WORDS; ++i) {
            if (returnedArray[i] != NULL) {
                free(returnedArray[i]);
            }
        }
        free(returnedArray);
    }

    return ERROR_FLAG;
}

// TODO: Determine the proper place to free memory...