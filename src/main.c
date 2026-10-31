#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>

pid_t pid;

void handle_sigalrm(int sig){
    (void)sig;
    time_t atime = time(NULL);
    printf("%s Alarm reached\n",ctime(&atime));
    kill(pid,SIGKILL);
}

void handle_sigint(int sig){
    (void)sig;
    printf("Program terminated with SIGINT\n");
    exit(0);
}

int handle_signal(int sig, void (*handler)){
    struct sigaction action;
    action.sa_handler = handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    return sigaction(sig,&action,NULL);
}


int main(int argc, char**argv){
    if (argc < 3) {
        fprintf(stderr,"Not enough arguments\n");
        exit(0);
    }
    handle_signal(SIGALRM,handle_sigalrm);
    handle_signal(SIGINT,handle_sigint);
    time_t btime = time(NULL);
    int timer;
    char *endptr;// for the strtol
    char* comm = argv[2];
    errno = 0;

    FILE *stream = fopen("Logs.txt","w");
    if (!stream) {
        fprintf(stderr,"Can't create log stream\n");
        exit(0);
    }// Log stream

    timer = strtol(argv[1],&endptr,10);
    if (endptr == argv[1] || errno != 0 ) {
        if (errno != 0) {
            fprintf(stderr,"Error with timer value: %s\n",strerror(errno));
        }
        if (endptr == argv[1]){
            fprintf(stderr,"Timer number not detected\n");
        }
        exit(1);
    }// Handling strtol

    char* arguments[argc - 1];
    arguments[0] = comm;
    int k = 1;
    for (int i = 3; i < argc; i++) {
        arguments[k] = argv[i];
        k++;
    }
    arguments[argc - 2] = NULL;

    int fd[2];
    pipe(fd);

    pid = fork();
    char buffer[1024];
    if (pid == -1) {
        fprintf(stderr,"Error Creating Stream.\n");
        exit(1);
    }
    else if (pid == 0) {
        // CHILD PROCESS
        close(fd[0]);
        dup2(fd[1],1);
        handle_signal(SIGINT,handle_sigint);
        printf("In child \n");
        fflush(stdout);
        printf("%s %s %d\n",comm,arguments[1],timer);
        fflush(stdout);
        //alarm((unsigned int)timer);
        //fprintf(stderr,"About to EXEC\n");
        execvp(comm,arguments);
    }
    else{
        // PARENT PROCESS
        int kid_status;
        handle_signal(SIGALRM,handle_sigalrm);
        close(fd[1]);
        dup2(fileno(stream),1);
        FILE *in = fdopen(fd[0],"r"); // Directs the other end of the pipe to a custom stream

        alarm(timer);
        if (!in) {
            fprintf(stderr,"Problem creating stream.\n");
            exit(1);
        }
        while (fgets(buffer,1024,in)) {
            btime = time(NULL);
            fprintf(stream,"%s --- %s",ctime(&btime),buffer);
            fflush(stream);
        }
        pid_t kid_waiter = waitpid(pid,&kid_status,0);
        if (kid_waiter == -1) {
            fprintf(stderr, "WAITPID FAILED\n");
        }// Waitpid waits for the program to finish before disarming the timer so it reaps zombie processes.
        alarm(0); // Disarming the timer if the reading loop completes
    }
    return 0;
}