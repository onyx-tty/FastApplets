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

#ifndef CURL_HANDLER_H
#define CURL_HANDLER_H

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <QString>

using json = nlohmann::json;

class CurlHandler final {
private:
        CURL*       curl;
        std::string response_buffer;

        static size_t writeCallback(char* ptr, size_t element_size, size_t element_quantity,
                                    void* user_data);
        void          initOptions();

public:
        explicit CurlHandler();
        ~CurlHandler();

        void               fetchData(const std::string& target_url);
        const std::string& getResponse() const;
        // Get a specific part of the received response, mostly meant for debugging
        const std::string  getResponse(size_t character, size_t length) const;
        const std::string  popResponse();
        void               setOpt(CURLoption&& option, auto&& value);
};

#endif // CURL_HANDLER_H
