#include <iostream>
#include <deque>
#include <sstream>
#include <string>

#define CURL_STATICLIB
#include <curl/curl.h>

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::deque<char> *s);
std::string GetFileFromHTTPS(const std::string &url);

int main()
{
    std::cout << GetFileFromHTTPS("https://download.qt.io/archive/qt/INSTALL") << std::endl;
    return 0;
}


size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size,
                                        size_t nmemb, std::deque<char> *s) {
  size_t newLength = size * nmemb;
  size_t oldLength = s->size();
  try {
    s->resize(oldLength + newLength);
  } catch (std::bad_alloc &e) {
    std::stringstream msg;
    msg << "Error allocating memory: " << e.what() << std::endl;
    printf("%s", msg.str().c_str());
    return 0;
  }

  std::copy((char *)contents, (char *)contents + newLength,
            s->begin() + oldLength);
  return size * nmemb;
}

std::string GetFileFromHTTPS(const std::string &url) {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  std::deque<char> s;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // verify ssl peer
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L); // verify ssl hostname
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                     CurlWrite_CallbackFunc_StdString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, true);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK) {
      std::string errorMsg(curl_easy_strerror(res));
      throw std::runtime_error(std::string(errorMsg).c_str());
    } else {
      long http_response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
      if (http_response_code != 200) {
        throw std::runtime_error("Error retrieving data with https protocol, error . " + std::to_string(http_response_code) +
                                 "Probably the URL is invalid.");
      }
    }

    curl_easy_cleanup(curl);
  }
  std::string fileStr(s.begin(), s.end());
  return fileStr;
}
