// Copyright (C) 2024 lejennb and licensed under the BSD-2-Clause license

#ifndef OUTPUT
#define OUTPUT

#define ABUF_INIT {NULL, 0}

struct abuf {
  char *b;
  int len;
};

void abAppend(struct abuf *ab, const char *s, int len);
void abFree(struct abuf *ab);
void editorScroll();
void editorDrawRows(struct abuf *ab);
void editorDrawStatusBar(struct abuf *ab);
void editorDrawMessageBar(struct abuf *ab);
void editorRefreshScreen();
void editorSetStatusMessage(const char *fmt, ...);

#endif // OUTPUT

