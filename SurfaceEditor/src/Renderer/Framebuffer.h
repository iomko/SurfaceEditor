#pragma once
#include "glad/glad.h"

struct FramebufferSettings
{
	int width = 0;
	int height = 0;
};

class Framebuffer
{
public:
	Framebuffer(FramebufferSettings& settings)
	{
		m_settings = settings;
		recreate();
	}

	void recreate()
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		//color texture
		glGenTextures(1, &m_colorTextureId);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_settings.width, m_settings.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureId, 0);

		//depth and stencil texture

		glGenTextures(1, &m_depthStencilTextureId);
		glBindTexture(GL_TEXTURE_2D, m_depthStencilTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_settings.width, m_settings.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthStencilTextureId, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	
	}
	void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
public:
	unsigned int m_id;
	unsigned int m_colorTextureId;
	unsigned int m_depthStencilTextureId;
	FramebufferSettings m_settings;
};