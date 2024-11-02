// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <stdlib.h>
#include <string.h>

#include "copy.h"
#include "input.h"
#include "output.h"
#include "row.h"

struct editorClipboard C;

void editorCopy() {
  // Clear previous copy buffer, if existent
  editorFreeClipboard();

  // Select lines to copy
  char* copy_prompt = editorPrompt("Copy N lines: %s (ESC to cancel)", NULL);
  if (copy_prompt == NULL) { return; }

  C.line_count = atoi(copy_prompt);
  free(copy_prompt);
  if (C.line_count <= 0) { return; }  // Avoid negative values and 0

  // Create new copy buffer
  int line_offset = E.cy + C.line_count;
  if (line_offset > E.numrows) { return; }  // Avoid stepping over the end of the file

  erow* row;
  C.line_buffer = malloc(sizeof(char*) * C.line_count);
  for (int i = 0; i < C.line_count; i++) {
    row = &E.row[E.cy + i];
    C.line_buffer[i] = malloc((row->size + 1) * sizeof(char));  // + 1 needed for '\0'
    strcpy(C.line_buffer[i], row->chars);
  }

  editorSetStatusMessage("Copied %dL", C.line_count);
}

void editorPasteBlock() {
  if (C.line_buffer == NULL) { return; }

  for (int i = 0; i < C.line_count; i++) {
    editorInsertRow(E.cy, C.line_buffer[i], strlen(C.line_buffer[i]));
    E.cy += 1;
  }

  E.dirty = 1;

  editorSetStatusMessage("Pasted %dL", C.line_count);
}

void editorPasteInto() {
  if (C.line_count != 1) { return; }

  char* source = C.line_buffer[0];

  if (E.cy == E.numrows) {  // Nothing to insert into, paste as new line
    editorInsertRow(E.cy, source, strlen(source));
    editorSetStatusMessage("Pasted %dL into line %d", C.line_count, E.cy + 1);
    return;
  }

  int pos = E.cx;
  erow* row = &E.row[E.cy];
  char* cache = malloc((row->size + strlen(source) + 1) * sizeof(char));

  strncpy(cache, row->chars, pos);
  cache[pos] = '\0';
  strcat(cache, source);
  strcat(cache, row->chars + pos);

  editorDelRow(E.cy);
  editorInsertRow(E.cy, cache, strlen(cache));

  free(cache);
  E.dirty = 1;

  editorSetStatusMessage("Pasted %dL into line %d", C.line_count, E.cy + 1);
}

void editorFreeClipboard() {
  if (C.line_buffer) {
    for (int i = 0; i < C.line_count; i++) {
      free(C.line_buffer[i]);
    } 
    free(C.line_buffer);
    C.line_buffer = NULL;
    C.line_count = 0;
  }
}

void initClipboard() {
  C.line_buffer = NULL;
  C.line_count = 0;
}

