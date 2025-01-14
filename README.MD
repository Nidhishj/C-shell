#### GENERAL
This is a C shell (terminal implemented in shell)

The home directory would be the directory where you keep this file and all relative paths, etc have been handled. 

Majority of the input is in format of strings of length 4096 chars.
##### HOW TO RUN 

Type `make` makefile will initialize all the files 
Then type `./a.out` and then the shell would start running.

#### Main4.c
This will be the file having int main() responsible for tokenization 

Note it is having some error handling cases where it print error on invalid format however if someone does `log execute` on a command having wrong input format as it has a differnet tokenization method it will not detect the error but will run in the same manner as a errenoous command would run when a user would input it.

(this is main.c only but I didnt want to change its name :) )


#### DISPLAY
For the display of the command line will display current directory as well.

#### HOP
This will be similar to `cd` function in terminal and handle the cases like terminal handles. Also will be printing the absolute path if the command given is correct.

#### Calling_comm
Made for modularity of code and tokenization purposes and for the calling of different files.(Extra file)

#### REVEAL 
Here one assumption is taken that  the directory will contain at most 1024 files. This has been done for storage purpose.

#### PROCLORE 
Virtual memory in Bytes 
As per answer for Q3(till aug28 4pm) we can print any path so printing absolute path
Error is reported for those processes which do not allow to read path without using `sudo` (i.e only root can access those)

#### LOG
Will store the recent most 15 commands will store the whole string given as input regardless of whether the command entered is correct or not.

Also upon log execute the command which is being executed would `not` be stored in the history. 

It implements its own tokenization process which differs from the main tokenization process.

#### SYSTEM COMMANDS 
At max `4096` background process can be stored (means create only 4096 bg process). However after completion one can create more processes. 
Also for background commands the command length i.e -  `sleep`,`echo` they are restricted to be of 256 char.
`<256>char other args` would work or else not entire reading would be there 
Also if a foreground process runs for more than 2 seconds then the entire command given including all `;` `&` would be printed.
For a wrong background process the pid generated is printed and then exited abnormally is printed (similar to terminal where pid is printed and exit 127 is printed.)

Note - The foreground process is spawned with `setpgid()` command so that signal could be handled properly. Because of this those processes which need the entire control of terminal do not get the control and then cannot be worked however on closing shell they give signal of SIGHUP (means the process ran but couldnt get control).

Note those processes can be hardcoded but that does defeat the purpose of the shell.

#### seek
This will find all the files/folders specify the flag which have the name similar to what is given in input.
If nothing is found then nothing would be printed.

#### Aliases - 
Aliases has been implemented in the `myshrc.sh` file.Their reading is done in main at the start of the shell.

#### Ping - 
After sending the signal SIGNAL sent to the process `pid` would be printed. 

#### activities - 
This will print the pid - process name and status of all the background processes created by the shell which have not been terminated.

#### IO redirection - 

This has been implemented in i_o_redirect.c file along with the pipe functioning. 
If a background process is being executed it will not execute to the whole pipe.
Also the process pid would be printed but the status would not be printed on completion as the bg_process array gets updated in child however signal comes to parent so it could not be checked upon whether the process was there or not.

#### fg_and_bg
For continuing a stopped background process (bg)
For fg it brings a background process to foreground and continues it.

#### Signals- 
For Signals there is not seprate file being made just implemented the function in main. 
`IMP - for closing the shell while taking input press Ctrl+D no exit command has been made for this.`

#### Neonate - 

Syntax- `neonate -n <time>`

So here we are printing the most recent pids being generated after a time interval of `time_arg` seconds. This continues until key `x` is not being pressed.

#### iMan 

Syntax - `iMan <command>`

This will be parsing a html page of a system command from the website - `http://man.he.net/`. 

Note only the first commands html page would be displayed rest would be ignored.

Prompts file - prompts.odt
