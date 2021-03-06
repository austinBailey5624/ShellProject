/**
 * @file execute.c
 *
 * @brief Implements interface functions between Quash and the environment and
 * functions that interpret an execute commands.
 *
 * @note As you add things to this file you may want to change the method signature
 */

#include "execute.h"
#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>//for cd to work
#include <signal.h>//for cd to work
#include "quash.h"
#include <dirent.h>

// Remove this and all expansion calls to it
/**
 * @brief Note calls to any function that requires implementation
 */
#define IMPLEMENT_ME()                                                  \
  fprintf(stderr, "IMPLEMENT ME: %s(line %d): %s()\n", __FILE__, __LINE__, __FUNCTION__)

/***************************************************************************
 * Interface Functions
 ***************************************************************************/

// Return a string containing the current working directory.
char* get_current_directory(bool* should_free) {
  //Get the current working directory. This will fix the prompt path.
  // HINT: This should be pretty simple
  *should_free = false;

  char buf[1024];
  if (getcwd(buf, 1024) != NULL)
  {
    char* buf2=buf;
    return buf2;
  }
  else
  {
    return "Didnt work";
  }
}

// Returns the value of an environment variable env_var
const char* lookup_env(const char* env_var) {
  //This is required for parser to be able
  // to interpret variables from the command line and display the prompt
  // correctly
  // HINT: This should be pretty simple
  //IMPLEMENT_ME();
  return getenv(env_var);
}

// Sets the environment variable env_var to the value val
void write_env(const char* env_var, const char* val) {
  // HINT: This should be pretty simple
  setenv(env_var, val, 1);
}

// Check the status of background jobs
void check_jobs_bg_status() {
  // TODO: Check on the statuses of all of the background jobs

  //IMPLEMENT_ME();

// if(length_Example()==0)
// {
//       printf("There are no Background jobs to print");
// }
// else
// {
//       for(int i=0;i<length_Example();i++)
//       {
//             print_job(i,i,)
//       }
// }
  //IMPLEMENT_ME();

  // TODO: Once jobs are implemented, uncomment and fill the following line
  // print_job_bg_complete(job_id, pid, cmd);
}

// Prints the job id number, the process id of the first process belonging to
// the Job, and the command string associated with this job
void print_job(int job_id, pid_t pid, const char* cmd) {
  printf("[%d]\t%8d\t%s\n", job_id, pid, cmd);
  fflush(stdout);
}

// Prints a start up message for background processes
void print_job_bg_start(int job_id, pid_t pid, const char* cmd) {
  printf("Background job started: ");
  print_job(job_id, pid, cmd);
}

// Prints a completion message followed by the print job
void print_job_bg_complete(int job_id, pid_t pid, const char* cmd) {
  printf("Completed: \t");
  print_job(job_id, pid, cmd);
}

/***************************************************************************
 * Functions to process commands
 ***************************************************************************/
// Run a program reachable by the path environment variable, relative path, or
// absolute path
void run_generic(GenericCommand cmd) {
  // Print an array of strings. The `str` array is a NULL terminated (last
  // character pointer is always NULL) list of strings. The first element in the
  // array is the executable
  char** str = cmd.args;
  execvp(str[0], str);

}

// Print strings
void run_echo(EchoCommand cmd) {
  // Print an array of strings. The args array is a NULL terminated (last
  // character pointer is always NULL) list of strings.
  char** str = cmd.args;
  for (int i=0; str[i] != NULL; i++)
  {
    printf(str[i]);
    printf(" ");
  }
    printf("\n");
}

// Sets an environment variable
void run_export(ExportCommand cmd) {
  // Write an environment variable
  const char* env_var = cmd.env_var;
  const char* val = cmd.val;
  // HINT: This should be quite simple.
  setenv(env_var, val, 1);
  //IMPLEMENT_ME();
}

// Changes the current working directory
void run_cd(CDCommand cmd) {
  // Change directory
  // Update PWD and optionally update OLD_PWD

  char currentDirectory[1024];
  getcwd(currentDirectory,sizeof(currentDirectory));
  write_env("OLD_PWD",currentDirectory);

  char resolved_path[4096];
  realpath(cmd.dir, resolved_path);
  chdir(resolved_path);
  write_env("PWD",resolved_path);

  // chdir(cmd.dir);
  // char buf[1024];
  // getcwd(buf, sizeof(buf));
  // write_env("PWD", buf);
}

// Sends a signal to all processes contained in a job
void run_kill(KillCommand cmd) {
  int signal = cmd.sig;
  int job_id = cmd.job;

  // TODO: Remove warning silencers
  (void) signal; // Silence unused variable warning
  (void) job_id; // Silence unused variable warning

  // TODO: Kill a background job
  IMPLEMENT_ME();
}


