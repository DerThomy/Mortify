#pragma once

namespace Mortify
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		static VertexBuffer* Create(float* vertices, uint32_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);

		virtual uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}
