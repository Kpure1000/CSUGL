#ifndef CSUGL_VERTEX_ARRAY_H
#define CSUGL_VERTEX_ARRAY_H
#include "../core/core.h"

namespace csugl {

    struct BufferLayout;
    class VertexBuffer;
    class IndexBuffer;

    class VertexArray {
    public:
        VertexArray();

        ~VertexArray();

        unsigned int GetID() const { return id; }

        void AddVertexBuffer(Ref<VertexBuffer> vb, const BufferLayout &layout);
        std::vector<Ref<VertexBuffer>> GetVertexBuffers() const ;
        std::vector<Ref<VertexBuffer>> &GetVertexBuffers();

        void SetIndexBuffer(Ref<IndexBuffer> ib);
        Ref<IndexBuffer> GetIndexBuffer() const { return index_buffer; }

        void Bind() const;

    private:
        unsigned int id;
        Ref<std::vector<Ref<VertexBuffer>>> vertex_buffers;
        Ref<IndexBuffer> index_buffer;
        size_t attri_count;

    public:
        static Ref<VertexArray> Creat();
    };

}

#endif