// Copyright (C) 2024 lejennb and licensed under the BSD-2-Clause license

#ifndef REPLACE
#define REPLACE

void editorReplaceSubstring(int find_len, char* replace, int replace_len);
int editorFindSubstring(char* row_chars, int row_len, char* find, int find_len, char* replace, int replace_len);
void editorReplace();

#endif // REPLACE

