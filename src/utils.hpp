#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <json.hpp>
using json = nlohmann::json;

/*
   ---
   hash util for option parsing
   ---
*/
constexpr unsigned long djb2_hash_impl( const char* text, unsigned long prev_hash ) {
    return text[0] == '\0' ? prev_hash : djb2_hash_impl( &text[1], prev_hash * 33 ^ static_cast<unsigned long>(text[0]) );
}

constexpr unsigned long hash( const char* text ) {
    if (!text) return 0;
    return djb2_hash_impl( text, 5381 );
}

std::string               search_replace          ( std::string& text, const std::string& search, const std::string& replace);

std::string               shell_cmd               ( const char* cmd);
const char*               get_env_or_default      ( const char* name, const char* default_value);

std::vector<std::string>  get_enqueued            ( json& queue);
void                      save_queue              ( json& queue);

std::vector<std::string>  get_todos               ( json& queue, const int nb_lefts = -1);

void                      enqueue_job             ( json& queue, const char* job);
void                      submit_job              ( json& queue, const char* job);

void                      cron_schedule           ( json& queue);
void                      cron_unschedule         ( json& queue);
bool                      is_qboy_scheduled       ( json& queue);
