# CurlFileFetcher

The CurlFileFetcher class provides a simple interface to download files from the internet using libcurl.
It handles CURL initialization, performs the HTTP GET request to fetch the file from a URL, and writes the content directly to a specified output file.

###### Public Methods
`CurlFileFetcher()`
Constructor that initializes the CURL environment and creates a CURL easy handle.

`~CurlFileFetcher()`
Destructor that cleans up the CURL handle and global CURL environment.

`bool fetchFile(const std::string& url, const std::string& outputFilePath)`
Downloads the file from the specified url and saves it to outputFilePath.
Returns true if the download and file writing succeeded, otherwise false.
Prints error messages to std::cerr if initialization fails, file cannot be opened, or CURL returns an error.