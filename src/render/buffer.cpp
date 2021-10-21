#include "vertex_array.h"

#include "buffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glog/logging.h>

namespace csugl {

    ///////////////////
    /// Vertex Buffer Layout
    ///////////////////

    size_t SizeOfBufferElementType(BufferElementType type) {
        switch (type) {
        case BufferElementType::Float:  return sizeof(float);
        case BufferElementType::Float2: return 2 * sizeof(float);
        case BufferElementType::Float3: return 3 * sizeof(float);
        case BufferElementType::Float4: return 4 * sizeof(float);
        case BufferElementType::Int:    return sizeof(int);
        case BufferElementType::Int2:   return 2 * sizeof(int);
        case BufferElementType::Int3:   return 3 * sizeof(int);
        case BufferElementType::Int4:   return 4 * sizeof(int);
        case BufferElementType::Bool:   return sizeof(bool);
        case BufferElementType::Mat3:   return 3 * 3 * sizeof(float);
        case BufferElementType::Mat4:   return 4 * 4 * sizeof(float);
        default:
            CHECK(false) << "Unsupported BufferElementType";
            return 0;
        }
    }

    size_t CountOfBufferElementType(BufferElementType type) {
        switch (type) {
        case BufferElementType::Float:  
        case BufferElementType::Int:    
        case BufferElementType::Bool:   return 1;
        case BufferElementType::Float2: 
        case BufferElementType::Int2:   return 2;
        case BufferElementType::Float3: 
        case BufferElementType::Int3:   return 3;
        case BufferElementType::Float4: 
        case BufferElementType::Int4:   return 4;
        case BufferElementType::Mat3:   return 3; // TODO: 3?
        case BufferElementType::Mat4:   return 4; // TODO: 4?
        default:
            CHECK(false) << "Unsupported BufferElementType";
            return 0;
        }
    }

    BufferLayout::BufferLayout(const std::initializer_list<BufferElement> &&ele_list,
                               bool is_cross, size_t size)
        : is_cross(is_cross), size(size), elements(ele_list), stride(0) {
        for (auto &ele : elements) {
            ele.offset = stride;
            stride += ele.size;
        }
    }

    ///////////////////
    /// Vertex Buffer
    ///////////////////

    void VertexBuffer::Gen(const void *data, BufferUsage usage){
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &id); }

    void VertexBuffer::SetLayout(const VertexArray *va, const BufferLayout &layout) {
        // if exsits this va use this vb, cover it
        layouts.insert_or_assign(va->GetID(), layout);
    }

    void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

    void VertexBuffer::set_data(size_t count,const void *data ) {
        CHECK(offset + count <= size) << "Subdata count[" << count << "] out of range[" << size << "]";
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, offset, count, (const void *)data);
        offset += count;
    }
    
    void VertexBuffer::UnBind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    ///////////////////
    /// Index Buffer
    ///////////////////

    void IndexBuffer::Gen(const void *data, BufferUsage usage) {
            glGenBuffers(1, &id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    }

    IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &id); }

    void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

    void IndexBuffer::set_data(size_t count, const void *data) {
        CHECK(offset + count <= size) << "Subdata count[" << count << "] out of range[" << size << "]";
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count, (const void *)data);
        offset += count;
    }

    void IndexBuffer::UnBind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
}
