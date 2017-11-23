#pragma once
#include "utils.hpp"
#include <json.hpp>
#include <string>

using json = nlohmann::json;
static int max_jobs_allowed;

void help () {
  std::cout << "qboy [DO <job_script> | DONT DO <job_script> | GO | LIST | STOP | ?]" << std::endl;
  std::cout << "ring qboy and it will lift your jobs to the queue for you" << std::endl;
}


//TODO the do_command should be based on this watch_for_command which is more generic
// Syntax: watch for <condition> in <command>
int watch_for_command (int argc, char** argv) {
  // watch for : idle nodes, job termination etc...
  // the check is added to cron table
  // if they are availables: notify the user: by mail or by notify.

  return 0;
}


// go immediately refresh and schedule qboy if not already in crontab
int go_command (json& cfg, const char* arg = "") {
  if (! is_qboy_scheduled(cfg)) {
    cron_schedule(cfg);
  }
  //TODO: get a lock on directory .qboy
  int nb_left = max_jobs_allowed - get_enqueued(cfg).size();
  for (std::string& f : get_todos (cfg, nb_left)) {
    submit_job (cfg, f.c_str());
  }
  return 0;
}

int stop_command (json& cfg) {
   cron_unschedule(cfg);
   return 0;
}

int ls_command (json& cfg) {
  for (auto& job : get_todos(cfg)) {
    std::cout << job.c_str() << std::endl;
  }
  for (auto& job: get_enqueued(cfg)) {
    std::cout << job.c_str() << std::endl;
  }
  return 0;
}


int status_command (json& cfg) {
  unsigned int minutes = 0;
  if (is_qboy_scheduled(cfg)) {
    std::cout << "qboy is scheduled to check queue in" << minutes << "minutes" << std::endl;
  }
  return 0;
}


// TODO: if possible to queue directly, do it, otherwise write it in .qboy and schedule for next time
int do_command (json& cfg, const char* job) {
  go_command(cfg);
  std::cout << shell_cmd ("ls -l").c_str() << std::endl;
  int nb_left = max_jobs_allowed - get_enqueued (cfg).size();
  //TODO: lock on directory
  if (nb_left > 0) {
    submit_job(cfg, job);
  } else { //TODO: or submit failed
    enqueue_job(cfg, job);
  }
  return 0;
}

int dont_do_command (json& cfg, const char* arg) {
  std::cout << shell_cmd ("ls -l").c_str() << std::endl;
  return 0;
}
