#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define MAX_BUFFER 1024			// max line buffer
#define MAX_FILENAME 257                // max length of the file 
                                        // path

#define MAX_ARGS 64			// max # args
#define SEPARATORS " \t\n"		// token separators
#define ERASE 1
#define MIMIC 2
#define MORPH 4

// ennvironment array
extern char ** environ;


/*
   function headers, description for each listed at the bottom 
*/ 
void copyFile(char source[], char dest[]);
//void listFilesSimple();
void listFiles(char flag[]);
int is_directory(const char *path);
FILE* outputFileRedirection(const char *path);
FILE* inputFileRedirection(const char *path);
FILE* appendFileRedirection(const char *path);
void doFork(char *args[]);

typedef struct filemanin_st filemanip;

/* TODO:
   - replace us of system() with fork and exec. This include the commands that are
   sent to the shell.
   
   - You will now need to more fully parse the incoming command line so that 
   you can set up the argument array (char *argv[] 
   in the above examples). N.B. remember to malloc/strdup and 
   to free memory you no longer need!

   - You will find that while a system function call only returns after 
   the program has finished, the use of fork means that two processes 
   are now running in foreground. In most cases you will not want your 
   shell to ask for the next command until the child process has finished. 
   This can be accomplished using the wait or waitpid functions 
   (see project 2 specs for more detail).

   - Be able to handle I/O Redirection
   */

