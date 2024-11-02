// Copyright (C) 2024 LJ and licensed under BSD-2-Clause

#ifndef COPY
#define COPY

struct editorClipboard {
  char **line_buffer;
  int line_count;
};

extern struct editorClipboard C;

void editorCopy();
void editorPasteBlock();
void editorPasteInto();
void editorFreeClipboard();
void initClipboard();

#endif // COPY

