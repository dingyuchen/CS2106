#include <stdio.h>
#include <stdlib.h>

/*
 * This function reads from the standard input one character a time, with the
 * `gerchar()` function. Before reading any input, a buffer of size 1 is allocated.
 * To deal with the unknown input, the buffer size is incremented by 1 whenever
 * a new character is read from the standard input. In this way, the buffer is 
 * dynamic and can deal with unknown line length.
 */
int read_palindrome() {
    char *buffer = malloc(1);
    char *start = buffer;
    char *end = buffer;
    // int read_value = 0;
    char read_value = 1;
    
    while (read_value != EOF && read_value != '\n') {
        read_value = getchar();
        *end = read_value;
        buffer = realloc(buffer, end - start + 2);
        end++;
    }
    // check whether input is empty
    if (end - start <= 1) {
        free(buffer);
        return 1;
    } else {
        // move end to point to the last eligible character
        end -= 2;
    }

    // close up the two pointers
    while (end - start >= 1) {
        if (*start != *end) {
            free(buffer);
            return 0; 
        }
        start++;
        end--;
    }
    free(buffer);
    return 1;
}