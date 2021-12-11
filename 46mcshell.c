//Writed by Mert Can Yılmaz g191210018 and group 46
//Alihan Sarac g191210057
//Yunus Akgul g191210003
//Salih Yener g191210036
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif                                      //true and false values defined

#define MAX_BUFFER 80                       // Max buffer size defined
#define MAX_ARGS 10                         // Max argument size defined
#define ParseChracther " \t\n\r\a"          // parsing charachters defined                 // 


void type_prompt();                                     //print the prompt
void pid_exec(char **, char*,pid_t*);                   // forks a process and starts another program
char * getcwdstr(char *, int);                          // set program path
char * stripath(char*);                                 // get program path
void add_to_showpid_memory(pid_t,pid_t*);
void systemerror_message(char *, char *);               // print system error
void errormesage(char *, char *);                       // print error
char *getcwd(char *buf, size_t size);
int find_char_count(char* ,char );

/*******************************************************************/

int main (int argc, char * argv[])
{
    FILE * ip = stdin;                  // batch/keyboard inputs
    FILE * op = stdout;                 // (redirected) o/p stream
    char line[MAX_BUFFER];              // line buffer
    char cwdbuf[MAX_BUFFER];            // cwd buffer
    char * args[MAX_ARGS];              // max argument count
    char ** arg;                        // arguments address 
    char * shellpath;                   // structure definations
    int execargs;                       // command running lock
    int i,counter=0;                    // index 
    pid_t child_pids[5] = {0,0,0,0,0};  //showpid memory

    //Mevcut konumdan komutları uygulamaya başlatır
    shellpath = strdup(strcat(strcat(getcwdstr(cwdbuf, sizeof(cwdbuf)),"/"),stripath(argv[0]))); 

    signal(SIGINT, SIG_IGN);       // Ctr+c inbhibition
    signal(SIGCHLD ,SIG_IGN);      // Zombie inhibition
    
    int BUFFER_SIZE_LEAK=0;

    while (!feof(ip)) { // Prompt inpuut wait loop


        if(ip == stdin&&BUFFER_SIZE_LEAK==0) 
        { 
            type_prompt(); //print propmt
            fflush(stdout); //clear buffer memory
        }
        if (fgets(line, MAX_BUFFER, ip ))
        {   
            //read first line from file

            if (MAX_ARGS<find_char_count(line,' ')){
                //Max Arg control
                printf("Max argument count exceeded (%i)\n",MAX_ARGS);

            }else if (NULL==strchr(line,'\n')){
                //Max Buffer control
                BUFFER_SIZE_LEAK=1;
                
			}else if(BUFFER_SIZE_LEAK==1){
                printf("Commands bigger than %i charachter not supported\n",MAX_BUFFER);
                BUFFER_SIZE_LEAK=0;
            }else{
                //Parsing and giving to argument array stage
                arg = args;
                *arg++ = strtok(line,ParseChracther);
                while ((*arg++ = strtok(NULL,ParseChracther))); //if line empty, do nothing
                counter=0;
                for (i = 0; args[i]!=NULL; ++i)
                {
                    if (strcmp(args[i],"")!=0)
                    {
                        counter++;
                    }
                }

                //Control argument availabilty
                if (args[0])
                {                 
                    execargs = TRUE;           
                    

                    if (!strcmp(args[0],"cd")) // if command is cd, change env pwd : built in command
                    {
                        if (!args[1]) 
                        {            
                            printf("%s\n", getcwdstr(cwdbuf, sizeof(cwdbuf)));
                        } else 
                        {
                            if (chdir(args[1]))
                             {  
                                systemerror_message("Root relocating error",NULL);
                            } else 
                            {
                                if (setenv("PWD",strdup(cwdbuf),0))
                                    systemerror_message("File is broken ",NULL);
                            }
                        } 
                        execargs = FALSE;        
                    }else if (!strcmp(args[0],"showpid")){
                        //if command is showpid, show created last 5 child process : built in command
                        for (i = 0; i<5 ; i++){
                            if(child_pids[i]!=0)
                                printf("%d\n",child_pids[i]);
                            else
                                printf("Empty pid field. You should call programs from this prompt\n");
                        }

                    }else if (!strcmp(args[0],"exit"))
                    {
                        //if command is exit, exit :) : built in command
                        printf("exit\n");
                        return 0;
                    }

                    else
                    {
                        //if calling program inside prompt
                        if (execargs)
                        {
                            pid_exec(args,shellpath,child_pids);
                        }
                                                      
                    } 
                }
            }


        }
    }
    return 0; 
}

void type_prompt() // prompt print process
{
    static int firs_time =0;
    if (firs_time == 0 )
    {
        system("clear");     
    }
    firs_time++;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("\33[1;32m%s\33[1;35m/mc46@rootShell >\33[0m ",cwd);
    } else {
       perror("getcwd() error");
    } //prompt yazısı 

}

//Fork child process for execute program
void pid_exec(char ** args, char* shellpath,pid_t* child_pids)
{
    int status;
    pid_t child_pid;
    char tempbuf[MAX_BUFFER];

    switch (child_pid = fork()) {
        case -1:
            systemerror_message("fork",NULL); // if pid -1 send system error
            break;
        case 0:
            signal(SIGINT, SIG_DFL);
            signal(SIGCHLD, SIG_DFL);


            if (setenv("PARENT",strdup(shellpath),0))
                systemerror_message("Setting of PARENT environment failed \n", NULL);

            if (execvp(*args,args)==-1){
                 errormesage("Command cannot be executed \n",NULL);
            }
            kill(getpid(), SIGTERM);

            systemerror_message("exec failed -",args[0]);
            exit(127); //comman not found inside path
    }

    add_to_showpid_memory(child_pid,child_pids);

    waitpid(child_pid, &status, WUNTRACED); //wait for child process ends
    
}

char * getcwdstr(char *buffer, int size)
{
    if(!getcwd(buffer, size)) {  //for write program path
        systemerror_message("getcwd",NULL);
        buffer[0] = 0;
    }
    return buffer;
}

char * stripath(char * pathname) //geth path value
{
    char * filename = pathname;

    if (filename && *filename) {           
        filename = strrchr(filename, '/');
        if (filename)                      
            if (*(++filename))            
                return filename;
            else
                return NULL;
        else
            return pathname;              
    }                                      
    return NULL;
}

void errormesage(char * msg1, char * msg2)  //return error message
{
    fprintf(stderr,"ERROR: ");
    if (msg1)
        fprintf(stderr,"%s; ", msg1);
    if (msg2)
        fprintf(stderr,"%s; ", msg2);
    return;
    fprintf(stderr,"\n");
}
void systemerror_message(char * msg1, char * msg2) //handle system error
{
    errormesage(msg1, msg2);
    perror(NULL);
    return;
}

void add_to_showpid_memory(pid_t pid, pid_t* child_pids){ // adds created child pids into limited array. This array saves only last 5 pids
    
    pid_t temp_pid;
    for(int i=0; i<4; i++){
        temp_pid=child_pids[i+1];
            child_pids[i+1]= child_pids[0];
            child_pids[0]=temp_pid;
        }
    child_pids[0]= pid;
    
}

int find_char_count(char* line,char param){ //finds char count in reading buffer line
    int count = 0;
    for(int i = 0; i< MAX_BUFFER&& line[i]!= '\n';i++){
        if(param==line[i])
            count++;
    }
    return count;
}