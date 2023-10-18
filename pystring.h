/*
MIT License

Copyright (c) 2023 Anthony Umemoto

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PYSTRING
#define PYSTRING

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// Constants ===================================================

// =============================================================

// String Comparison ===========================================

/**
 * @brief Returns true if the two strings are equal.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  bool
 */
bool str_eq(const char *string1, const char *string2) {
    return strcmp(string1, string2) == 0;
}

/**
 * @brief Returns true if the first string is greater than the second string.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  bool
 */
bool str_gt(const char *string1, const char *string2) {
    return strcmp(string1, string2) > 0;
}

/**
 * @brief Returns true if the first string is greater than or equal to the second string.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  bool
 */
bool str_ge(const char *string1, const char *string2) {
    return strcmp(string1, string2) >= 0;
}

/**
 * @brief Returns true if the first string is less than the second string.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  bool
 */
bool str_lt(const char *string1, const char *string2) {
    return strcmp(string1, string2) < 0;
}

/**
 * @brief Returns true if the first string is less than or equal to the second string.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  bool
 */
bool str_le(const char *string1, const char *string2) {
    return strcmp(string1, string2) <= 0;
}

/**
 * @brief Returns the number of characters that match, and are in the same index.
 *        Returns -1 on failure to compare strings.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  int
 */
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

/**
 * @brief Returns the index where the first instance of the substring starts.
 *        If no instance is found, return -1.
 *        If failed to compare strings, return -2.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  int
 */
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

/**
 * @brief Returns true if string contains substring.
 * 
 * @param	string1	First string in comparison.
 * @param	string2	Second string in comparison.
 * @return  bool
 */
bool contains(const char *string, const char *sub_str) {
    return find(string, sub_str) >= 0; 
}

// =============================================================

// String Manipulation =========================================

/**
 * @brief Splits string into an array of tokens based on the given deliminator char, does not change original string.
 * Returns the malloc'ed pointer to the array of strings, the length of the array is stored in arr_len.
 * Returns NULL if split fails.
 * 
 * @param	string	The string that will be split.
 * @param	delim	The deliminator used to split the string.
 * @param   arr_len Used to return the length of the tokens array.
 * @return  char ** The malloc'ed tokens array, must be free'd.
 */
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
    if (str_len < 1) return NULL;

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

/**
 * @brief Concatenates an array of strings together into a malloc'ed string using a deliminator.
 * Returns NULL if concatenation fails.
 * 
 * @param	tokens	The array of strings that will be joined.
 * @param	arr_len	The length of the tokens array.
 * @param   delim   The deliminator the tokens will be joined with, set to 0 to not use a deliminator.
 * @param   str_len Used to return the length of the created string.
 * @return  char *  The malloc'ed string, must be free'd.
 */
char *join(char **tokens, const size_t arr_len, const char delim, size_t *str_len) {
    if (tokens == NULL) return NULL;

    size_t delim_size = delim == 0 ? 0 : 1; // determine size of delim

    // find total size of concatenated string
    size_t total_len = 1;
    for (unsigned int i = 0; i < arr_len; i++) {
        if (tokens[i] == NULL) continue; // skip string if it doesn't exist
        total_len += strlen(tokens[i]);
        if (i < arr_len - 1) total_len += delim_size;
    }

    // allocate new string
    char *joined = (char *) calloc(total_len, sizeof(char));
    if (str_len != NULL) *str_len = total_len;

    // copy tokens into joined string
    unsigned int j = 0; // tracks index of joined
    for (unsigned int i = 0; i < arr_len; i++) {
        if (tokens[i] == NULL) continue; // skip string if it doesn't exist
        
        // concatenate token
        unsigned int t = 0; // tracks index of token
        while (tokens[i][t] != '\0') {
            joined[j] = tokens[i][t];
            j++; t++;
        }     
        if (delim && i < arr_len - 1) { joined[j] = delim; j++; } // concatenate delim
    }

    return joined;
}

