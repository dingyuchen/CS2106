int check_palindrome(char *s) {
    // get the end pointer
    char *start = s;
    char *end = s;
    while (*end != 0) end++;
    // check for empty string case and correct end pointer
    if (start == end) {
        // empty string
        return 1;
    } else {
        // correct end pointer position
        end--;
    }

    // close up the two pointers
    while (end - start >= 1) {
        if (*start != *end) {
            return 0; 
        }
        start++;
        end--;
    }
    return 1;
}
