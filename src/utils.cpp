#include "utils.hpp"
#include <json.hpp>
#include <cstdio>

using json = nlohmann::json;

std::string search_replace (std::string& text, const std::string& search, const std::string& replace) {
  return (text.replace(text.find(search), search.length(), replace));
}

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
std::vector<std::string> get_enqueued (json& queue) {
  std::string              cmd_str  (queue.at("stat_cmd").get<std::string>());
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

// TODO: create default qboy.json
void init_or_update_qboy (json& cfg) {
  std::string home        ( std::getenv("HOME"));
  std::string default_file =  home + std::string("/.qboy.json");
}

std::vector<std::string> get_todos (json& queue, const int nb_lefts) {
  std::vector<std::string> results;
  for (auto& job : queue["jobs"]) {
    results.push_back(job);
    if ((nb_lefts > 0) && (results.size() == nb_lefts)) break;
  }
  return results;
}

void enqueue_job (json& queue, const char* job) {
   assert (queue.at("jobs"));
   queue["jobs"].push_back({{"job_name", job}});
}

void submit_job (json& queue, const char* job) {
  std::string submit_template  = queue["submit_cmd"];
  std::string submit_cmd       = search_replace(submit_template, {"{job}"}, job);
  std::string submit_result    = shell_cmd(submit_cmd.c_str());
  assert (submit_result == queue["submit_success"]);
  queue["jobs"].erase(job);
}

/*
   ---
   cron related
   ---
*/
void cron_schedule (json& cfg) {
  std::string queue_name (cfg["full_name"].get<std::string>());
  std::string cmd_template (R"%((crontab -l ; echo "1 * * * * qboy go {queue}") 2>&1 | grep -v "no crontab" | sort | uniq | crontab - )%");
  std::string cmd = search_replace (cmd_template, {"{queue}"}, queue_name);
  shell_cmd (cmd.c_str());
}

void cron_unschedule (json& cfg) {
  std::string queue_name   (cfg["full_name"].get<std::string>());
  std::string cmd_template (R"%((crontab -l ; echo "1 * * * * qboy go {queue}") 2>&1 | grep -v "no crontab" | grep -v qboy |  sort | uniq | crontab - )%");
  std::string cmd = search_replace (cmd_template, {"{queue}"}, queue_name);
  shell_cmd (cmd.c_str());
}

bool is_qboy_scheduled (json& cfg) {
  std::string queue_name   (cfg["full_name"].get<std::string>());
  std::string cmd_template ("crontab -l | grep qboy go {queue}");
  std::string cmd = search_replace (cmd_template, {"{queue}"}, queue_name);
  std::string cron_line = shell_cmd(cmd.c_str());
  return cron_line == "";
}
