// Copyright (C) 2024 lejennb and licensed under the BSD-2-Clause license

#include "row.h"

#ifndef FIND
#define FIND

int editorFindRowLastMatch(erow* row, int start, int end, char* query);
void editorFindCallback(char* query, int key);
void editorFind();

#endif // FIND

