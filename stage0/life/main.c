#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define BOARD_COLS 180
#define BOARD_ROWS 45
#define SIZE (BOARD_ROWS * BOARD_COLS)
#define ANSI_COLOR_GREEN   "\033[32m"
#define ANSI_COLOR_RESET   "\033[0m"

int board[BOARD_ROWS * BOARD_COLS] = {0};
int living_cells = 0;

void init_board(int board[])
{
    for (int i = 0; i != SIZE; i++)
    {
        board[i] = 0;
    }
}

void set_cell(int board[], int row, int col)
{
    board[row * BOARD_COLS + col] = 1;
}

void populate_board(int board[])
{
    // R-pentomino
    set_cell(board, 21, 90);
    set_cell(board, 21, 91);
    set_cell(board, 22, 89);
    set_cell(board, 22, 90);
    set_cell(board, 23, 90);
}

int count_neighbors(int board[], int row0, int col0)
{
    int count = 0;
    int idx0 = row0 * BOARD_COLS + col0;

    for (int row = row0 - 1; row < row0 + 2; row++)
    {
        for (int col = col0 - 1; col < col0 + 2; col++)
        {
            int idx = row * BOARD_COLS + col;
            if (col < 0 || col >= BOARD_COLS || row < 0 || row >= BOARD_ROWS || idx == idx0)
            {
                continue;
            }
            count += board[idx];
        }
    }
    return count;
}

void update(int board[])
{
    int next_board[BOARD_ROWS * BOARD_COLS];
    memcpy(next_board, board, sizeof(next_board));
    living_cells = 0;

    for (int row = 0; row != BOARD_ROWS; row++)
    {
        for (int col = 0; col != BOARD_COLS; col++)
        {
            int idx = row * BOARD_COLS + col;
            int count = count_neighbors(board, row, col);
            if (board[idx])
            {
                next_board[idx] = (count > 1 && count < 4) ? 1 : 0;
            }
            else
            {
                next_board[idx] = (count == 3) ? 1 : 0;
            }
            living_cells += next_board[idx];
        }
    }

    memcpy(board, next_board, sizeof(next_board));
}

void draw_board(int board[], char *back)
{
    for (int idx = 0; idx != SIZE; idx++)
    {
        if (board[idx])
        {
            back[idx] = 'X';
        }
        else
        {
            back[idx] = '.';
        }
    }
}

void test()
{
    init_board(board);

    set_cell(board, 0, 1);
    set_cell(board, 1, 0);
    set_cell(board, 1, 1);
    set_cell(board, 1, 2);
    set_cell(board, 2, 0);
    set_cell(board, 2, 2);
    set_cell(board, 3, 0);
    set_cell(board, 3, 1);
    set_cell(board, 3, 2);

    assert(count_neighbors(board, 0, 0) == 3);
    assert(count_neighbors(board, 2, 0) == 4);
    assert(count_neighbors(board, 2, 1) == 8);
    assert(count_neighbors(board, 0, 9) == 0);
    printf("All tests passed successfully!\n");
}

int main(int argc, char *argv[])
{
    if (argc > 1 && (strcmp(argv[1],"-t") == 0 || strcmp(argv[1],"--test") == 0 ))
    {
        test();
        return 0;
    }

    struct termios oldt, newt;
    char ch;

    // 1. Get current settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // 2. Disable canonical mode (ICANON) and local echo (ECHO)
    newt.c_lflag &= ~(ICANON | ECHO);

    // 2. Set STDIN to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // 3. Set minimum characters to read to 1, with no timeout
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;

    // 4. Apply new settings instantly
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    init_board(board);
    populate_board(board);

    char buffer1[SIZE];
    char buffer2[SIZE];

    memset(buffer1, '.', SIZE);
    memset(buffer2, '.', SIZE);

    char *front = buffer1;
    char *back  = buffer2;

    int generation = 1;

    printf("\033[2J\033[?25l");

    while(true)
    {

        memset(back, '.', SIZE);    // clear back buffer

        draw_board(board, back);

        for (int i = 0; i < SIZE; i++) {
            // Only issue an ANSI write call if the character actually changed!
            if (back[i] != front[i]) {
                int x = i % BOARD_COLS;
                int y = i / BOARD_COLS;
                // Move cursor to (x+1, y+1) and print the updated character
                printf("\033[%d;%dH%c", y + 1, x + 1, back[i]);
            }
        }
        fflush(stdout); // Instantly commit changes to the screen

        read(STDIN_FILENO, &ch, 1);
        if (ch == 27)
        {
            break;
        }

        char *temp = front;
        front = back;
        back = temp;

        update(board);
        generation++;

        usleep(100000);
        printf("\033[999;1H");
        printf("generation: %d\tliving cells: %d", generation, living_cells);
    }

// 4. Restore original settings and clear non-blocking flag
    fcntl(STDIN_FILENO, F_SETFL, flags);
    // 6. Restore original terminal settings before exiting
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("\033[999;1H");

    printf("\033[?25h");
    return 0;
}
