/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <vars.h>
#include "Curl.h"
#include "Utils.h"

#define setopt(key, val, message) if (curl_easy_setopt(conn, key, val) != CURLE_OK) { printf("Failed to %s: %s\n", message, errorBuffer); }

Curl::Curl(std::string url, params_type params, bool check_ca) : url(url) {
    if ((conn = curl_easy_init()) == nullptr) {
        printf("Failed to create CURL connection\n");
        exit(EXIT_FAILURE);
    }

//    curl_easy_setopt(conn, CURLOPT_VERBOSE, true);
    setopt(CURLOPT_ERRORBUFFER, errorBuffer, "set error buffer");

    if (!params.empty()) {
        url = append_params(url, params);
    }

    if (check_ca) {
        check_root_ca();
        setopt(CURLOPT_CAINFO, SSL_CA_PATH.c_str(), "set ca info");
    } else {
        setopt(CURLOPT_SSL_VERIFYPEER, 0L, "turn off SSL verification");
    }

    setopt(CURLOPT_URL, url.c_str(), "set URL");
    setopt(CURLOPT_FOLLOWLOCATION, 1L, "set redirect option");
}

std::string Curl::get() {
    std::string reply;

    setopt(CURLOPT_WRITEDATA, &reply, "set data");
    setopt(CURLOPT_WRITEFUNCTION, writeString, "set write function");

    do_request();

    return reply;
}

void Curl::getToFile(std::string file_path) {
    file.open(file_path, std::ifstream::out);

    setopt(CURLOPT_WRITEDATA, &file, "set data");
    setopt(CURLOPT_WRITEFUNCTION, writeFile, "set write function");

    do_request();
}

std::string Curl::getToDir(std::string dir) {
    std::string file_name = url.substr(url.find_last_of('/') + 1);

    if (file_name.empty()) { // Generate if empty
        file_name = "out" + std::to_string(rand() % 1000);
    }

    if (dir.back() != PATH_SEP) // Append
        dir += PATH_SEP;

    file_name = dir + file_name;

    getToFile(file_name);

    return file_name;
}

Curl::~Curl() {
    curl_easy_cleanup(conn);
}

void Curl::check_root_ca() {
    std::fstream file(SSL_CA_PATH);

    if (file.good()) { // File accessible
        return;
    }

    // Download recent version of root certificates
    Curl(SSL_CA_URL, {}, false).getToFile(SSL_CA_PATH);
}

void Curl::do_request() {
    if (curl_easy_perform(conn) != CURLE_OK) {
        printf("Failed to execute: [%s]\n", errorBuffer);
    }
}

std::string Curl::append_params(std::string url, params_type params) {
    if (url.find('?') == -1) {
        url += '?';
    }

    std::vector<std::string> params_vec;
    std::string temp;

    for (const auto &it : params) {
        temp = curl_easy_escape(conn, it.second.c_str(), (int) it.second.size());
        params_vec.push_back(it.first + '=' + temp);
    }

    return url + Utils::join(params_vec, '&');
}
