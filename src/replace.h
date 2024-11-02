// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#ifndef REPLACE
#define REPLACE

void editorReplaceSubstring(int find_len, char* replace, int replace_len);
int editorFindSubstring(char* row_chars, int row_len, char* find, int find_len, char* replace, int replace_len);
void editorReplace();

#endif // REPLACE

