Project 2

Name: Eric Gonzalez

Email: eric.g.gonzalez-1@ou.edu

Email: 10/23/2018

Description: This program will continue from project 1. As before, we are building upon morph/mimic to handle recursive directories
to either delete, copy, etc. We also need to add rmdirz, mkdirz as well. I also needed to replace all system calls with fork-exec 
patterns. I also need to handle IO redirection of input, output files, and >>. I first created my rmdirz, and mkrirz. rmdirz will only be expected
to delete empty directories. I used a stat struct to see if the arguement given is a directory. I checked with an if statement using S_ISDIR to 
see if the directory exists. If it does then call rmdir to remove it. Else the directory does not exist so stderr. mkdirz will set
arg to args, then same process with stat. If it exists then set each element to the first arguement then count the arguements present.
While the counter is positive and the buffer doesn't encoutner a null, then set that buffer to null and decrement. 
I then tackled fork-exec. I created a doFork() function that is similar to the professors code. Except I added my file redirection
code right before my execvp. When the child creation process is successful, I then start my IO Redirection code. IF the user typed; >, <, >> then i handle them 
respectively with if statements. I set each >, <, >> to null then increment args to point to the file that needs to be passed in. Once that's taken 
care of then I began work on mimic/morph recursion. I created a if stateement rough draft to see how the recursion should work. 

Directions: When program is running, the user must select a command first. They must type the following; esc, wipe, filez, environ, ditto, help,
mimic, erase, morph, chdir, rmdirz, mkdirz, else type a regular terminal command.
For esc, type a file/folder to delete. For wipe, no extra input is 
needed. Filez, the user must either type the command itself to 
run a list the contents. For a target directory, the user must give
a path to desired directory. Typing Environ itself is enough to 
perform its action. Ditto must be followed by a comment. Help itself is 
fine to type. Mimic must have a src and a dst required to copy 
files/directories. Erase must be given a file/directory to be removed. 
Morph must be given a src and a dst to properly work without error 
messages printing. Chdir must be given a path to move into.

Any known bugs or assumptions made: 
Mimic must take in two files and they must exist. 
Morph should have a file as a src
Morph/mimic recursion has some bugs

References used:
- http://man7.org/linux/man-pages/man2/open.2.html
- https://stackoverflow.com/questions/18415904/what-does-mode-t-0644-mean- https://www.die.net/search/?q=freopen&sa=Search&ie=ISO-8859-1&cx=partner-pub-5823754184406795%3A54htp1rtx5u&cof=FORID%3A9&siteurl=linux.die.net%2F&ref=www.die.net%2Fsearch%2F%3Fq%3Dfreopen%26sa%3DSearch%26ie%3DISO-8859-1%26cx%3Dpartner-pub-5823754184406795%253A54htp1rtx5u%26cof%3DFORID%253A9&ss=1109j301209j7
- https://linux.die.net/man/3/freopen
- http://pubs.opengroup.org/onlinepubs/009695399/functions/chdir.html
- http://www.cplusplus.com/reference/cstdio/fopen/
- http://www.cplusplus.com/reference/cstdio/fopen/
- https://www.google.com/search?q=how+to+exit+and+save+in+nano&rlz=1C1CHBF_enUS773US773&oq=how+to+exit+and+save+in+nano&aqs=chrome..69i57j0l3.5069j0j7&sourceid=chrome&ie=UTF-8
- http://pubs.opengroup.org/onlinepubs/009695299/functions/access.html
- https://linux.die.net/man/2/access
- https://stackoverflow.com/questions/15177378/recursive-directory-copying-in-c
- https://stackoverflow.com/questions/35185503/how-to-write-to-a-file-using-open-and-printf
- https://stackoverflow.com/questions/15102992/what-is-the-difference-between-stdin-and-stdin-fileno
Alex Kloppenburg, Job Villamil, Cavan Gary, Andrew Wyatt, Tommy Mai assisted
        - Explained the basic workings of mimic while also  explaining different cases and what should happen
        - Explained basic file functions work, i.e. fopen, fwrite, fclose etc
        - Explained how basic functions that deal with working directory, i.e. getcwd, putenv, etc
        - Explained how morph works, while also explaining different cases and what should happen 
