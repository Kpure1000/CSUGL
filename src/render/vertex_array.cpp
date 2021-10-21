#include "buffer.h"

#include "vertex_array.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glog/logging.h>

namespace csugl {

    unsigned int GetGlTypeOfBufferElement(BufferElementType type) {
        switch (type) {
        case BufferElementType::Float:  
        case BufferElementType::Float2: 
        case BufferElementType::Float3: 
        case BufferElementType::Float4: 
        case BufferElementType::Mat3:   
        case BufferElementType::Mat4:   return GL_FLOAT;
        case BufferElementType::Int:    
        case BufferElementType::Int2:   
        case BufferElementType::Int3:   
        case BufferElementType::Int4:   return GL_INT;
        case BufferElementType::Bool:   return GL_BOOL;
        default:
            CHECK(false) << "Unsupported BufferElementType";
            return 0;
        }
    }

    VertexArray::VertexArray() 
        : vertex_buffers(MakeRef<std::vector<Ref<VertexBuffer>>>()), attri_count(0) {
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &id);
    }

    void VertexArray::AddVertexBuffer(Ref<VertexBuffer> vb, const BufferLayout &layout) {
        Bind();
        vb->Bind();
        vb->SetLayout(this, layout);
        size_t offset = 0;
        for(const auto& ele : layout.elements) {
            size_t lay_count = 1;
            if (ele.type == BufferElementType::Mat3 ||
                ele.type == BufferElementType::Mat4) {
                lay_count = ele.count;
            }
            for (size_t i = 0; i < lay_count; i++) {
                glEnableVertexAttribArray(static_cast<GLuint>(attri_count)); // 草！
                glVertexAttribPointer(
                    attri_count,
                    ele.count,
                    GetGlTypeOfBufferElement(ele.type),
                    ele.is_normalized,
                    layout.is_cross ? layout.stride : 0,
                    layout.is_cross
                        ? (const void *)ele.offset
                        : (const void *)(offset));
                offset += ele.size * layout.size;
                attri_count++;
            }
        }
        vertex_buffers->push_back(vb); // TODO: why need storage vb
    }

    std::vector<Ref<VertexBuffer>> VertexArray::GetVertexBuffers() const {
        return *vertex_buffers;
    }

    std::vector<Ref<VertexBuffer>> &VertexArray::GetVertexBuffers() {
        return *vertex_buffers;
    }

    void VertexArray::SetIndexBuffer(Ref<IndexBuffer> ib) {
        index_buffer = ib;
    }

    void VertexArray::Bind() const {
        glBindVertexArray(id);
    }

    Ref<VertexArray> VertexArray::Creat() {
        return MakeRef<VertexArray>();
    }

}