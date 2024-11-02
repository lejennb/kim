// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "copy.h"
#include "input.h"
#include "output.h"
#include "row.h"
#include "syntax.h"
#include "terminal.h"

void editorExit() {
  editorFreeOnEvent(E.row, 1);
  editorFreeClipboard();

  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  write(STDOUT_FILENO, "\x1b[3J", 4);  // Also clear scrollback 
  exit(0);
}

void editorNew() {
  E.hl_queue = 0;
  E.cy = 0;
  editorFreeOnEvent(E.row, 0);
  E.syntax = NULL;

  E.numrows = 0;
  E.cx = 0;

  editorSetStatusMessage("");
  E.dirty = 0;
}

void editorOpen(char *filename) {
  free(E.filename);
  E.filename = strdup(filename);

  editorSelectSyntax();

  if (access(filename, F_OK) != 0) return;  // File not existent, let "editorSave" create it

  FILE *fp = fopen(filename, "r");
  if (!fp) die("fopen");  // Same as "if (fp == 0)"

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, fp)) != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
                           line[linelen - 1] == '\r'))
      linelen--;
    editorInsertRow(E.numrows, line, linelen);
  }
  free(line);
  fclose(fp);
  E.dirty = 0;
}

void editorLoad() {
  char* open_prompt = editorPrompt("Open or create: %s (ESC to cancel)", NULL);
  if (open_prompt == NULL) { return; }

  editorNew();
  editorOpen(open_prompt);
  free(open_prompt);
}

void editorSave(int new_file) {
  if (new_file != 0 && E.filename != NULL) {
    free(E.filename);
    E.filename = NULL;
  }

  if (E.filename == NULL) {
    E.filename = editorPrompt("Save as: %s (ESC to cancel)", NULL);
    if (E.filename == NULL) {
      editorSetStatusMessage("Save aborted");
      return;
    }
    editorSelectSyntax();
  }

  int len;
  char *buf = editorRowsToString(&len);

  int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        E.dirty = 0;
        editorSetStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }

  free(buf);
  editorSetStatusMessage("Can't save! I/O error: %s", strerror(errno));
}

