#pragma once

#include "../../utils/utils.h"

namespace xten { namespace xgraphics { namespace xfont {

	class FontChar;
	class BinPackNode;

	class BinPacker
	{
	private:
		xmaths::vec2i m_Size;
		BinPackNode* m_Root;
	public:
		BinPacker(const xmaths::vec2i & size);
		~BinPacker();
	public:
		bool pack(std::vector<FontChar*> fontCharList);
	};

	class BinPackNode
	{
	private:
		xmaths::vec2i m_Pos;
		xmaths::vec2i m_Size;
		BinPackNode* m_Leaf1;
		BinPackNode* m_Leaf2;
	private:
		bool fits(FontChar *fontChar);
		void createBranches(FontChar *fontChar);
	public:
		BinPackNode();
		BinPackNode(int x, int y, int width, int height);
		~BinPackNode();
	public:
		bool isEmpty() const;
		void set(int x, int y, int width, int height);
		bool add(FontChar* fontChar);
	};


} } }