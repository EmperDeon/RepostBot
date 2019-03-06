/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifndef REPOSTBOT_CURL_H
#define REPOSTBOT_CURL_H

#include <string>
#include <fstream>
#include <curl/curl.h>
#include <Storage.h>

/*
 * Curl library helpers
 *
 * Supported:
 *   protocols - HTTP, HTTPS
 *   methods - GET
 */
class Curl {
    CURL *conn = nullptr;
    CURLcode code;
    char errorBuffer[CURL_ERROR_SIZE];
    std::string url, buffer;

    std::ofstream file;

public:
    typedef std::map<std::string, std::string> params_type;

    /*
     * Init connection with URL
     *
     * url: Page url with protocol
     * params: Map of url params
     * check_ca: Ignore "no root certificates" error
     */
    explicit Curl(std::string url, params_type params = {}, bool check_ca = true);

    /*
     * Fetch url and return response
     */
    std::string get();

    /*
     * Fetch url and write response to file
     *
     * file_path: absolute path to file
     */
    void getToFile(std::string file_path);

    /*
     * Fetch url and write response to file
     * File name is determined from url
     *
     * dir: folder for file
     */
    std::string getToDir(std::string dir);

    /*
     * Destroys CURL handle
     */
    ~Curl();

protected:
    /*
     * Checks for ca root file presence, and downloads if not found
     */
    void check_root_ca();

    /*
     * Performs request
     */
    void do_request();

    /*
     * Append params to url
     */
    std::string append_params(std::string url, params_type params);

    /*
     * Writers for get and getToFile functions
     */
    static size_t writeString(char *data, size_t size, size_t nmemb, std::string *reply) {
        reply->append(data, size * nmemb);
        return size * nmemb;
    }

    static size_t writeFile(char *data, size_t size, size_t nmemb, std::ofstream *file) {
        file->write(data, size * nmemb);
        return size * nmemb;
    }
};


#endif //REPOSTBOT_CURL_H