// Prints the current working directory to stdout
void run_pwd() {
  //Print the current working directory
//  IMPLEMENT_ME();
  char dir_name[1024];
  if ( getcwd(dir_name, sizeof(dir_name)) !=NULL)
  {
    puts(dir_name);
  }
  else
  {
    perror("getcwd() error");
  }
  fflush(stdout);
}

// Prints all background jobs currently in the job list to stdout
void run_jobs() {
  // TODO: Print background jobs
  IMPLEMENT_ME();
}

/***************************************************************************
 * Functions for command resolution and process setup
 ***************************************************************************/

/**
 * @brief An example of how to resolve a Command type from a @a Command union.
 *
 * You will need some variant(s) of this switch structure to direct the command
 * statements to their correct caller. Probably you won't want all commands
 * accessible from a single function like this.
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void example_run_parent_command(Command cmd) {
  CommandType type = get_command_type(cmd);

  switch (type) {
  case GENERIC:
    run_generic(cmd.generic);
    break;

  case ECHO:
    run_echo(cmd.echo);
    break;

  case EXPORT:
    run_export(cmd.export);
    break;

  case CD:
    run_cd(cmd.cd);
    break;

  case KILL:
    run_kill(cmd.kill);
    break;

  case PWD:
    run_pwd();
    break;

  case JOBS:
    run_jobs();
    break;

  case EXIT:
  case EOC:
    break;

  default:
    fprintf(stderr, "Unknown command type: %d\n", type);
  }
}

void example_run_child_command(Command cmd) {
  CommandType type = get_command_type(cmd);

  switch (type) {
  case GENERIC:
    run_generic(cmd.generic);
    break;

  case ECHO:
    run_echo(cmd.echo);
    break;

  case EXPORT:
    run_export(cmd.export);
    break;

  case CD:
    run_cd(cmd.cd);
    break;

  case KILL:
    run_kill(cmd.kill);
    break;

  case PWD:
    run_pwd();
    break;

  case JOBS:
    run_jobs();
    break;

  case EXIT:
  case EOC:
    break;

  default:
    fprintf(stderr, "Unknown command type: %d\n", type);
  }
}
/**
 * @brief Create a process centered around the @a Command in the @a
 * CommandHolder setting up redirects and pipes where needed
 *
 * @note Not all commands should be run in the child process. A few need to
 * change the quash process in some way
 *
 * @param holder The CommandHolder to try to run
 *
 * @sa Command CommandHolder
 */
void create_process(CommandHolder holder) {
  // Read the flags field from the parser
  bool p_in  = holder.flags & PIPE_IN;
  bool p_out = holder.flags & PIPE_OUT;
  bool r_in  = holder.flags & REDIRECT_IN;
  bool r_out = holder.flags & REDIRECT_OUT;
  bool r_app = holder.flags & REDIRECT_APPEND;

  // TODO: Remove warning silencers
  (void) p_in;  // Silence unused variable warning
  (void) p_out; // Silence unused variable warning
  (void) r_in;  // Silence unused variable warning
  (void) r_out; // Silence unused variable warning
  (void) r_app; // Silence unused variable warning

  // TODO: Setup pipes and redirects
  int fd[2];
//  pid_t pid;

  if (pipe(fd) == -1)
  {
    perror("Error creating pipe -> execute.c:294");
  }

  //TODO: Check if command should be run as a child or parent process
        example_run_parent_command(holder.cmd);
  //pid=fork();
  //if(pid==0)
  {
      //  push_front_Example(pid);
        //dup2(fd[0],p_in);
        //close(fd[1]);
      //  killCommand(pid);
  }

//  IMPLEMENT_ME();
}

// Run a list of commands
void run_script(CommandHolder* holders) {
  if (holders == NULL)
    return;

  check_jobs_bg_status();

  if (get_command_holder_type(holders[0]) == EXIT &&
      get_command_holder_type(holders[1]) == EOC) {
    end_main_loop();
    return;
  }

  CommandType type;

  // Run all commands in the `holder` array
  for (int i = 0; (type = get_command_holder_type(holders[i])) != EOC; ++i)
    create_process(holders[i]);

  if (!(holders[0].flags & BACKGROUND)) {
    // Not a background Job
    // TODO: Wait for all processes under the job to complete
 //   IMPLEMENT_ME();
  }
  else {
    // A background job.
    // TODO: Push the new job to the job queue
  //  IMPLEMENT_ME();

    // TODO: Once jobs are implemented, uncomment and fill the following line
    // print_job_bg_start(job_id, pid, cmd);
  }
}
