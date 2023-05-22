#ifndef PYSTRING
#define PYSTRING

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// String Comparison ===========================================

bool str_eq(const char *string1, const char *string2) {
    return strcmp(string1, string2) == 0;
}

bool str_gt(const char *string1, const char *string2) {
    return strcmp(string1, string2) > 0;
}

bool str_ge(const char *string1, const char *string2) {
    return strcmp(string1, string2) >= 0;
}

bool str_lt(const char *string1, const char *string2) {
    return strcmp(string1, string2) < 0;
}

bool str_le(const char *string1, const char *string2) {
    return strcmp(string1, string2) <= 0;
}

// returns the number of characters that match, and are in the same index
// returns -1 on failure to compare strings
int match(const char *string1, const char *string2) {
    if (string1 == NULL || string2 == NULL) return -1;

    unsigned int i = 0;
    int count = 0;
    while (string1[i] != '\0' || string2[i] != '\0') {
        if (string1[i] == string2[i]) count++;
        i++;
    }

    return count;
}

// returns the index where the first instance of the substring starts
// if no instance is found, return -1
// if failed to compare strings, return -2
int find(const char *string, const char *sub_str) {
    if (string == NULL || sub_str == NULL) return -2;
    
    unsigned int i = 0; // tracks index of string
    unsigned int j = 0; // tracks index of the substring
    while (string[i] != '\0') {
        if(sub_str[j] == '\0') { // if instance of sub is found
            return i - j;        // return the index where instance starts

        } else if(string[i] == sub_str[j]) { // if sub is being match
            j++;                             // increment sub index

        } else if(j > 0) { // if matching failed
            j = 0;         // reset j
        }
        i++;
    }
    // check if sub array is at end of string
    if(sub_str[j] == '\0') { // if instance of sub is found
        return i - j;        // return the index where instance starts
    }

    return -1; // nothing was found
}

// returns true if string contains substring
bool contains(const char *string, const char *sub_str) {
    return find(string, sub_str) >= 0; 
}

// =============================================================

// String Manipulation =========================================

// splits string into an array of tokens based on the given deliminator char, does not change original string
// returns the malloc'ed pointer to the array of strings
// returns NULL of split fails
char **split(const char *string, const char delim, size_t *arr_len) {
    if (string == NULL) return NULL;

    unsigned int i = 0; // tracks index of string

    // find number of tokens, and string length
    size_t tok_len = 1; // number of tokens that should be created
    size_t str_len = 0;
    while (string[i] != '\0') {
        if (string[i] == delim) tok_len += 1; // increment number of tokens if delim is found
        i++; str_len++;
    }

    // allocate array of strings
    char **tokens = (char **) calloc(tok_len, sizeof(char *));
    if (arr_len != NULL) *arr_len = tok_len;

    // create tokens
    char buffer[str_len];   // buffer for temp storing token
    unsigned int j = 0;     // tracks index of buffer
    unsigned int token = 0; // tracks index of tokens array
    i = 0;
    while (string[i] != '\0') {
        if (string[i] == delim) {  // if delim is found
            buffer[j] = '\0';      // end buffer
            tokens[token] = strdup(buffer); // add token to array
            token++;               // move to next token
            j = 0;                 // reset buffer
        } else {
            buffer[j] = string[i]; // add next char to buffer
            j++;
        }

        i++;
    }
    // flush buffer
    if (j > 0) {
        buffer[j] = '\0';         // end buffer
        tokens[token] = strdup(buffer); // add token to array
    }

    return tokens;
}

// concatonates an array of strings together into a malloc'ed string using a deliminator
// returns NULL if concatonation fails
char *join(char **tokens, const size_t arr_len, const char delim, size_t *str_len) {
    if (tokens == NULL) return NULL;

    size_t delim_size = delim == 0 ? 0 : 1; // determine size of delim

    // find total size of concatonated string
    size_t total_len = 0;
    for (unsigned int i = 0; i < arr_len; i++) {
        if (tokens[i] == NULL) continue; // skip string if it doesn't exist
        total_len += strlen(tokens[i]) + delim_size;
    }

    // allocate new string
    char *joined = (char *) calloc(total_len, sizeof(char));
    if (str_len != NULL) *str_len = total_len;

    // copy tokens into joined string
    unsigned int j = 0; // tracks index of joined
    for (unsigned int i = 0; i < arr_len; i++) {
        if (tokens[i] == NULL) continue; // skip string if it doesn't exist
        
        // concatonate token
        unsigned int t = 0; // tracks index of token
        while (tokens[i][t] != '\0') {
            joined[j] = tokens[i][t];
            j++; t++;
        }     
        if (delim && i < arr_len - 1) { joined[j] = delim; j++; } // concatonate delim
    }

    return joined;
}

