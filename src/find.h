// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include "row.h"

#ifndef FIND
#define FIND

int editorFindRowLastMatch(erow* row, int start, int end, char* query);
void editorFindCallback(char* query, int key);
void editorFind();

#endif // FIND