int main(int argc, char ** argv)
{
        setbuf(stdout, NULL);
	char buf[MAX_BUFFER];		// line buffer, determines the amound of arguements entered
	char * args[MAX_ARGS];		// max # args
	char ** arg;			// working pointer thru args
	char * prompt = "==>";		// shell prompt
	char bufCopy[MAX_BUFFER];       // copy before tokenizing, used to fix bug in buf
        FILE *input;
        char promptDir[100];           // string that will contain the current working directory
	// since these string commands need to be seperated using a tokenizer,
	// we need delim array to hold these commands so it can be easy to
	// make if statements to perform actions on these commands
	char delim[] = " ";
   
        // create a copy of the open file descriptor
        int saved_stdout = dup(1);
        // if the amound of arguements is greater than 
        if (argc > 1)
        {
            // input fopen the file as a read only permission
            input = fopen (argv[1], "r");
        }
        else    // set the FILE objects to standard in
        {
            input = stdin;
        }
	// while true
	while (1)
	{     
            // use getcwd() and strcat()
            // functions to print current working
            // directory before the "==>" prompt.
            // i.e. "/projects/2==>"
               
           
            //  getcwd(currentDir, sizeof(currentDir));                     // gets the current working directory
            getcwd(promptDir, sizeof(promptDir));
            printf("%s%s", promptDir, prompt);			        // write prompt
                						        // writes a string up tto null character

            if (fgets (buf, MAX_BUFFER, input))			// read a line
            {
                // if the arguements exceed one
                if ( argc > 1 )
                {
                    // print the buf
                    printf("%s", buf);
                }
                // used counter to assist deep copy to fix bug problem with buf
                int counter = 0;
                // deep copy a bug relating to buf, and copy over to buf to fix it
                bufCopy[counter] = buf[counter];
                // while there are no null values in buf
                while( buf[counter] != '\0' ) 
                {
                    // count the amound of arguements
                    counter++;
                    // deep copy buf to bufCopy
                    bufCopy[counter] = buf[counter];
                }
                // tokenize the input into args array
                arg = args;
                *arg++ = strtok(buf, SEPARATORS);		// tokenize input
                // argCounter used to count amount of aguements entered,
                // note that it begins at one because the user needs to type a command to begin
                
                int argCounter = 1;
                // the program
   
                // while arguements are typed in, tokenize the arguements
                while ((*arg++ = strtok(NULL, SEPARATORS))) 
                {
                    argCounter++;                           // count each arguement that is present
                }

                // search args for ">"
                // if found, the next index is teh filename
                // mimic src dst > output.txt
                // send filename to ioredirect function

		//int saved_stdout = dup(1);

                //printf("Processing command with %d args.\n", argCounter);
                // last entry will be
                // NULL
                if (args[0]) // if the first arguement...
                {
                    // check for internal/external command
                    if (!strcmp(args[0], "wipe")) 		// "wipe" command
                    {
                        // system function clear and continue
                        char *tempArg[2];
                        tempArg[0] = "clear";
                        tempArg[1] = NULL;
                        doFork(tempArg);
                        //system("clear");
                        continue;
                    }
                    else if (!strcmp(args[0], "esc"))	// "quit" command
                    {
                        break;				// break out of 'while' loop and exit program
                    }
                    /* filez [target] — list the contents or the target 
                    // directory (ls -1 [target]).
                    // If [target] is a file, you should only print that file.
                    // If no target is specified, the program should print out
                    // the files in the current working directory.
                    // You will need to provide some command line parsing
                    // capability to extract the target directory for listing.
                    // Once you have built the replacement command line,
                       use the system function to execute it. 
                       The files should sorted by name in ASCII order.
                    */
                    else if (!strcmp(args[0], "filez"))
                    {
                        // print all files in a directory and whether they are folders
                        // arguement present
                        if(argCounter == 1)
                        {
                            char *tempArg[2];
                            tempArg[0] = "ls";
                            tempArg[1] = "-1";
                            tempArg[2] = NULL;
                            doFork(tempArg);
                            //listFilesSimple();
                        }
                        else                            // if argCounter is more than one
                        {
                            listFiles(args[1]);
                        }
                    }
                    /*
                       environ — list all the environment strings - the
                       environment strings can be accessed from within a
                       program by specifying the POSIX compliant environment list:
                       extern char **environ; as a global variable.
                       environ is an array of pointers to the environment
                       strings terminated with a NULL pointer.
                       (see environ.c in specs for examples of use)
                     */
                    else if (!strcmp(args[0], "environ"))
                    {
                        // create a variable that holds environment strings 
                        char ** env = environ;
                        // print environment strings
                        while (*env) printf("%s\n", *env++);
                    }
                    
                    /*
                       ditto [comment] - Calling this will print the 
                       comment line on the display (without the word ditto). 
                       Remember that [comment] can be multiple words. 
                       This command always prints to the stdout.
                     */
                    else if (!strcmp(args[0], "ditto"))
                    {
                        // assign arg to be the arguements string that is typed by suer
                        arg = args;
                        // increment arg to accept the next arguement being passed in
                        arg++; 
                        // while loop is not null, stdout the contents, then increment 
                        // to accept the next arguemtn
                        while (*arg) fprintf(stdout, "%s ", *arg++);
                        // write to stdout
                        fputs ("\n", stdout);
                    }
                    
                    /*
                       help - This command will print the contents of the 
                       README file. 
                       More information about the contents of the README file is 
                       in the specs.
                    */
                    else if (!strcmp(args[0], "help"))
                    {
                        FILE *file;	// file object pointer to open README.txt
                        // fopen the path to README, "r" gives read permission
                        file = fopen("/projects/1/README.txt", "r");	
			
                        // if file is NULL
                        if (file == NULL)
                        {
                            // stderr error message
                            fprintf(stderr, "README.txt is not found!\n");
			
                        }
                        else // file is not NULL
                        {
                            // assign c to get the file
                            int c = fgetc(file);
                            // while the file is not end of file
                            while (c != EOF)
                            {
                                // print README
                                printf("%c", c);
                                // update c every iteration
                                c = fgetc(file);
                            }
                            // close file once done printing
                            fclose(file);
                        }
                    }
                    // mimic [src] [dst] - This command copies the file 
                    // pointed to by [src] and writes a new file pointed to by [dst]. 
                    // It overwrites any file that may previously existed. 
                    // Both [src] and [dst] may be relative or full paths, 
                    // but they must be files. 
                    // Your code must copy the bytes from src and write 
                    // them to dst to create this file. 
                    // You must use syscalls or libc wrapper functions. 
                    // Do not use system.
                    else if (!strcmp(args[0], "mimic"))
                    {
                        // if there is not two or three parameters, error
                        if (!args[1] || !args[2]) 
                        {
                            fprintf(stderr, "error mimic takes two or three parameters\n");
                        }
                        else if (!strcmp(args[1], "-r")) // if user said "mimic -r"
                        {
                            // user typed: mimic -r 
                            printf("USER SELECTED -r in MIMIC\n");
                            if (!args[2] || !args[3]) // if there is not two or three parameters, error
                            {
                                fprintf(stderr, "error mimic takes two or three parameters\n");
                            }
                            else // if valid amount of arguemnts are present after "mimic -r"
                            {
                                if(is_directory(args[2])) // if 2nd arguement is a directory
                                {

                                    printf("Recursive directory copy to %s\n", args[3]);
                                    // TODO: do last;
                                    // create a folder named args[2] inside the folder args[3]
                                    // go through ALL files in args[2], and copy them into the folder created in args[3]
                                }
                                else // else 2nd arguement is a file; mimic -r t1Dir file1.txt
                                {
                                    printf("Recursive file copy (what is that?)\n");
                                    copyFile(args[2], args[3]);

                                }
                                
                            } 
                        }
                        else // else user has not selected -r 
                        {
                            // mimic src dst
                            if(is_directory(args[1])) // if src is a directory
                            {
                                // 
                                // TODO: MOVE BELOW CODE INTO FUNCTION
                                //do first;
                                printf("copying directory to directory %s...\n", args[2]);
                                //check if args[1] is empty (if not throw an error)
                                int srcFileCount = 0;
                                // struct used to 
                                struct dirent *de;
                                // assign DIR objec to open directory of the first arguement
                                DIR *dr = opendir(args[1]);
                                // if directory is NULL, stderr
                                if(dr == NULL)
                                {
                                    fprintf(stderr, "Could not open current directory\n");
                                }
                                else // print directory
                                {
                                    // while reading directory is not null
                                    while((de = readdir(dr)) != NULL)
                                    {
                                        srcFileCount++;
                                        // print it out 
                                        // printf("%s\n", de->d_name);
                                    }
                                    // close directory
                                    closedir(dr);
                                }
                                //TODO: MOVE ABOVE CODE INTO A FUNCTION
                                if(srcFileCount == 0)
                                {
                                 
                                    
                                    //mkdir
                                }
                                else 
                                {
                                    printf("Error: folder has files and -r not included\n");
                                }


                                // create a folder named args[1] inside args[2]
                                // (done)
                            }
                            else // if src is a file, then copy like normal
                            {
                                copyFile(args[1], args[2]);

                            }
                        }
                    }

                    /*
                       The rmdirz [path] command is only expected to work on empty 
                       directories

                       Example,
                       rmdirz foo/bar will remove the directory bar from foo only if 
                       bar is empty. 
                    */
                    else if (!strcmp(args[0], "rmdirz"))
                    {

                        // printf("chdir commands needs to be implemented\n");
                       
                        //printf("CHANGING DIRECTORY\n");
                        
                        // stat that will be helpful when 
                        // determining if a directory exists
                        // or not
                        struct stat dir;
                        // isdir used to assign the
                        // return value of stat dir
                        int isdir;
                        isdir = stat(*arg, &dir);
                        //printf("ISDIR %d\n", isdir);
                        
                        // change the present working directory 
                        
			
                        // to the directory pointed to by [path]
                        if (S_ISDIR(dir.st_mode) != -1)
                        {
                            //printf("removing %s\n", args[1]);
                            //update directory
                            rmdir(args[1]);
                            //remove(args[1]);
                            //strcpy(directory, "PWD=");
                            //printf("REMOVED!\n");
                            //getcwd(currentDir, sizeof(currentDir));
                            
                            //strcat(directory, currentDir);
                            
                            //putenv(directory); // update environ variable using putenv function
                        }
                        else    // flag == -1 
                        {
                        
                            // directory does not exist
                            
                            fprintf(stderr, "Directory does not exist\n");
                        }

                    }
                    /*
                       The mkdirz [path] command must contain a [path that does not
                       already exist. The parent of [path] must exist.

                       Example, 
                       mkdirz foo/bar/baz will create directory baz in foo/bar/ 
                       relative to the current working directory (the latter must
                       already exist).
                    */
                    else if (!strcmp(args[0], "mkdirz"))
                    {
                        // set arg to args
		 	arg = args;
                        // stat used to determine
                        struct stat dir;
                        // isdir 
                        int isdir;
                        // set isdir to the return type of stat
			isdir = stat(*arg, &dir);
                        // if the stat is a directory is successful
                        if (S_ISDIR(dir.st_mode) != -1)
                        {
                            
			    //printf("creating %s\n", args[1]);
			    // string buffer to hold the arguement
                            char buffer[100];
                            // counter used to count the valid arguements
			    int counter = 0;
                            // while the first arguement is not null
			    while(args[1][counter] != '\0') 
                            {
                                // set each element of args to the first arguement 
				buffer[counter] = args[1][counter];
                                // count the arguement
				counter++;
			    }
                            // while the arguement counter is greater than 0 and buffer element is not at the '/' 
                            // root directory
			    while(counter > 0 &&  buffer[counter] != '/') 
                            {
                                // set buffer elements to null then decrement 
                                // arguement
				buffer[counter] = '\0';
				counter--;
			    }
			    //printf("Parent dir: %s\n", buffer);
                            //TODO: verify that parent directory (buffer) is a directory
                            //FIXME: bug here where it creates a directory
                            // where you don't have permission to 
                            // do certain actions, i.e. won't delete directory 
                            // b/c I have not permission to do so
                            
                            //if
                            // use mkdir function to create the specified directory
			    mkdir(args[1], 0777); // 0777 will give permission 
			    


                            //printf("removing %s\n", args[1]);
                            //update directory
                            //remove(args[1]);
                            //strcpy(directory, "PWD=");
                            //printf("REMOVED!\n");
                            //getcwd(currentDir, sizeof(currentDir));
                            
                            //strcat(directory, currentDir);
                            
                            //putenv(directory); // update environ variable using putenv function
                        }
                        else    // flag == -1 
                        {
                            // directory does not exist
                            fprintf(stderr, "Directory does not exist\n");
                        }
                    }
                    // erase [myfile] - This command deletes the 
                    // file pointed to by the parameter [myfile].
                    //  Do not use the system function to implement the erase function.
                    else if (!strcmp(args[0], "erase"))
                    {
                        remove(args[1]);
                    }
                                
                    /*
                    
                       morph [src] [dst] - Move the file or directory indicated by [src] to [dst], 
                       changing the name to the name described in [dst]. For file transfers,
                       your code must move the bytes from src and write them to dst to create this file. 
                       You must use syscalls or libc wrapper functions. 
                       Do not use system. Cases:
                       [src] does not exist: this is an error
                       [dst] does not exist and its parent does not exist 
                       (/foo/bar/baz: /foo exists, but bar does not): this is an error
                       [dst] does not exist, but its parentf does exist 
                       (/foo/bar/baz: /foo/bar exists, but baz does not): 
                       move/copy the directory/file so that it is now named [dst]
                       (/foo/bar/baz in the example)

                       [dst] exists and is a directory (/foo/bar/baz/ exists): move/copy [src] into [dst]
                       [src] is a directory and [dst] is a file: this is an error
                     */
                    else if (!strcmp(args[0], "morph"))
                    {
                        if (!args[1] || !args[2]) // if the user types other than two or three parameters
                        {
                            fprintf(stderr, "error morph takes two or three parameters\n");
                        }
                        else    // else the user has selected the correct parameters needed
                        {
                            // stat used to determine a directory exists
                            struct stat dir;
                            // isDir used as a flag to see if a directory exists
                            int isDir;
                            // fd_in and out is used to see if a file/directory can be opened
                            int fd_in;
                            int fd_out;
                            // set arg to be args
                            arg = args;
                            // increment arg to get the src input 
                            *arg++;
                            // if src exists
                            if ((fd_in = open(args[1], O_RDWR)) >= 0)
                            {
                                // case 2: [dst] and its parent do not exist
                                // check if file eixsts, if not
                                // exists used as a flag 
                                int exists;
                                // set it the return type of when accessing a file is 
                                // possible
                                exists = access(args[2], F_OK);
                                if(exists == -1) // dst file does not exist
                                {
                                    // check if parent exists
                                    // whatever/folder/subfolder/file.txt must become whatever/folder/subfolder
                                    // if not, error
                                    // create a temp buffer of size 100
                                    char tempBuffer[100];
                                    // counter used to help parse directory
                                    int counter = 0;
                                    // while the dst is not null
                                    while(args[2][counter] != '\0')
                                    {
                                        // print and count the amount of arguements present 
                                        //printf("%d %c \n", counter, args[2][counter]);
                                        tempBuffer[counter] = args[2][counter];
                                        counter++;
                                    }
                                    // while the file is a successful pathway
                                    // and while the tempBuffer does not hit a '/' while parsing
                                    while(counter >= 0 && tempBuffer[counter] != '/')
                                    {
                                        //printf("%d %c \n", counter, tempBuffer[counter]);
                                        tempBuffer[counter] = '\0';
                                        counter--;
                                        // morph
                                        // users/me/files/whatever.txt
                                        // documents/temp/file.txt
                                    }
                                    if(counter != -1) // parent folder 
                                    {
                                        // set isDir to the return type succeeds or fail
                                        isDir = stat(*arg, &dir); //chdir(tempBuffer); check if this fails
                                        if(S_ISDIR(dir.st_mode) != 0) // if it failes, means no parent folder
                                        {
                                            fprintf(stderr, "Error, no parent folder\n");
                                            continue;
                                        }
                                        else // if it succeeds, then copy into new parent folder
                                        {
                                            copyFile(args[1], args[2]);
                                            // remove original file
                                            remove(args[1]);
                                        }
                                    }
                                    else // if not parent folder 
                                    {
                                        // copy src to dst
                                        copyFile(args[1], args[2]);
                                        //remove original file
                                        remove(args[1]);
                                    }
                                }
                                else // dst does exist 
                                {
                                    // figure out if args[2] is a file or folder
                                    if ((fd_out = open(args[2], O_RDWR)) >= 0)
                                    {
                                        // copy src to dst
                                        copyFile(args[1], args[2]);
                                        //remove args1
                                        remove(args[1]);
                                    }
                                    else // if not folder 
                                    {
                                        // morph file.txt dumbfolder/
                                        // morph file.txt dumbfolder
                                        // (needs to become)
                                        // morph file.txt dumbfolder/file.txt
                                        // temp buffer to hold directory stuff
                                        char tempbuffer[100];
                                        int counter = 0;
                                        // while dst is not null
                                        while(args[2][counter] != '\0')
                                        {
                                            // copy string from tempbuffer from the dst
                                            tempbuffer[counter] = args[2][counter];
                                            // increment character
                                            counter++;
                                        }
                                        // if dst does not contain a / at the end
                                        if(args[2][counter-1] != '/') 
                                        {
                                            // then update tempbuffer to that /
                                            tempbuffer[counter] = '/';
                                            // increment
                                            counter++;
                                        }
                                        // used to decide nullspace is there
                                        int counter2 = 0;
                                        // while dst is not null
                                        while(args[1][counter2] != '\0')
                                        {
                                            // copy dst into tempBuffer then increment
                                            tempbuffer[counter] = args[1][counter2];
                                            counter++;
                                            counter2++;
                                        }
                                        // set tempbuffer to null
                                        tempbuffer[counter] = '\0';
                                    }
                                }
                            }
                            else // src does not exist, print error
                            {
                                fprintf(stderr, "Source file does not exist\n");
                            }
                        }
                    }
                    
                    /*
                       chdir [path] - Changes the present working directory to 
                       the directory pointed to by [path]. If no [path] is supplied, 
                       the code should print the current working directory followed 
                       by a new line to stdout. Note that only a chdir will not work. 
                       You also need to update the PWD environment variables using 
                       the putenv function. You do do not need to update the OLDPWD environment variable.
                     */
                    else if (!strcmp(args[0], "chdir"))
                    {
                        // printf("chdir commands needs to be implemented\n");
                        //printf("CHANGING DIRECTORY\n");
                        int flag = chdir (args[1]);     // 0 for success, -1 for failed path
                        char directory[100];            // "PWD="
                        char currentDir[100];           // "pathname" that will update PWD        
                        
                        // change the present working directory 
                        // to the directory pointed to by [path]
                        if (flag == 0)
                        {
                            //update directory
                            strcpy(directory, "PWD=");
                            getcwd(currentDir, sizeof(currentDir));
                            strcat(directory, currentDir);
                            putenv(directory); // update environ variable using putenv function
                        }
                        else    // flag == -1 
                        {
                            // directory does not exist
                            fprintf(stderr, "Directory does not exist\n");
                        }
                    }
                    else // else pass command onto OS (or in this instance, print them out)
                    {
                        //printf("doing fork on: %s...\n", args[0]);
                        // use system call to use OS operating system 	
			//char** subArgs = malloc(1000);
			//for(int i=0;i<argCounter;i++) {
			//	subArgs[i] = args[i];
			//}
                        //printf("before forking %s...\n", *subArgs);
                        //printf("passing in doFork(%s)...\n", *args);
                        doFork(args);
                        //printf("after doFork(%s).\n", *args);
                        //system(bufCopy);  
                    }
                }
            }
            // after the end of each iteration, will create
            // a copy of the file descriptor using the lowest-numbered
            // unused descriptor for the new descriptor.
            // int dup2(int oldfd, int newfd)
            
            //dup2(saved_stdout, 1);
	    //                 ^ FIXME: may be a bug here
        
            //printf("Normal output resumed; while loop repeating\n");
	    // could use freopen() instead?
        }

}

