#pragma once
#include "utils.hpp"
#include <json.hpp>
#include <string>

using json = nlohmann::json;

void help () {
  std::cout << "qboy [ DO <job_script> <queue.json>"
            << " | GO <queue.json>| LIST <queue.json>"
            << "| STOP <queue.json>| CREATE | ?]" << std::endl;
  std::cout << "ring qboy and it will lift your jobs to the queue for you" << std::endl;
}

int stop_command (json& queue) {
   cron_unschedule(queue);
   return 0;
}

// go immediately refresh and schedule qboy if not already in crontab
int go_command (json& queue, const char* arg = "") {
  if (! is_qboy_scheduled(queue)) {
    cron_schedule(queue);
  }
  int nb_left = queue["max_allowed"].get<int>() - get_enqueued(queue).size();
  for (std::string& f : get_todos (queue, nb_left)) {
    submit_job (queue, f.c_str());
  }
  if (get_todos(queue).size() == 0) stop_command(queue);
  return 0;
}

int ls_command (json& queue) {
  for (auto& job : get_todos(queue)) {
    std::cout << job.c_str() << std::endl;
  }
  for (auto& job: get_enqueued(queue)) {
    std::cout << job.c_str() << std::endl;
  }
  return 0;
}

// TODO return every queues already scheduled
int status_command (json& cfg) {
  unsigned int minutes = 0;
  if (is_qboy_scheduled(cfg)) {
    std::cout << "qboy is scheduled to check queue in" << minutes << "minutes" << std::endl;
  }
  return 0;
}

// add job obj to a queue and schedule for next time
int do_command (json& queue, const char* job) {
  enqueue_job(queue, job);
  go_command(queue);
  return 0;
}

//TODO: search job in the queue, and delete it. Cancel submited job if already submitted
int dont_do_command(json& queue, const char* ID) {
  return -1;
}
