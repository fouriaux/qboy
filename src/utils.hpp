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


std::string               shell_cmd               (const char* cmd);

const char*               get_env_or_default      ( const char* name, 
                                                    const char* default_value);

std::vector<std::string>  get_enqueued            ( json& cfg);

void                      init_or_update_qboy_rep ( json& cfg);

std::vector<std::string>  get_todos               ( json& cfg, 
                                                    const int nb = -1);

void                      enqueue_job             ( json& cfg, 
                                                    const char* job);

void                      submit_job              ( json& cfg, 
                                                    const char* job);

void                      cron_schedule           ( json& cfg);

void                      cron_unschedule         ( json& cfg);

bool                      is_qboy_scheduled       ( json& cfg);




