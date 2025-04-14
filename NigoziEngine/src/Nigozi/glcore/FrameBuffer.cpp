#include "ngpch.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(const FrameBufferSpecification& props)
	:m_spec(props), m_rendererID(0), m_colorAttachment(0), m_depthAttachment(0)
{
	GLCall(glCreateFramebuffers(1, &m_rendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorAttachment));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.Width, m_spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0));

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_depthAttachment));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_spec.Width, m_spec.Height, 0, GL_DEPTH_STENCIL,
		GL_UNSIGNED_INT_24_8, NULL));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0));

	ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Checking framebuffer...");

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_rendererID);
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
