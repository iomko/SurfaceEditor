# Buffers

### VertexArrayObject
The VertexArrayObject class manages an OpenGL Vertex Array Object (VAO). It encapsulates creation, binding, and setup of vertex attribute layouts, ensuring proper cleanup of resources.

###### Public Methods
`VertexArrayObject()`
Default constructor, creates an empty VAO wrapper.

`VertexArrayObject(const VertexArrayObject&)`
Copy constructor is deleted to prevent copying.

`VertexArrayObject& operator=(const VertexArrayObject&)`
Copy assignment is deleted.

`VertexArrayObject(VertexArrayObject&& other)`
Move constructor transfers ownership of the VAO ID and invalidates the source.

`VertexArrayObject& operator=(VertexArrayObject&& other)`
Move assignment transfers ownership and cleans up the previous VAO if needed.

`~VertexArrayObject()`
Destructor deletes the VAO resource if owned.

`void bind()`
Binds the VAO for use in rendering.

`void unbind()`
Unbinds any VAO, binding to zero.

`void create()`
Generates a new VAO and binds it.

`void addVertexBufferLayout(int index, int size, GLenum type, bool normalized, int stride, void* offset)`
Specifies the format and enables a vertex attribute pointer for the VAO.

### VertexBufferObject
The VertexBufferObject class manages an OpenGL Vertex Buffer Object (VBO). It supports creation, binding, uploading, and updating of vertex data.

###### Public Methods
`VertexBufferObject()`
Default constructor for an empty VBO wrapper.

`VertexBufferObject(const VertexBufferObject&)`
Copy constructor is deleted.

`VertexBufferObject& operator=(const VertexBufferObject&)`
Copy assignment is deleted.

`VertexBufferObject(VertexBufferObject&& other)`
Move constructor transfers VBO ownership.

`VertexBufferObject& operator=(VertexBufferObject&& other)`
Move assignment transfers ownership and deletes existing buffer if needed.

`~VertexBufferObject()`
Destructor deletes the VBO resource.

`void bind()`
Binds the VBO to GL_ARRAY_BUFFER.

`void unbind()`
Unbinds the VBO by binding zero.

`void create()`
Generates a new VBO and binds it.

`void createData(const void* data, int size, GLenum usage)`
Uploads data to the buffer with the specified usage hint.

`void updateData(const void* data, int size, int offset)`
Updates a subset of the buffer's data starting at the given offset.

### ElementBufferObject
The ElementBufferObject class manages an OpenGL Element Buffer Object (EBO or Index Buffer). It handles creation, binding, and uploading/updating of index data.

###### Public Methods
`ElementBufferObject()`
Default constructor.

`ElementBufferObject(const ElementBufferObject&)`
Copy constructor deleted.

`ElementBufferObject& operator=(const ElementBufferObject&)`
Copy assignment deleted.

`ElementBufferObject(ElementBufferObject&& other)`
Move constructor.

`ElementBufferObject& operator=(ElementBufferObject&& other)`
Move assignment.

`~ElementBufferObject()`
Destructor deletes the EBO.

`void bind()`
Binds the EBO to GL_ELEMENT_ARRAY_BUFFER.

`void unbind()`
Unbinds the EBO.

`void create()`
Generates and binds a new EBO.

`void createData(unsigned int* indices, int count, GLenum usage)`
Uploads index data to the buffer with a usage hint.

`void updateData(unsigned int* indices, int size, int offset)`
Updates a subset of the index buffer at the given offset with new data.