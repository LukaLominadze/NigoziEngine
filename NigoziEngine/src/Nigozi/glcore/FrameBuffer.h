#pragma once
#include "ngpch.h"

struct FrameBufferSpecification {
	uint32_t Width, Height;
};

class FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpecification& props);
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetColorAttachment() const { return m_colorAttachment; }
private:
	uint32_t m_rendererID, m_colorAttachment, m_depthAttachment;
	FrameBufferSpecification m_spec;
};

