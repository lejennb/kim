// Copyright (C) 2024 lejennb and licensed under the BSD-2-Clause license

#ifndef INPUT
#define INPUT

#define QUIT_MODIFIED 1
#define CTRL_KEY(k) ((k) & 0x1F)

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

int editorReadKey();
char *editorPrompt(char *prompt, void (*callback)(char *, int));
void editorMoveCursor(int key);
void editorProcessKeypress();

#endif // INPUT

