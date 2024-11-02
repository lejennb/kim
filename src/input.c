// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "copy.h"
#include "find.h"
#include "input.h"
#include "ops.h"
#include "output.h"
#include "replace.h"
#include "row.h"
#include "syntax.h"
#include "terminal.h"

int editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }

  if (c == '\x1b') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }

    return '\x1b';
  } else {
    return c;
  }
}

char *editorPrompt(char *prompt, void (*callback)(char *, int)) {
  size_t bufsize = 128;
  char *buf = malloc(bufsize);

  size_t buflen = 0;
  buf[0] = '\0';

  while (1) {
    editorSetStatusMessage(prompt, buf);
    editorRefreshScreen();

    int c = editorReadKey();
    if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      if (buflen != 0) buf[--buflen] = '\0';
    } else if (c == '\x1b') {
      editorSetStatusMessage("");
      if (callback) callback(buf, c);
      free(buf);
      return NULL;
    } else if (c == '\r') {
      if (buflen != 0) {
        editorSetStatusMessage("");
        if (callback) callback(buf, c);
        return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1) {
        bufsize *= 2;
        buf = realloc(buf, bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }

    if (callback) callback(buf, c);
  }
}

void editorMoveCursor(int key) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        E.cx--;
      } else if (E.cy > 0) {
        E.cy--;
        E.cx = E.row[E.cy].size;
      }
      break;
    case ARROW_RIGHT:
      if (row && E.cx < row->size) {
        E.cx++;
      } else if (row && E.cx == row->size) {
        E.cy++;
        E.cx = 0;
      }
      break;
    case ARROW_UP:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case ARROW_DOWN:
      if (E.cy < E.numrows) {
        E.cy++;
      }
      break;
  }

  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}

void editorProcessKeypress() {
  static int quit_times = QUIT_MODIFIED;

  int c = editorReadKey();

  switch (c) {
    case '\r':
      editorInsertNewline();
      break;

    case CTRL_KEY('q'):
      if (E.dirty && quit_times > 0) {
        editorSetStatusMessage("Exit: Press Ctrl-Q again (%dx) to discard and quit", quit_times);
        quit_times--;
        return;
      }

      editorExit();
      break;

    case CTRL_KEY('s'):
      editorSave(0);
      break;

    case CTRL_KEY('a'):
      editorSave(1);
      break;

    case CTRL_KEY('n'):
      if (E.dirty && quit_times > 0) {
        editorSetStatusMessage("New: Press Ctrl-N again (%dx) to discard changes", quit_times);
        quit_times--;
        return;
      }

      editorNew();
      break;

    case CTRL_KEY('o'):
      if (E.dirty && quit_times > 0) {
        editorSetStatusMessage("Open: Press Ctrl-O again (%dx) to discard changes", quit_times);
        quit_times--;
        return;
      }

      editorLoad();
      break;

    case CTRL_KEY('u'):
      editorChangeSyntax();
      break;

    case HOME_KEY:
      E.cx = 0;
      break;

    case END_KEY:
      if (E.cy < E.numrows) { E.cx = E.row[E.cy].size; }
      break;

    case CTRL_KEY('c'):
      editorCopy();
      break;

    case CTRL_KEY('v'):
      editorPasteBlock();
      break;

    case CTRL_KEY('p'):
      editorPasteInto();
      break;

    case CTRL_KEY('i'):
      if (TAB_SPACE == 1) {
        for (int i = 0; i < TAB_INDENT; i++) { editorInsertChar(' '); }
      } else {
        editorInsertChar('\t');
      }
      break;

    case CTRL_KEY('f'):
      editorFind();
      break;

    case CTRL_KEY('r'):
      editorReplace();
      break;

    case CTRL_KEY('j'):
      char* jump_prompt = editorPrompt("Jump to line: %s (ESC to cancel)", NULL);
      if (jump_prompt == NULL) { break; }

      int line_index = atoi(jump_prompt);
      free(jump_prompt);

      if (line_index >= 1 && line_index <= E.numrows) {
        E.cy = line_index - 1;
        E.cx = 0;
      }
      break;

    case CTRL_KEY('d'):
      if (E.numrows == 0) { break; }
      if (E.cy >= E.numrows) { break; }  // Avoid stepping over the end of the file

      editorDelRow(E.cy);

      E.cx = 0;
      E.dirty = 1;
      E.hl_queue = 1;
      E.hl_queue_pos = E.cy;
      E.hl_queue_time = clock();

      editorSetStatusMessage("Deleted 1L");
      break;

    case CTRL_KEY('k'):
      char* line_prompt = editorPrompt("Delete N lines: %s (ESC to cancel)", NULL);
      if (line_prompt == NULL) { break; }

      int line_count = atoi(line_prompt);
      free(line_prompt);
      if (line_count <= 0) { break; }          // Avoid negative values and 0
      int line_offset = E.cy + line_count;     // 0-based: E.cy 0 = line 1 + e.g. 4 = lines 1-4, so no ">=" in the next line
      if (line_offset > E.numrows) { break; }  // Avoid stepping over the end of the file

      for (int i = 0; i < line_count; i++) {
        editorDelRow(E.cy);
      }

      E.cx = 0;
      E.dirty = 1;
      E.hl_queue = 1;
      E.hl_queue_pos = E.cy;
      E.hl_queue_time = clock();

      editorSetStatusMessage("Deleted %dL", line_count);
      break;

    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
      if (c == DEL_KEY) editorMoveCursor(ARROW_RIGHT);
      editorDelChar();
      break;

    case PAGE_UP:
    case PAGE_DOWN:
      {
        if (c == PAGE_UP) {
          E.cy = E.rowoff;
        } else if (c == PAGE_DOWN) {
          E.cy = E.rowoff + E.screenrows - 1;
          if (E.cy > E.numrows) E.cy = E.numrows;
        }

        int times = E.screenrows;
        while (times--)
          editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;

    case CTRL_KEY('y'):  // ST emits ctrl-y control character for mouse scroll-up
      for (int i = 0; i < SCROLL_STEP; i++) { editorMoveCursor(ARROW_UP); }
      break;

    case CTRL_KEY('e'):  // ST emits ctrl-e control character for mouse scroll-down
      for (int i = 0; i < SCROLL_STEP; i++) { editorMoveCursor(ARROW_DOWN); }
      break;

    case CTRL_KEY('l'):
    case '\x1b':
      break;

    default:
      editorInsertChar(c);
      break;
  }

  quit_times = QUIT_MODIFIED;
}