// copy functions from src to dst
void copyFile(char source[], char dest[])
{
    // file directory in and out used to change directories
    int fd_in;
    int fd_out;
    // if the first arguement can be opened successfully 
    if ((fd_in = open(source, O_RDWR)) >= 0)
    {
        //printf("Copying file to %s\n", args[2]);
	// if fd_out can open dst, allowing user to read and write
        if ( (fd_out = open(dest, O_CREAT|O_WRONLY, 0664)) >= 0)
	{
            // char used to print string
            char c;
            // while reading is valid, write 
            while (read(fd_in, &c, 1) == 1)
	    {
                write(fd_out, &c, 1);
	    }
        }
        else    // else print stderr
        {
        
            fprintf(stderr, "Could not open write file\n");
        }

    }
    else    // cant open read file 
    {
        fprintf(stderr, "You cannot write a directory to a file\n");
    }
    // close file directories in and out
    close(fd_in);
    close(fd_out);
}

// listFiles that takes in a flag arra
void listFiles(char flag[])
{
    // used stat to obtain the info (or path) about the named file
    // and write it to the area pointed to by the buf arguement
    struct stat path_stat; 		
    // if there is one arguement, then that means "filez" is the only 
    // assign fileExists to stat, 0 = successful, -1 = failed path
    int fileExists = stat(flag, &path_stat);
    // if the file doesn't exist, then stderr
    if(fileExists == -1)
    {
    
        fprintf(stderr, "File not found\n");
    }
    // if user types the name of a regular
    // file (IS_REG) that exists in directory path
    else if( S_ISREG(path_stat.st_mode))
    {

        printf("%s\n", flag); // print the name of the file out
    }
    // if the name of the file does not exist in 
    // directory, then stderr error message
    else if (S_ISDIR(path_stat.st_mode)) 
    {
        // struct used to 
	struct dirent *de;
        // assign DIR objec to open directory of the first arguement
	DIR *dr = opendir(flag);
        // if directory is NULL, stderr
	if(dr == NULL)
	{
            fprintf(stderr, "Could not open current directory\n");
	}
	else // print directory
        {
            // while reading directory is not null
            while((de = readdir(dr)) != NULL)
	    {
                // print it out 
		printf("%s\n", de->d_name);
	    }
            // close directory
	    closedir(dr);
        }
    }
}

