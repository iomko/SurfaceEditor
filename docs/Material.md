# Material
The Material class represents a material in the rendering system by associating it with a Shader and an optional string identifier.  
######  Methods
`Material(const std::string& id)` - Constructs a Material with the given ID.  
`Material(Shader* shader)` - Constructs a Material using a pointer to a Shader object.  
`void setColorValue(const std::string& name, const glm::vec4& color)` - Sets a color uniform in the shader.  
`Shader* getShader()` - Returns the associated pointer to Shader.  
