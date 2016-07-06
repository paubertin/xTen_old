#pragma once

#include "../utils/utils.h"
#include "fonts\font.h"

namespace xten { namespace xgraphics {

	class Renderable2d;

	class Renderer2d
	{
	protected:
		std::vector<xmaths::mat4> m_TransformationStack;
		const xmaths::mat4* m_TransformationBack;
		const xMask* m_Mask;
	protected:
		Renderer2d()
		{
			m_TransformationStack.push_back(xmaths::mat4::identity());
			m_TransformationBack = &m_TransformationStack.back();
		}
	public:
		void push(const xmaths::mat4 & matrix, bool override = false)
		{
			if (override)
				m_TransformationStack.push_back(matrix);
			else
				m_TransformationStack.push_back(m_TransformationStack.back() * matrix);
			m_TransformationBack = &m_TransformationStack.back();
		}

		void pop()
		{
			if (m_TransformationStack.size() > 1)
				m_TransformationStack.pop_back();
			m_TransformationBack = &m_TransformationStack.back();
		}

		virtual void setMask(const xMask* mask) { m_Mask = mask; }

		virtual void begin() {}
		virtual void submit(const Renderable2d * renderable) = 0;
		virtual void drawString(const std::string & text, const xmaths::vec3 & position, const xfont::Font &font, GLuint color) {}
		virtual void end() {}
		virtual void flush() = 0;
	};

} }