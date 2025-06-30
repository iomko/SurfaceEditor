# Shader

The Shader class encapsulates loading, compiling, linking, and managing OpenGL shader programs.
It supports vertex, fragment, and optional geometry shaders.
Provides convenient methods to bind/unbind the shader program and set uniform variables of various types.

###### Public Methods
Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath = "")
Constructs and compiles shaders from given file paths, links them into a shader program.
The geometry shader is optional and only compiled/linked if the path is not empty.

`void bind()`
Activates the shader program for use in subsequent rendering calls.

`void unbind()`
Deactivates any shader program (sets current shader to none).

`void setFloat(const std::string& name, float val)`
Sets a float uniform variable by name in the shader program.

`void setMat4(const std::string& name, glm::mat4 val)`
Sets a 4x4 matrix uniform variable by name.

`void setVec4(const std::string& name, const glm::vec4& val)`
Sets a 4-component vector uniform variable by name.

`void setVec3(const std::string& name, const glm::vec3& val)`
Sets a 3-component vector uniform variable by name.

`void setInt(const std::string& name, int val)`
Sets an integer uniform variable by name.