#include "utils.hpp"
#include <json.hpp>
#include <cstdio>

using json = nlohmann::json;

/*
   ---
   shell command wrapper
   ---
*/

std::string shell_cmd (const char* cmd) {
  FILE* in;
  char buff[100];
  std::string result;
  if (! (in = popen(cmd, "r"))) {
    return result;
  }
  while (std::fgets(buff, sizeof(buff), in) != NULL) {
    result+= buff;
  }
  pclose (in);
  return result;
}

const char* get_env_or_default (const char* name, const char* default_value) {
  char* env_str = nullptr;
  return (env_str = std::getenv(name)) ? env_str : default_value;
}

/*
    scheduler queue calls
*/
std::vector<std::string> get_enqueued (json& cfg) {
  std::string              cmd_str  (cfg.at("stat_cmd").get<std::string>());
  std::istringstream       buffer   (shell_cmd (cmd_str.c_str()));
  std::vector<std::string> results;
  char line [256];
  while (buffer.getline(line, 256)) {
    results.push_back ({line});
  }
  return results;
}

/*
  ---
  Internal queue management
  ---
*/

// TODO: to test
void init_or_update_qboy_rep (json& cfg) {
  std::string home     ( std::getenv("HOME"));
  std::string cmd    =   std::string("mkdir -p ") + home + std::string("/.qboy");
  shell_cmd (cmd.c_str());
}

//TODO next version is reading cfgs to get the clean names
std::vector<std::string> get_todos (json& cfg, const int nb) {
  std::vector<std::string> results;
  std::string home   = std::string(std::getenv("HOME")) + std::string("/.qboy");
  std::string cmd    = std::string("ls -t -1 ") + home;
  std::istringstream buffer (shell_cmd (cmd.c_str()));
  char line [256];
  while (buffer.getline(line, 256)) {
    results.push_back ({line});
  }
  return results;
}

//TODO
void enqueue_job (json& cfg, const char* job) {
  // 1. create unique temp name in .qboy (carefull of multi-proc)
   //std::FILE* tmpf = ; // create your own, as it supposed to be a security issue ?
  // 2. create coresponding cfg file (json format)
  // 3. copy content of job into temp_name
  // 4. serialize json made config into .cfg
        // -- first line as  ORIGINAL_WORKING_DIR
        // -- second line as ORIGINAL_NAME
}

//TODO
void submit_job (json& cfg, const char* job) {
   // 1. read corresponding cfg file (json format)
   // extract working dir
   // cd workingdir
   // cp job name to working dir
   // qsub job => on success rm job, rm .qboy/job
}

/*
   ---
   cron related
   ---
*/

void cron_schedule (json& cfg) {
std::string cmd (R"%((crontab -l ; echo "1 * * * * qboy go") 2>&1 | grep -v "no crontab" | sort | uniq | crontab - )%");
  shell_cmd (cmd.c_str());
}

void cron_unschedule (json& cfg) {
std::string cmd (R"%((crontab -l ; echo "1 * * * * qboy go") 2>&1 | grep -v "no crontab" | grep -v qboy |  sort | uniq | crontab - )%");
  shell_cmd (cmd.c_str());
}

bool is_qboy_scheduled (json& cfg) {
  std::string cmd ("crontab -l | grep qboy");
  std::string cron_line = shell_cmd(cmd.c_str());
  return cron_line == "";
}