/**
 * @brief Replace all instances of the substring target with instances of new_sub.
 * Returns a new string that has been malloc'ed, the original is un-altered.
 * 
 * @param string   The original string.
 * @param target   The substring that will be replaced.
 * @param new_sub  The substring instances of target will be replaced with.
 * @param str_len  Used to return the length of the new string.
 * @return char *  The new string with replacements, must be free'd.
*/
char *replace(const char *string, const char *target, const char *new_sub, size_t *str_len) {
    if (string == NULL) return NULL;
    if (target == NULL) return NULL;
    if (new_sub == NULL) return NULL;

    size_t original_len = strlen(string);
    size_t target_len = strlen(target);
    size_t new_sub_len = strlen(new_sub);

    // find targets
    unsigned int target_count = 0;      // track number of target instances found
    unsigned int targets[original_len]; // store targets where target instances start
    for (unsigned int w = 0; w < original_len; w++) {
        targets[w] = original_len + 1; // set to a value it can never be
    }
    unsigned int i = 0, j = 0;          // index for string & targets array
    while (i < original_len) {
        // mask string to search only unvisited chars
        int found = 0;
        if ((found = find(string + i, target)) < 0) {
            break;
        }
        i += found;
        targets[j] = i;
        i++; j++;
        target_count++;
    }

    // new string
    size_t new_len = original_len - (target_len * target_count) + (new_sub_len * target_count) + 1;
    char *new_str = (char *) calloc(new_len, sizeof(char));
    if (str_len != NULL) *str_len = new_len;

    // build new string
    i = 0; // string index
    j = 0; // targets index
    for (unsigned int s = 0; s < new_len - 1; s++) {
        if (i == targets[j]) {
            // replace next target
            for (unsigned int k = 0; k < new_sub_len; k++) {
                new_str[s] = new_sub[k];
                s++;
            }
            i += target_len;
            j++;
        }
        new_str[s] = string[i];
        i++;
    }
    new_str[new_len - 1] = '\0';

    return new_str;
}

/**
 * @brief Sets all characters to uppercase, returns number of characters changed.
 * Returns -1 if string doesn't exist.
 * 
 * @param string The string that will be set to uppercase, cannot be string literal.
 * @return int   The number of characters changed to uppercase.
*/
int upper(char *string) {
    if (string == NULL) return -1;

    unsigned int i = 0; // tracks index of string
    size_t caps = 0;    // tracks number of chars changed
    while (string[i] != '\0') {
        char temp = string[i];
        string[i] = toupper(string[i]);
        if (temp != string[i]) caps++;
        i++;
    }

    return caps;
}

/**
 * @brief Sets all characters to uppercase, returns pointer to new string, original is un-altered.
 * Returns NULL if string doesn't exist.
 * 
 * @param string The string that will be set to uppercase.
 * @return char *
*/
char *upper_dup(const char *string) {
    if (string == NULL) return NULL;
    char *upper_str = strdup(string);
    upper(upper_str);
    return upper_str;
}

/**
 * @brief Sets all characters to lowercase, returns number of characters changed.
 * Returns -1 if string doesn't exist.
 * 
 * @param string The string that will be set to lowercase, cannot be string literal.
 * @return int   The number of characters changed to lowercase.
*/
int lower(char *string) {
    if (string == NULL) return -1;

    unsigned int i = 0; // tracks index of string
    size_t caps = 0;    // tracks number of chars changed
    while (string[i] != '\0') {
        char temp = string[i];
        string[i] = tolower(string[i]);
        if (temp != string[i]) caps++;
        i++;
    }

    return caps;
}

/**
 * @brief Sets all characters to lowercase, returns pointer to new string, original is un-altered.
 * Returns NULL if string doesn't exist.
 * 
 * @param string The string that will be set to lowercase.
 * @return char *
*/
char *lower_dup(const char *string) {
    if (string == NULL) return NULL;
    char *lower_str = strdup(string);
    upper(lower_str);
    return lower_str;
}

// =============================================================

// Garbage Collection ==========================================

// frees array of strings
/**
 * @brief Frees an array of strings. The reference to the array will be set to NULL.
 * 
 * @param arr_ptr A pointer to the array that will be free'd.
 * @param arr_len The length of the array of strings.
*/
void delete_strs(char ***arr_ptr, const size_t arr_len) {
    if (arr_ptr == NULL || *arr_ptr == NULL) return; // exit if there isn't anything to free

    // free individual strings
    for (unsigned int i = 0; i < arr_len; i++) {
        if ((*arr_ptr)[i] == NULL) continue; // skip string if it doesn't exist
        free((*arr_ptr)[i]);
        (*arr_ptr)[i] = NULL;
    }

    // free array
    free(*arr_ptr);
    *arr_ptr = NULL;

    return;
}

// =============================================================

#endif