// check if a file is a directory
int is_directory(const char *path) 
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

/*
   output file redirection function that takes in a path name string.
   The purpose of the function is to create an outputfile I/O redirection
   (this function handles output only).
Example:
programname arg1 arg2 < inputfile > outputfile
                                  ^ ^^^^^^^^^^
*/
FILE* outputFileRedirection(const char *path)
{
    // int file directory used as a flag if errors occur relating to 
    // the existence of directories/files
    //int fd;

    FILE *fp;
    // freopen to write to stdout
    fp = freopen(path, "w", stdout);

    // if cant open, then error
    if (fp == NULL)
    {
        fprintf(stderr, "Could not write file!!\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}


/*
   inputFileRedirection that will pass in a path name
   of the file that needs to be open using freopen.
   Will throw an error when the FILE pointer points 
   NULL. Then close fp.
  
*/
FILE* inputFileRedirection(const char *path)
{
    FILE *fp; // file pointer that will open the 
              // path being passed in

    
    // assosiates a new path with the given open stream 
    // with a read only permission, passed through stdin
    fp = freopen(path, "r", stdin);

    // throw an error when fp cannot open file
    if (fp == NULL)
    {
        fprintf(stderr, "Can't open input file!\n");
    
        exit(EXIT_FAILURE);
    }
    // close fp
    return fp;

}

/*
   this function will append the path 
   after the file that was previously mentioned
*/
FILE* appendFileRedirection(const char *path)
{
    // file pointer
    FILE *fp;
    // freopen to append to stdout
    fp = freopen(path, "a", stdout);

    // if cant append, error
    if (fp == NULL)
    {
        fprintf(stderr, "Can't append file!\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}

/*
   doFork will fork a process in place of system calls or 
   any other funciton that requires a fork.
*/
void doFork(char *args[])
{
    // pid used to hold the process id when fork is called
    int pid = 0;
    // status is used to updated to a certain process that is resulted
    int status = 0;
    // execIsError is used as a flag to catch if there is an error
    // when using execvp
    int execIsError = 0;
    // arg used to help reset args pointer at the end for each iteration
    char **arg;
    // reset pointer
    arg = args;
    // fp used as a flag 
    FILE *fp;
    // count the arguement present
    int argCounter = 0;
    // switch the pid fork return type
    switch (pid = fork())
    {
        // if fork returns -1, print fork error
        case -1:
            fprintf(stderr, "fork error, fork returned -1\n");
        // if fork returns 0, execvp the first (external) arguement command
        case 0:

            // loop through the arguements that is presented in the args
            while(*args != NULL)
            {
                   
                //printf("we are inside argCounter for loop!\n");
                // TODO: need to complete file redirection for '<', '>', and '>>'
                // when parsing, if there is an output redirection '>' encountered
                // example:
                // argCounter:                     2  3   4
                // command line (after ./projects) ls > out.txt
                //
                if(!strcmp(*args, ">"))
                {
                    // set > to null 
                    *args = NULL;
                    // increment args to point at outputfilet
                    *args++;
                    // pass in args into the outputfileredirection function
                    fp = outputFileRedirection(*args);
                }
                else if(!strcmp(*args, ">>")) // if >> is present in args
                {
                    // set > to null
                    *args = NULL;
                    // then increment to point at the file that needs to be passed in
                    *args++;
    
                    // pass in the file into the append function
                    fp = appendFileRedirection(*args);


                }
                if(!strcmp(*args, "<")) // if < is encountered
                {
                    // set '<' to point to NULL
                    *args = NULL;
                    // increment args to point at args
                    *args++;
                    // pass in args into the input file redirection function
                    fp = inputFileRedirection(*args);
                }
                else // if <, > or >> is not encountered at all
                {
                    // increment the args
                    *args++;
                }
            }
            
            args = arg;
            //*origArgs = *args;
            execIsError = execvp (args[0], args);
            fclose(fp);
            // if the execvp error throws error
            if (execIsError == -1)
            {
                printf("pid is: %d\n", pid);
                fprintf(stderr, "exec error, exec returned 0\n");
                exit(EXIT_FAILURE);
            }
        // else, wait for the process to change state     
        default:
            waitpid(pid, &status, WUNTRACED);
    }
}

