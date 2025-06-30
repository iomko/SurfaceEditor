# ThirdApiUtils

### utils::third_api
Namespace providing utility functions for building API URLs for external services, such as the OpenTopography Global DEM API.

###### Methods:
`std::string buildOpenTopoUrl(const OpenTopoParams& input)`
Constructs a URL string to query the OpenTopography Global DEM (NASADEM) API.
The URL is built using the geographic bounding box and API key provided in the OpenTopoParams structure.
Returns a complete and ready-to-use URL for downloading elevation data.
