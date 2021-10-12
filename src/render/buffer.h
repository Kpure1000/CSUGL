#ifndef CSUGL_BUFFER_H
#define CSUGL_BUFFER_H
#include "../core/core.h"

#include <vector>
#include <unordered_map>

namespace csugl {

    enum class BufferElementType {
        Float, Float2, Float3, Float4,
        Bool,
        Int, Int2, Int3, Int4,
        Mat3, Mat4
    };

    size_t SizeOfBufferElementType(BufferElementType type);
    size_t CountOfBufferElementType(BufferElementType type);

    struct BufferElement {
        BufferElement(BufferElementType type, bool normalized = false)
            : type(type), size(SizeOfBufferElementType(type)),
              count(CountOfBufferElementType(type)),
              offset(0), is_normalized(normalized){
        }

        BufferElementType type;
        size_t size;
        size_t count;
        size_t offset;
        bool is_normalized;
    };

    struct BufferLayout {
        BufferLayout(const std::initializer_list<BufferElement> &&ele_list);
        std::vector<BufferElement> elements;
        size_t stride;
    };

    // Example: STATIC | DRAW
    enum BufferUsage {
        STREAM_DRAW     = 0x88E0,
        STREAM_READ     = 0x88E1,
        STREAM_COPY     = 0x88E2,
        STATIC_DRAW     = 0x88E4,
        STATIC_READ     = 0x88E5,
        STATIC_COPY     = 0x88E6,
        DYNAMIC_DRAW    = 0x88E8,
        DYNAMIC_READ    = 0x88E9,
        DYNAMIC_COPY    = 0x88EA,
    };

    class Buffer {
    public:
        Buffer(size_t size) : id(0), size(size), offset(0) {}
        virtual ~Buffer() = default;

        virtual void Bind() const = 0;

    protected:
        unsigned int id;
        size_t size;
        size_t offset;
    };

    class VertexArray;

    class VertexBuffer : public Buffer {
    public:
        // VertexBuffer
        // size: size of data elements, for examle size of VB[0.0, 0.0, 0.0] is 3
        template <typename T>
        VertexBuffer(size_t size, const T *data = nullptr,
                     BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(size * sizeof(T)) {
            Gen(data, usage);
        }

        ~VertexBuffer() override;

        void SetLayout(const VertexArray *va, const BufferLayout &layout);

        void Bind() const override;

        template <typename T>
        void SetData(size_t count, const T *data){
            set_data(count * sizeof(T), data);
        }

        inline void Reset() { offset = 0; }

    private:
        std::unordered_map<unsigned int, BufferLayout> layouts;

        void Gen(const void *data, BufferUsage usage);
        void set_data(size_t count, const void* data);

    public:
        template<typename T>
        inline static Ref<VertexBuffer> Create(size_t size, 
            const T *data = nullptr, BufferUsage usage = BufferUsage::STATIC_DRAW) {
                return MakeRef<VertexBuffer>(size, data, usage);
            }

        static void UnBind();
    };

    class IndexBuffer : public Buffer {
    public:
        template <typename T>
        IndexBuffer(size_t size, const T *data = nullptr,
                    BufferUsage usage = BufferUsage::STATIC_DRAW)
            : Buffer(size * sizeof(T)) {
            Gen(data, usage);
        }

        ~IndexBuffer() override;

        void Bind() const override;

        template <typename T>
        void SetData(size_t size, const T *data) {
            set_data(size * sizeof(T), data);
        }

        inline void Reset() { offset = 0; }

    private:
        void Gen(const void *data, BufferUsage usage);
        void set_data(size_t count, const void *data);

    public:
        template <typename T>
        inline static Ref<IndexBuffer> Create(size_t size,
            const T *data = nullptr, BufferUsage usage = BufferUsage::STATIC_DRAW) {
            return MakeRef<IndexBuffer>(size, data, usage);
        }

        static void UnBind();
    };

    class FrameBuffer {
    };

}

#endif