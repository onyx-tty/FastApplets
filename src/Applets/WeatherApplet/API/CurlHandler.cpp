/* Pretty Applets

   Copyright (C) 2025 Łukasz Wrodarczyk

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

CURL* CurlHandler::curl        = nullptr;
bool  CurlHandler::initialized = false;

size_t CurlHandler::writeCallback(char* ptr, size_t chunk_element_size,
                                  size_t chunk_element_quantity, void* user_data) {
        size_t total_chunk_size = chunk_element_size * chunk_element_quantity;

        std::string* data_buffer = static_cast<std::string*>(user_data);
        data_buffer->append(ptr, total_chunk_size);
        qDebug() << "Downloading" << total_chunk_size << "... Total of" << data_buffer->size();
        return total_chunk_size;
}

void CurlHandler::initialize() {
        if (!initialized) {
                // Init libcurl global environment
                curl_global_init(CURL_GLOBAL_DEFAULT);

                // Create a curl handle
                curl = curl_easy_init();
                if (!curl) { qFatal("Failed to initialize curl handle!"); }

                qDebug() << "Curl initialized!";

                // TODO Dotenv appid accessed
                // Choose and set up reponse buffer for data
                setOpt(CURLOPT_WRITEFUNCTION, &writeCallback);
                setOpt(CURLOPT_MAXFILESIZE, 1024 * 1024 * 2); // limit of 2 mb

                // Enable debut output to stdout
                setOpt(CURLOPT_VERBOSE, 1L);

                // Automatically clean up curl at application exit
                QObject::connect(qApp, &QCoreApplication::aboutToQuit, []() { cleanup(); });
                initialized = true; // initialization successfully finalized
        }
}

void CurlHandler::cleanup() {
        if (initialized) {
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                initialized = false;
        }
}

// New method
std::string CurlHandler::download(const std::string& target_url) {
        // Configure our curl if it's not configured yet
        if (!initialized) { initialize(); }

        // Obtained data will continue being appended to our response_buffer
        std::string response_buffer;

        // Tracker for errors
        CURLcode result;

        // Set writing to our response_buffer
        setOpt(CURLOPT_WRITEDATA, &response_buffer);

        // Specify target url
        setOpt(CURLOPT_URL, target_url.c_str());

        // Attempt downloading, success?
        result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
                qCritical() << "Curl failed to fetch data from" << target_url << "!";
                qCritical() << "curl_easy_perform() returned" << curl_easy_strerror(result) << "in"
                            << __func__;
                return std::string();
        }

        // Notify us of the size of downloaded data
        qDebug() << "Response size: " << response_buffer.size();

        // Return released response_buffer data
        return std::move(response_buffer);
}

void CurlHandler::setOpt(CURLoption&& option, auto&& value) {
        if (curl && value) {
                CURLcode result = curl_easy_setopt(curl, option, value);
                if (result != CURLE_OK)
                        qFatal("failure setting curl option at %s in %s",
                               curl_easy_strerror(result), __func__);
        } else if (!curl) {
                qFatal("curl is null in %s", __func__);
        } else if (!value) {
                qFatal("value given to curl is null in %s", __func__);
        }
}
