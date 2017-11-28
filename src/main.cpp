#include <iostream>
#include <fstream>
#include <json.hpp>
#include "commands.hpp"

using json = nlohmann::json;

static json configuration;

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
         return go_command (configuration);
         break;

// Aliases to remove qboy from crontab
       case hash("STOP"):
         return stop_command(configuration);
         break;

// Aliases to list jobs
       case hash("LIST"):
         return ls_command(configuration);
         break;

// Aliases to add commands
       case hash("DO"):
         return do_command(configuration, argv[++i]);
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

//TODO log error message, try catch over configuration read
int read_cfg (const char* filename) {
  std::ifstream cfg (filename, std::ifstream::in);
  if (cfg.fail()) return -1;
  cfg >> configuration;
  return 0;
}

int main (int argc, char** argv) {
  const char* default_config = ".qboy.json";
  if (read_cfg(get_env_or_default("QBOY_CONFIG", default_config)) != 0) {
    return parseArgs (argc, argv);
  } else {
    return -1;
  }
}
