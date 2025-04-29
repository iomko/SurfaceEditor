#pragma once
#include <fstream>
#include <iostream>
#include <curl/curl.h>

class CurlFileFetcher
{
public:
	CurlFileFetcher() {
		curl_global_init(CURL_GLOBAL_DEFAULT);
		m_curl = curl_easy_init();
	}

	~CurlFileFetcher() {
		if (m_curl) {
			curl_easy_cleanup(m_curl);
		}
		curl_global_cleanup();
	}

	bool fetchFile(const std::string& url, const std::string& outputFilePath) {
		if (!m_curl) {
			std::cerr << "CURL initialization failed!\n";
			return false;
		}

		std::ofstream outfile(outputFilePath, std::ios::binary);
		if (!outfile.is_open()) {
			std::cerr << "Failed to open output file: " << outputFilePath << "\n";
			return false;
		}

		curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &outfile);

		CURLcode res = curl_easy_perform(m_curl);

		outfile.close();

		if (res != CURLE_OK) {
			std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
			return false;
		}

		return true;
	}


private:
	static size_t writeData(void* ptr, size_t size, size_t nmemb, void* stream) {
		std::ofstream* out = static_cast<std::ofstream*>(stream);
		size_t totalSize = size * nmemb;
		out->write(static_cast<char*>(ptr), totalSize);
		return totalSize;
	}

	CURL* m_curl = nullptr;
};
