#include <map>
#include "Scene/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/Buffers.h"

class BufferSystem {
public:    
    BufferSystem(){

    }
    
    
    
};

class BufferStorage{
public:
    BufferStorage(){

    }

    void run(){
        
    }


};

class MeshBufferStorage : public BufferStorage {
public:
    MeshBufferStorage(){

    }

    //vao
    VertexArrayObject vao;
    //vbo
	std::map<Mesh*, std::map<Material*, VertexBufferObject>> vbos;
};


class Testing {
public:
    void test(){

    }
};