// removes all characters in symbols from string, removal is done in-place
// returns the number of characters removed
// returns -1 if string or symbols doesn't exist
int strip(char *string, const char *symbols) {
    if (string == NULL || symbols == NULL) return -1;

    unsigned int i = 0; // tracks index of string
    size_t removals = 0; // tracks number of removals have been made
    while (string[i] != '\0') {

        unsigned int j = 0; // tracks index of symbols
        while(symbols[j] != '\0') { // conpare string char to all symbols

            if (string[i] == symbols[j]) { // if string char is a symbol

                // remove symbol
                unsigned int k = i; // tracks following subarray
                while (string[k] != '\0') {
                    string[k] = string[k + 1];
                    k++;
                }
                removals++;
            }
            j++;
        }
        i++;
    }

    return removals;
}

// removes all characters in symbols from string, copy of string is created for removal, original is left un-altered
// returns a pointer to the new string, NULL on failure
char *strip_dup(const char *string, const char *symbols) {
    if (string == NULL || symbols == NULL) return NULL;

    char *temp = strdup(string); // duplicate string for temporary holding

    size_t removals = strip(temp, symbols); // remove symbols in-place from temp

    // allocate smaller string, +1 for terminating \0 char
    char *stripped = (char *) calloc(strlen(string) - removals + 1, sizeof(char));

    strcpy(stripped, temp); // copy stripped string into smaller string

    free(temp); // free temp

    return stripped;
}

// sets all characters to uppercase, returns number of characters changed
// returns -1 if string doesn't exist
int upper(char *string) {
    if (string == NULL) return -1;

    unsigned int i = 0; // tracks index of string
    size_t caps = 0;    // tracks number of chars changed
    while (string[i] != '\0') {
        char temp = string[i];
        string[i] = toupper(string[i]);
        if (temp != string[i]) caps++;
    }

    return caps;
}

// sets all characters to uppercase, returns pointer to new string, original is un-altered
// returns NULL if string doesn't exist
char *upper_dup(const char *string) {
    if (string == NULL) return NULL;
    char *upper_str = strdup(string);
    upper(upper_str);
    return upper_str;
}

// sets all characters to lowercase, returns number of characters changed
// returns -1 if string doesn't exist
int lower(char *string) {
    if (string == NULL) return -1;

    unsigned int i = 0; // tracks index of string
    size_t caps = 0;    // tracks number of chars changed
    while (string[i] != '\0') {
        char temp = string[i];
        string[i] = tolower(string[i]);
        if (temp != string[i]) caps++;
    }

    return caps;
}

// sets all characters to lowercase, returns pointer to new string, original is un-altered
// returns NULL if string doesn't exist
char *lower_dup(const char *string) {
    if (string == NULL) return NULL;
    char *lower_str = strdup(string);
    upper(lower_str);
    return lower_str;
}

// =============================================================

// File Parsing ================================================

int fline(const FILE *file, char *buffer, const size_t buf_len);

char **flines(const FILE *file, const size_t max_char);

// =============================================================

// Garbage Collection ==========================================

// frees array of strings
void delete_strs(char ***array_ptr, const size_t arr_len) {
    if (array_ptr == NULL || *array_ptr == NULL) return; // exit if there isn't anything to free

    // free individual strings
    for (unsigned int i = 0; i < arr_len; i++) {
        if ((*array_ptr)[i] == NULL) continue; // skip string if it doesn't exist
        free((*array_ptr)[i]);
        (*array_ptr)[i] = NULL;
    }

    // free array
    free(*array_ptr);
    *array_ptr = NULL;

    return;
}

// =============================================================

#endif
