// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <stdlib.h>
#include <string.h>

#include "find.h"
#include "input.h"
#include "row.h"
#include "syntax.h"

int editorFindRowLastMatch(erow* row, int start, int end, char* query) {
  // Find the position of the last match in a row
  int qlen = strlen(query);
  while (end >= start) {
    if (row->render[end] == query[0]) {
      if (!strncmp(&row->render[end], query, qlen))
        return end;
    }
    end--;
  }
  return -1;
}

void editorFindCallback(char* query, int key) {
  static int last_match = -1;
  static int direction = 1;

  static int saved_hl_line;
  static char* saved_hl = NULL;

  if (saved_hl) {
    memcpy(E.row[saved_hl_line].hl, saved_hl, E.row[saved_hl_line].rsize);
    free(saved_hl);
    saved_hl = NULL;
  }

  if (key == '\r' || key == '\x1b') {
    last_match = -1;
    direction = 1;
    return;
  } else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
    direction = 1;
  } else if (key == ARROW_LEFT || key == ARROW_UP) {
    direction = -1;
  } else {
    last_match = -1;
    direction = 1;
  }

  if (last_match == -1) direction = 1;
  int current = last_match;
  int rx = -1;      // This variable will contain the postion of the match
  int same_row = 0; // If the match is found in the same row

  if (current != -1) {
    // We check in the same row if there was a previous match
    erow* row = &E.row[current];

    if (direction == 1) {
      char* match = strstr(&row->render[E.rx + 1], query); // Find a match after the previous match position(E.rx) in the same row 
      if (match) {
        rx = match - row->render;
        E.cx = editorRowRxToCx(row, rx);
        same_row = 1;
      }
    } else {
      // If in reverse direction
      int lfound = editorFindRowLastMatch(row, 0, E.rx - 1, query); // Find the first match before previous match in the same row
      if (lfound != -1) {
        rx = lfound;
        E.cx = editorRowRxToCx(row, rx);
        same_row = 1;
      }
    }
  }

  if (!same_row) {
    // If match was not found in the same_row
    int i;
    for (i = 0; i < E.numrows; i++) {
      current += direction;
      if (current == -1) current = E.numrows - 1;
      else if (current == E.numrows) current = 0;

      erow* row = &E.row[current];
      char* match = strstr(row->render, query);
      if (match) {
        last_match = current;
        E.cy = current;
        rx = match - row->render;
        
        if (direction == -1) {
          // If match found and the direction is reverse we find the last match in that row 
          int lfound = editorFindRowLastMatch(row, rx + 1, row->rsize - strlen(query), query);
          if (lfound != -1) {
            rx = lfound;
          }
        }
        E.cx = editorRowRxToCx(row, rx);
        E.rowoff = E.numrows;
        break;
      }
    }
  }

  if (rx != -1) {
    // Highlight only when match is found
    erow* row = &E.row[current];
    saved_hl_line = current;
    saved_hl = malloc(row->rsize);
    memcpy(saved_hl, row->hl, row->rsize);
    memset(&row->hl[rx], HL_MATCH, strlen(query));
  }
}

void editorFind() {
  int saved_cx = E.cx;
  int saved_cy = E.cy;
  int saved_coloff = E.coloff;
  int saved_rowoff = E.rowoff;

  char *query = editorPrompt("Search: %s (Use ESC/Arrows/Enter)", editorFindCallback);

  if (query) {
    free(query);
  } else {
    E.cx = saved_cx;
    E.cy = saved_cy;
    E.coloff = saved_coloff;
    E.rowoff = saved_rowoff;
  }
}

