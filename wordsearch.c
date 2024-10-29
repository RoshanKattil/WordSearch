#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables

void printPuzzle(char** arr, int size);
void printPath(int *path, int wordLen, int size);

void searchPuzzle(char** arr, char* word, int size);
int recursiveSearch(char **arr, int size, char *word, int wordLen, int row, int col, int index, int *path);
int searchFromCell(char **arr, int size, char *word, int row, int col, int *path);

int compareChars(char a, char b) 
{
    if (a >= 'A' && a <= 'Z') a = a + ('a' - 'A');  // Convert 'A'-'Z' to 'a'-'z'
    if (b >= 'A' && b <= 'Z') b = b + ('a' - 'A');  // Convert 'A'-'Z' to 'a'-'z'

    return a == b;
}





int bSize;

// Main function, DO NOT MODIFY
int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
    fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
    return 2;
    }

    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");

    if (fptr == NULL) 
    {
    printf("Cannot Open Puzzle File!\n");
    return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);

    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) 
    {
        *(block + i) = (char*)malloc(bSize * sizeof(char));

        for (j = 0; j < bSize - 1; ++j) 
        {
            fscanf(fptr, "%c ", *(block + i) + j);
        }

        fscanf(fptr, "%c \n", *(block + i) + j);
    }

    fclose(fptr);



    printf("Enter the word to search: ");
    scanf("%s", word);

    // Print out original puzzle arr
    printPuzzle(block, bSize);

    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word, bSize);

return 0;
}


void printPuzzle(char** arr, int size) 
{
// This function will print out the complete puzzle arr (arr).
// It must produce the output in the SAME format as the samples
// in the instructions.
// Your implementation here...


    for (int i = 0; i < size; i++) 
    {
        for (int n = 0; n < size; n++) 
        {
            printf("%c ", *(*(arr + i) + n)); 
        }
        printf("\n");
    }

}

void searchPuzzle(char **arr, char *word, int size) 
{

    int wordLen = 0;

    for (char *ptr = word; *ptr != '\0'; ptr++) 
    {
        wordLen++;
    }

    //1D array of interger pointers for path n*n
    int *path = (int *)malloc(size * size * sizeof(int));

    for (int i = 0; i < size * size; i++) 
    {
        *(path + i) = 0;
    }
    
    //2D double pointer array temp UPDATE NOT NEEDED DIDNT READ INSTUCTIONS BACKTRACTING IS ALLOWED
    

    // Loop through each cell in the arr
    // i = row n = col
    for (int i = 0; i < size; i++) 
    {
        for (int n = 0; n < size; n++) 
        {

            if (searchFromCell(arr, size, word, i, n, path)) 
            {
                printf("Word found!\n");
                printPath(path, wordLen, size);  // Print the path if found

                free(path);
                return;
            }
        }
    }

    printf("Word not found!\n");


    free(path);
}


int recursiveSearch(char **arr, int size, char *word, int wordLen, int row, int col, int index, int *path) 
{
    //check boundaries
    if (row < 0 || row >= size || col < 0 || col >= size) 
    {
        return 0;
    }

    if (!compareChars(*(*(arr + row) + col), *(word+index))) {
        return 0;
    }

    // update the path when charachter is found
    *(path + row * size + col) = index + 1;

    // break
    if (index == wordLen - 1) 
    {
        return 1;
    }

   //array to store differnt searching directions
    int **directions = (int **)malloc(8 * sizeof(int *));

    for (int i = 0; i < 8; i++) 
    {
        *(directions + i) = (int *)malloc(2 * sizeof(int));
    }

    
    int *values = (int *)malloc(16 * sizeof(int));  
    *(values + 0) = -1; *(values + 1) = 0;
    *(values + 2) = 1;  *(values + 3) = 0;
    *(values + 4) = 0;  *(values + 5) = -1;
    *(values + 6) = 0;  *(values + 7) = 1;
    *(values + 8) = -1; *(values + 9) = -1;
    *(values + 10) = -1; *(values + 11) = 1;
    *(values + 12) = 1;  *(values + 13) = -1;
    *(values + 14) = 1;  *(values + 15) = 1;

    //fill directions with combos of -1,0,1 exept for 0,0
    for (int i = 0; i < 8; i++) 
    {
        //row
        *(*(directions + i) + 0) = *(values + 2 * i);
        //column
        *(*(directions + i) + 1) = *(values + 2 * i + 1);
    }

    //basically kind of starts 8 processes that end themselfs if they dont find a mathcing letter
    for (int i = 0; i < 8; i++) 
    {
        int newRow = row + *(*(directions + i) + 0);
        int newCol = col + *(*(directions + i) + 1);

        if (recursiveSearch(arr, size, word, wordLen, newRow, newCol, index + 1, path)) {
            return 1;
        }
    }

    *(path + row * size + col) = 0;

    return 0;
}

//start search
int searchFromCell(char **arr, int size, char *word, int row, int col, int *path) 
{
    int wordLen = 0;
    for (char *ptr = word; *ptr != '\0'; ptr++)
    {
        wordLen++;
    }
    
    return recursiveSearch(arr, size, word, wordLen, row, col, 0, path);
}


void printPath(int *path, int wordLen, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            printf("%d ", *(path + i * size + j));  
        }
        printf("\n");
    }
}
