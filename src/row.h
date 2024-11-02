// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <termios.h>

#ifndef ROW
#define ROW

typedef struct erow {
  int idx;
  int size;
  int rsize;
  char *chars;
  char *render;
  unsigned char *hl;
  int comment_open;
  int string_raw_open;
  int string_single_open; 
  int string_double_open;
} erow;

struct editorConfig {
  int cx, cy;  // Cursor position
  int rx;
  int rowoff;
  int coloff;
  int screenrows;  // Window rows
  int screencols;  // Window columns
  int numrows;
  erow *row;
  int dirty;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  int hl_queue;
  int hl_queue_pos;
  clock_t hl_queue_time;
  struct editorSyntax *syntax;
  struct termios orig_termios;
};

extern struct editorConfig E;

int editorRowCxToRx(erow *row, int cx);
int editorRowRxToCx(erow *row, int rx);
void editorUpdateRow(erow *row);
void editorInsertRow(int at, char *s, size_t len);
void editorFreeRow(erow *row);
void editorFreeOnEvent(erow *row, int event);
void editorDelRow(int at);
void editorRowInsertChar(erow *row, int at, int c);
void editorRowAppendString(erow *row, char *s, size_t len);
void editorRowDelChar(erow *row, int at);
void editorInsertChar(int c);
void editorInsertNewline();
void editorDelChar();
char *editorRowsToString(int *buflen);

#endif // ROW

