#ifndef BLOCKGAME_FILE_H
#define BLOCKGAME_FILE_H

/// @brief Reads and allocates the contents of a file into a string.
/// @param out Output for the newly allocated string with the file's contents.
/// @param filename Name of the file to be opened.
/// @return Returns zero on success, otherwise nonzero on failure.
int bg_readFile(char **out, char const *filename);

#endif // BLOCKGAME_FILE_H