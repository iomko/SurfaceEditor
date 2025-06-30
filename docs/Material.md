# Material
The Material class represents a material in the rendering system by associating it with a Shader and an optional string identifier. It provides a simple interface to set shader uniform color values.

###### Public Methods
`Material(const std::string& id)`
Constructs a Material with the given ID. The shader pointer remains nullptr until assigned.

`Material(Shader* shader)`
Constructs a Material using a pointer to a Shader object.

`void setColorValue(const std::string& name, const glm::vec4& color)`
Sets a color uniform in the shader. Calls the shader's setVec4 method with the uniform name and RGBA color vector.

`Shader* getShader()`
Returns the associated shader pointer.