#include <iostream>
#include <fstream>
#include <json.hpp>
#include "commands.hpp"

using json = nlohmann::json;

static json configuration;

//TODO log error message, try catch over configuration read
int read_cfg (const char* filename) {
  std::ifstream cfg (filename, std::ifstream::in);
  if (cfg.fail()) return -1;
  cfg >> configuration;
  return 0;
}

// TODO not yet implemented : GO command should try to launch on remote server
void set_server (char* server_name) {
  configuration["server"] = server_name;
}

int parseArgs (int argc, char** argv) {
   int i = 0;

   while (i++ < argc) {
     switch (hash(argv[i])) {

// Aliases to set qboy on crontab
       case hash("GO"):
         if (argc < (i+1)) {
           help ();
           return -1;
         }
         read_cfg(argv[i+1]);
         return go_command (configuration);
         break;

// Aliases to remove qboy from crontab
       case hash("STOP"):
         if (argc < (i+1)) {
           help ();
           return -1;
         }
         read_cfg(argv[i+1]);
         return stop_command(configuration);
         break;

// Aliases to list jobs
       case hash("LIST"):
        if (argc < (i+1)) {
          help ();
          return -1;
        } else {
        read_cfg(argv[i+1]);
        return ls_command(configuration);
        }
         break;

// Aliases to add commands
       case hash("DO"):
         if (argc < (i+2)) {
           help ();
           return -1;
         }
         read_cfg(argv[i+2]);
         return do_command(configuration, argv[i+1]);
         break;

// Aliases commands to remove a job
       case hash("DONT"):
         if (hash(argv[++i]) != hash("do")) {
            std::cerr << "correct command is: dont do <job>" << std::endl;
         }
         return dont_do_command(configuration, argv[++i]);
         break;

// Aliases to ask a specific machine
       case hash("@"):
          set_server(argv[++i]);
          break;
// Aliases to ask status of qboy (in crontab, last execution report etc...
       case hash("?"):
          status_command(configuration);
          break;
       default:
         help();
         return -1;
     }
   }
  return 0;
}

int main (int argc, char** argv) {
  return parseArgs (argc, argv);
}
