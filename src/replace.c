// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "input.h"
#include "output.h"
#include "replace.h"
#include "row.h"

/*** replace ***/

void editorReplaceSubstring(int find_len, char* replace, int replace_len) {
  char* source = replace;

  int start = E.cx - find_len;
  int end = E.cx;

  int replace_diff = replace_len - find_len;
  erow* row = &E.row[E.cy];
  char* cache = malloc((row->size + replace_diff + 1) * sizeof(char));

  strncpy(cache, row->chars, start);
  cache[start] = '\0';
  if (replace) { strcat(cache, source); }
  strcat(cache, row->chars + end);

  editorDelRow(E.cy);
  editorInsertRow(E.cy, cache, strlen(cache));
  E.cx += replace_diff;

  free(cache);
  E.dirty = 1;
}

int editorFindSubstring(char* row_chars, int row_len, char* find, int find_len, char* replace, int replace_len) {
  int match_len = 0;

  if (row_len >= find_len) {
    // Continue at row[i][E.cx] (same line), or row[i][0] (new line)
    int find_i = 0;
    for (int col = E.cx; col < row_len; col++) {
      if (row_chars[col] == find[find_i]) {
        match_len++;
        if (match_len == find_len) {  // Match found
          E.cx = col + 1;  // Move to the end of the match
          editorReplaceSubstring(find_len, replace, replace_len);
          return 1;
        }
        find_i++;
      } else {
        match_len = 0;
        find_i = 0;
      }
    }
  }
  return 0;
}

void editorReplace() {
  char* find_prompt = editorPrompt("Find: %s (ESC to cancel)", NULL);
  if (find_prompt == NULL) { return; }

  char* replace_prompt = editorPrompt("Replace with: %s (ESC to skip)", NULL);

  int find_len = strlen(find_prompt);
  int replace_len = replace_prompt ? strlen(replace_prompt) : 0;

  erow* row;
  int old_cy = E.cy;
  int replace_count = 0;
  int replace_i = 0;
  clock_t replace_timer = clock();

  for (int i = 0; i < E.numrows; i++) {
    E.cy = i;
    E.cx = 0;  // New line, start at row[i][0]
    do {
      row = &E.row[E.cy];
      replace_i = editorFindSubstring(row->chars, row->size, find_prompt, find_len, replace_prompt, replace_len);
      replace_count += replace_i;
    } while (replace_i);
  }

  replace_timer = clock() - replace_timer; 
  double time_delta = ((double)replace_timer)/CLOCKS_PER_SEC;
  editorSetStatusMessage("Replaced '%s' with '%s' (%dx in %fs)", find_prompt, replace_prompt ? replace_prompt : "", replace_count, time_delta);

  free(find_prompt);
  if (replace_prompt) { free(replace_prompt); }

  E.cy = old_cy;
  E.cx = 0;
}

