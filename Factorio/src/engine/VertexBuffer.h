#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, bool dynamic);
	~VertexBuffer();

	void Update(unsigned int size, const void* data);

	void Bind() const;
	void Unbind() const;
};

