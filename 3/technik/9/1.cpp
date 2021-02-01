#include <string>
#include <vector>

class Component
{
};

class Graphics
{
public:
	virtual drawLine(int x1, int y1, int x2, int y2) = 0;
};

class JFrame : Component
{
public:
	void setTitle(const std::string& s) 
	{ 
		m_title = s; 
	}
	
	void setSize(std::size_t w, std::size_t h)
	{
		m_width = w;
		m_height = h;
	}
	
	void addComponent(Component* c)
	{
		m_components.push_back(c);
	}
	
	void setVisible(bool b)
	{
		m_visibility = b;
	}
	
private:
	std::string m_title;
	std::size_t m_width;
	std::size_t m_height;
	std::vector<Component*> m_components;
	bool m_visibility;
};

class JPanel: Component
{
protected:
	void paintComponent(Graphics* g)
};

class Drawable
{
	void add(Drawable* d)
	{
		m_drawables.push_back(d);
	}
	
	auto getDrawables() const
	{
		return m_drawables;
	}
	
	virtual void draw(Graphics* g) {}
	
private:
	std::vector<Drawable*> m_drawables;
}

class MeinPanel: JPanel
{
public:
	MeinPanel(Drawable* d) {}
	
	void paintComponent(Drawable* d) {}
}

class Line: Drawable
{
	void draw(Graphics* g) {}
}

class Line: Drawable
{
	void draw(Graphics* g) {}
}

class Rectangle: Drawable
{
	void draw(Graphics* g) {}
}