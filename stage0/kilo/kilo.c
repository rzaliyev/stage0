/* includes {{{  */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
/* }}} */

/* defines {{{ */
#define KILO_VERSION "0.0.1"

#define ESCSEQ              '\x1b'
#define CLEAR_SCREEN        "\x1b[2J"
#define CLEAR_LINE          "\x1b[K"
#define CURSOR_TOPLEFT      "\x1b[H"
#define CURSOR_BOTTOMRIGHT  "\x1b[999C\x1b[999B"
#define CURSOR_POS          "\x1b[6n"
#define CURSOR_HIDE         "\x1b[?25l"
#define CURSOR_SHOW         "\x1b[?25h"
#define CURSOR_MOVE         "\x1b[%d;%dH"

#define CTRL_KEY(k) ((k) & 0x1f)    // 0x1f = 00011111
#define WRITE_LIT(x)        (write(STDOUT_FILENO, (x), sizeof(x) - 1))
#define APPEND_LIT(ab, x)   (abAppend((ab), (x), sizeof(x) - 1))
#define APPEND_STR(ab, x)   (abAppend((ab), (x), strlen(x)))

enum editorKey {
    ARROW_LEFT  = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,

};
/* }}} */

/* data {{{ */
typedef struct {
    int cx, cy;
    int screenrows;
    int screencols;
    struct termios orig_termios;
} editorConfig;

editorConfig E;
/* }}} */

/* terminal {{{ */
void die(const char *s)
{
    WRITE_LIT(CLEAR_SCREEN);
    WRITE_LIT(CURSOR_TOPLEFT);

    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    {
        die("tcsetattr");
    }
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
    {
        die("tcgetattr");
    }
    atexit(disableRawMode);

    struct termios raw = E.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // IEXTEN has no effect
    raw.c_cc[VMIN] = 0;     // read()\x1b returns immediately after VMIN bytes read
    raw.c_cc[VTIME] = 1;    // read() returns after VTIME (100 ms) timeout

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        die("tcsetattr");
    }
}

int editorReadKey()
{
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
        {
            die("read");
        }
    }

    if (c ==  ESCSEQ)
    {
         char seq[3];

         if (read(STDIN_FILENO, &seq[0], 1) != 1) return ESCSEQ;
         if (read(STDIN_FILENO, &seq[1], 1) != 1) return ESCSEQ;

         if (seq[0] == '[')
         {
             switch (seq[1])
             {
                 case 'A': return ARROW_UP;
                 case 'B': return ARROW_DOWN;
                 case 'C': return ARROW_RIGHT;
                 case 'D': return ARROW_LEFT;
             }
         }

         return ESCSEQ;
    }
    else
    {
        return c;
    }
}

int getCursorPosition(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;

    if (WRITE_LIT(CURSOR_POS) != strlen(CURSOR_POS)) return -1;

    while (i < sizeof(buf) - 1)
    {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';

    if (buf[0] != ESCSEQ || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

    return 0;
}

int getWindowSize(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        if (WRITE_LIT(CURSOR_BOTTOMRIGHT) != strlen(CURSOR_BOTTOMRIGHT)) return -1;
        return getCursorPosition(rows, cols);
    }
    else
    {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}
/* }}} */

/* append buffer {{{ */
typedef struct{
    char *b;
    int len;
} abuf;

#define ABUF_INIT {NULL, 0}

void abAppend(abuf *ab, const char *s, int len)
{
    char *new = realloc(ab->b, ab->len +  len);

    if (new == NULL) return;
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}

void abFree(abuf *ab)
{
    free(ab->b);
}
/* }}} */

/* output {{{ */
void editorDrawRows(abuf *ab)
{
    for (int y = 0; y < E.screenrows; y++)
    {
        if (y == E.screenrows / 3)
        {
            char welcome[80];
            int welcomelen = snprintf(welcome, sizeof(welcome),
                "Kilo editor -- version %s", KILO_VERSION);
            if (welcomelen > E.screencols) welcomelen = E.screencols;
            int padding = (E.screencols - welcomelen) / 2;
            if (padding)
            {
                APPEND_LIT(ab, "~");
                padding--;
            }
            while (padding--)
            {
                APPEND_LIT(ab, " ");
            }
            APPEND_STR(ab, welcome);
        }
        else
        {
            APPEND_LIT(ab, "~");
        }

        APPEND_LIT(ab, CLEAR_LINE);
        if(y < E.screenrows - 1)
        {
            APPEND_LIT(ab, "\r\n");
        }
    }
}

void editorRefreshScreen()
{
    abuf ab = ABUF_INIT;

    APPEND_LIT(&ab, CURSOR_HIDE);
    APPEND_LIT(&ab, CURSOR_TOPLEFT);

    editorDrawRows(&ab);

    char buf[32];
    snprintf(buf, sizeof(buf), CURSOR_MOVE, E.cy + 1, E.cx + 1);
    APPEND_STR(&ab, buf);

    APPEND_LIT(&ab, CURSOR_SHOW);

    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}
/* }}} */

/* input {{{ */
void editorMoveCursor(int key)
{
    switch (key)
    {
        case ARROW_LEFT:
            if (E.cx != 0)
            {
                E.cx--;
            }
            break;
        case ARROW_RIGHT:
            if (E.cx != E.screencols - 1)
            {
                E.cx++;
            }
            break;
        case ARROW_UP:
            if (E.cy != 0)
            {
                E.cy--;
            }
            break;
        case ARROW_DOWN:
            if (E.cy != E.screenrows - 1)
            {
                E.cy++;
            }
            break;
    }
}

void editorProcessKyepress()
{
    int c = editorReadKey();

    switch (c)
    {
        case CTRL_KEY('q'):
            WRITE_LIT(CLEAR_SCREEN);
            WRITE_LIT(CURSOR_TOPLEFT);
            exit(0);
            break;

        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;
    }
}
/* }}} */

/* init {{{ */
void initEditor()
{
    E.cx = 0;
    E.cy = 0;

    if (getWindowSize(&E.screenrows, &E.screencols) == -1)
    {
        die("getWindowSize");
    }
}

int main()
{
    enableRawMode();
    initEditor();

    while (1)
    {

        editorRefreshScreen();
        editorProcessKyepress();
    }
    return 0;
}
/* }}} */
