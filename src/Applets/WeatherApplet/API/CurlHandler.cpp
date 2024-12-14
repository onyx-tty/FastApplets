/* Pretty Applets

   Copyright (C) 2024 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "CurlHandler.h"

#include <QApplication>
#include <QDebug>

size_t CurlHandler::writeCallback(char* ptr, size_t chunk_element_size,
                                  size_t chunk_element_quantity, void* user_data) {
        size_t total_chunk_size = chunk_element_size * chunk_element_quantity;

        std::string* data_buffer = static_cast<std::string*>(user_data);
        data_buffer->append(ptr, total_chunk_size);
        qInfo() << "Downloading" << total_chunk_size << "... Total of" << data_buffer->size();
        return total_chunk_size;
}

void CurlHandler::initOptions() {
        curl_global_init(CURL_GLOBAL_DEFAULT);

        // TODO Dotenv appid accessed
        setOpt(CURLOPT_WRITEDATA, &response_buffer);
        setOpt(CURLOPT_WRITEFUNCTION, &writeCallback);
        setOpt(CURLOPT_MAXFILESIZE, 1024 * 1024 * 2); // limit of 5 mb

        // debug
        setOpt(CURLOPT_VERBOSE, 1L);
}


CurlHandler::CurlHandler() : curl(curl_easy_init()) {
        initOptions();
}

CurlHandler::~CurlHandler() {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
}

const std::string& CurlHandler::fetchData(const std::string& target_url) {
        CURLcode result;
        setOpt(CURLOPT_URL, target_url.c_str());

        // setOpt has already checked for the validity of curl
        result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() returned %s\n", curl_easy_strerror(result));
        }

        qInfo() << "Response size: " << response_buffer.size();
        return response_buffer;
}

const std::string& CurlHandler::getResponseAddress() const {
        return response_buffer;
}

const std::string CurlHandler::popResponse() {
        return std::move(response_buffer);
}

void CurlHandler::setOpt(CURLoption&& option, auto&& value) {
        if (curl && value) {
                CURLcode result;
                result = curl_easy_setopt(curl, option, value);
                if (result != CURLE_OK) {
                        fprintf(stderr, "failure at: %s\n", curl_easy_strerror(result));
                }
        } else {
                if (!curl) {
                        qFatal("curl is null!");
                } else if (!value) {
                        qFatal("value is null");
                }
                QApplication::quit();
        }
};
