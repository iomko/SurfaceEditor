# CurlFileFetcher

The CurlFileFetcher class provides a simple interface to download files from the internet using libcurl.  

### CurlFileFetcher class

###### Methods
`CurlFileFetcher()` - Constructor that initializes the CURL environment and creates a CURL handle.  

`~CurlFileFetcher()` - Destructor that cleans up the CURL handle and global CURL environment.  

`bool fetchFile(const std::string& url, const std::string& outputFilePath)` - Downloads the file from the specified url and saves it to outputFilePath. Returns true if the download and file writing succeeded, otherwise false.  
