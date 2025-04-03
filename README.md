// gamatch.cpp

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <signal.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Global PIDs of agents
pid_t pidX = -1;
pid_t pidY = -1;

int board[6][7]; // 6x7 game board
int turn = 1; // 1 = X, 2 = Y

// Signal handler for Ctrl+C (SIGINT)
void handle_sigint(int sig) {
    cout << "\n[!] Ctrl+C pressed. Exiting cleanly..." << endl;
    if (pidX > 0) kill(pidX, SIGKILL);
    if (pidY > 0) kill(pidY, SIGKILL);
    exit(1);
}

// Handler for SIGCHLD - when a child process terminates
void handle_sigchld(int sig) {
    wait(NULL); // avoid zombie processes
}

// SIGALRM - if agent doesn't respond within 3 seconds
void handle_sigalrm(int sig) {
    cout << "\n[!] Agent did not respond in time. Opponent wins." << endl;
    if (turn == 1 && pidX > 0) kill(pidX, SIGKILL);
    if (turn == 2 && pidY > 0) kill(pidY, SIGKILL);
    exit(0);
}

// SIGTERM - terminate the program
void handle_sigterm(int sig) {
    cout << "\n[!] SIGTERM received. Program terminated." << endl;
    exit(0);
}

// Print the game board to the console
void print_board() {
    cout << "\nGame Board:" << endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// Convert character (A-G) to column index (0-6)
int col_index(char c) {
    return c - 'A';
}

// Place a stone for the player in the specified column
bool place_stone(int col, int player) {
    if (col < 0 || col >= 7) return false;
    for (int i = 5; i >= 0; --i) {
        if (board[i][col] == 0) {
            board[i][col] = player;
            return true;
        }
    }
    return false; // column is full
}

int main(int argc, char* argv[]) {
    // Register signal handlers
    signal(SIGINT, handle_sigint);
    signal(SIGCHLD, handle_sigchld);
    signal(SIGALRM, handle_sigalrm);
    signal(SIGTERM, handle_sigterm);

    // Check input arguments
    if (argc != 5 || string(argv[1]) != "-X" || string(argv[3]) != "-Y") {
        cerr << "Usage: ./gamatch -X <agentX> -Y <agentY>" << endl;
        return 1;
    }

    char* agentX = argv[2];
    char* agentY = argv[4];

    memset(board, 0, sizeof(board)); // clear game board

    // Main game loop
    while (true) {
        int pipe_in[2], pipe_out[2];
        pipe(pipe_in);
        pipe(pipe_out);

        char* agent = (turn == 1) ? agentX : agentY;
        if ((turn == 1 && (pidX = fork()) == 0) || (turn == 2 && (pidY = fork()) == 0)) {
            dup2(pipe_in[0], 0); // redirect stdin
            dup2(pipe_out[1], 1); // redirect stdout
            close(pipe_in[1]);
            close(pipe_out[0]);
            execl(agent, agent, NULL);
            perror("exec failed");
            exit(1);
        }

        close(pipe_in[0]);
        close(pipe_out[1]);

        string input = to_string(turn) + "\n";
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 7; ++j) input += to_string(board[i][j]) + (j < 6 ? " " : "\n");
        }

        write(pipe_in[1], input.c_str(), input.size());

        alarm(3); // give agent 3 seconds to respond
        char buf[100];
        int n = read(pipe_out[0], buf, sizeof(buf) - 1);
        alarm(0); // turn off alarm after successful read

        buf[n] = '\0';
        cout << (turn == 1 ? "X" : "Y") << " agent move: " << buf;

        int col = col_index(buf[0]);
        if (!place_stone(col, turn)) {
            cout << "\n[!] Invalid column. You lose." << endl;
            exit(0);
        }

        print_board();
        wait(NULL); // wait for child process to end

        turn = (turn == 1) ? 2 : 1; // switch turn
    }

    return 0;
}

// Makefile
/*
all:
	g++ gamatch.cpp -o gamatch
*/

// README
/*
# Gamatch

This program runs a 4-in-a-row game between two agent programs.

## How to Run
$ ./gamatch -X /opt/os/hw2/rand_agent -Y /opt/os/hw2/greedy_agent

## How to Build
$ make

## Required System Calls
- fork()
- exec()
- pipe()
- dup2()
- wait()
- kill()
- signal() (SIGINT, SIGCHLD, SIGALRM, SIGTERM)
*/
