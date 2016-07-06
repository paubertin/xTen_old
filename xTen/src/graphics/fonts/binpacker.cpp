#include "binpacker.h"
#include "fontchar.h"

namespace xten { namespace xgraphics { namespace xfont {

	/////////////////////////////////////////////////////////
	// BinPacker
	/////////////////////////////////////////////////////////

	BinPacker::BinPacker(const xmaths::vec2i & size):
		m_Size(size)
	{
		m_Root = XNEW BinPackNode();
	}

	BinPacker::~BinPacker()
	{
		XDEL(m_Root);
	}

	bool BinPacker::pack(std::vector<FontChar*> fontCharList)
	{
		m_Root->set(0, 0, m_Size.x, m_Size.y);
		for (int i = 0; i < (int)fontCharList.size(); i++)
		{
			if (!m_Root->add(fontCharList[i]))
			{
				return false;
			}
		}
		return true;
	}

	/////////////////////////////////////////////////////////
	// BinPackNode
	/////////////////////////////////////////////////////////

	BinPackNode::BinPackNode(): m_Leaf1(nullptr), m_Leaf2(nullptr),
		m_Pos(xmaths::vec2i(0,0)), m_Size(xmaths::vec2i(0,0))
	{ }

	BinPackNode::BinPackNode(int x, int y, int width, int height) : m_Leaf1(nullptr), m_Leaf2(nullptr),
		m_Pos(xmaths::vec2i(x, y)), m_Size(xmaths::vec2i(width, height))
	{
	}

	BinPackNode::~BinPackNode()
	{
		XDEL(m_Leaf1);
		XDEL(m_Leaf2);
	}

	bool BinPackNode::isEmpty() const
	{
		return m_Leaf1 == nullptr && m_Leaf2 == nullptr;
	}

	void BinPackNode::set(int x, int y, int width, int height)
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_Size.x = width;
		m_Size.y = height;
		m_Leaf1 = nullptr;
		m_Leaf2 = nullptr;
	}

	bool BinPackNode::add(FontChar * fontChar)
	{
		if (fontChar->isEmpty())
		{
			return true;
		}

		if (isEmpty())
		{
			if (fits(fontChar))
			{
				createBranches(fontChar);
				fontChar->setXY(m_Pos.x + 1, m_Pos.y + 1);
				return true;
			}
			return false;
		}

		if (m_Leaf1->add(fontChar))
		{
			return true;
		}

		return m_Leaf2->add(fontChar);
	}

	bool BinPackNode::fits(FontChar * fontChar)
	{
		return ((fontChar->getWidth() <= m_Size.x) && (fontChar->getHeight() <= m_Size.y));
	}

	void BinPackNode::createBranches(FontChar * fontChar)
	{
		int dx = m_Size.x - fontChar->getWidth();
		int dy = m_Size.y - fontChar->getHeight();

		// we split to give one very small leaf and one very big one
		// because it allows more efficient use of space
		// if you don't do this, the bottom right corner never gets used
		if (dx < dy)
		{
			//	split so the top is cut in half and the rest is one big rect below
			m_Leaf1 = XNEW BinPackNode();
			m_Leaf1->set(m_Pos.x + fontChar->getWidth() + 2, m_Pos.y,
				m_Size.x - (fontChar->getWidth() + 2),
				fontChar->getHeight() + 2);

			m_Leaf2 = XNEW BinPackNode();
			m_Leaf2->set(m_Pos.x, m_Pos.y + fontChar->getHeight() + 2,
				m_Size.x, m_Size.y - (fontChar->getHeight() + 2));
		}
		else
		{
			// left (cut in half)
			m_Leaf1 = XNEW BinPackNode();
			m_Leaf1->set(m_Pos.x, m_Pos.y + fontChar->getHeight() + 2,
				fontChar->getWidth() + 2, m_Size.y - (fontChar->getHeight() + 2));

			// right (not cut)
			m_Leaf2 = XNEW BinPackNode();
			m_Leaf2->set(m_Pos.x + fontChar->getWidth() + 2, m_Pos.y, 
				m_Size.x - (fontChar->getWidth() + 2), m_Size.y);
		}

	}



} } }