#pragma once
#include "ngpch.h"

namespace Nigozi
{
	struct FameBufferProps {
		uint32_t Width, Height;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FameBufferProps& props);
		~FrameBuffer();

		void Resize(uint32_t width, uint32_t height);
		void Invalidate();

		void Bind() const;
		void Unbind() const;

		inline const uint32_t GetColorAttachment() const { return m_colorAttachment; }
		inline const uint32_t GetWidth() const { return m_props.Width; }
		inline const uint32_t GetHeight() const { return m_props.Height; }

		inline const FameBufferProps& GetProps() const { return m_props; }
	private:
		uint32_t m_rendererID, m_colorAttachment, m_depthAttachment;
		FameBufferProps m_props;
	};
}

