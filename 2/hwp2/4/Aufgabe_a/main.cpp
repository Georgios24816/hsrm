#include <iostream>

//this class is an abstract class to represent shapes
//with two coordinates 1)top-left x, y 2) down-right x, y
class Shape
{
//public 
public:
	Shape() {} 	//default Constructor, will be called if no values are provided
				//example: Shape s();

	Shape(int x0, int y0, int x1, int y1) : 
		m_x0(x0), m_y0(y0), m_x1(x1), m_y1(y1) //list initalization of the members
	{

	}

	//getters for the coordinates, const keyword signalizes that none of the 
	//class members will be modified
	int getX0() const { return m_x0; }	//Top Left...
	int getY0() const { return m_y0; } 	//			Corner
	int getX1() const { return m_x1; }	//Down Right...
	int getY1() const { return m_y1; }	//			Corner

	//setters for the coordinates (mutable)
	void setX0(int i) { m_x0 = i; }
	void setY0(int i) { m_y0 = i; }
	void setX1(int i) { m_x1 = i; }
	void setY1(int i) { m_y1 = i; }

	virtual void printName() const 		//A method to print the name of the shape
	{									//it can be reimplemented ("virtual" keyword)
		std::cout << "SHAPE\n";			//because every shape has a different name
	}									//if overriden it shall be marked with keyword "override"
										//at the end of the function definition

//private members are not directly visible and modifiable for the user
//in order to access and modify these values, the user has to use the 
//provided setter and getter functions
private:
	int m_x0 = 0, m_y0 = 0; //all member variables have default values to ensure that they will
	int m_x1 = 1, m_y1 = 1;	//have meaningfull values even when the default constructor is used
};

//a circle is a shape, it has all attribute a shape has all public functions 
//a shape has and a radius for simplicity
class Circle : public Shape
{
public:
	Circle() : Shape() {} //explicitly initalize shape with its default coordinates

	Circle(int x0, int y0, int r)
		: Shape(x0, y0, x0 + 2 * r, y0 + 2 * r),//explicity initalize shape with the circles coordinates
		  m_radius(r)							//x1 and y0 can be calculated from the radius
	{

	}

	int getRadius() const { return m_radius; } 	//getter for radius
	void setRadius(int radius)					//setter for radius
	{
		m_radius = radius;
		Shape::setX1(Shape::getX1() + 2 * m_radius); //if the radius changes so do the shape coordinates x1 and y1
		Shape::setY1(Shape::getY1() + 2 * m_radius);
	}

	void printName() const override //reimplemented virtual function to print the name of the cirle
	{										//override keyword shows that this function is reimplemented from Shape
		std::cout << "CIRCLE\n";
	}

private:
	int m_radius = 1; 
};

//similar to Circle but with width and height instead of radius
class Rectangle : public Shape
{
public:
	Rectangle() : Shape() {}

	Rectangle(int x0, int y0, int width, int height)
		: Shape(x0, y0, x0 + width, y0 + height),
		  m_width(width), m_height(height)
	{

	}

	int getHeight() const { return m_height; }
	int getWidth() const { return m_width; }
	
	void setHeight(int h) { m_height = h; /*unvollständig*/}
	void setWidth(int w) { m_width = w; /*unvollständig*/}


	void printName() const override
	{
		std::cout << "RECTANGLE\n";
	}

private:
	int m_width = 1;
	int m_height = 1;
};

//RectangleCircle inherits from both classes Rectangle and Circle
//it is used to describe a Circle within a Rectangle using 
//x0 and y0 with the rectangle's width and height
class RectangeCircle : public Rectangle, Circle 
{
public:
	RectangeCircle() : Rectangle(), Circle() {}
	
	RectangeCircle(int x0, int y0, int w, int h)//w = rectangle's width; h = rectangle's height
		: Rectangle(x0, y0, w, h), //initialize the first base class
		  Circle(x0, y0, h/2) //initialize the second base class
	{

	}

	virtual void printName() const override //Rectangle and Circle inherit the method printName from Shape
	{										//and because RectangleCircle inherits from those classes, it also
		std::cout << "RECTANGLECIRLE\n";	//has this method.
	}										//this method is marked with "override" because it overrides the printName
};											//method of it's base class

int main()
{
	return 0;
}

/*
Erläuterung:
Die Klasse Shape ist die Basisklasse von Rectangle, Circle und indirekt auch von RectangleCircle.
Shape beschreibt den grundlegenden Aufbau einer mathematischen Figur. Eine Figur wird IMMER durch die in Shape
gespeicherten(zu speichernden) Koordinaten beschrieben, und jede Figur die von Shape erbt hat diese Koordinaten.
Sie kann diese auslesen und verändern. Shape bietet ebenfalls eine virtuelle Methode an(gekennzeichnet durch das
virtual keyword). Diese virtuellen Methoden(Funktionen einer Klasse) können die Klassen die von Shape 
erben reimplementieren(können, MÜSSEN NICHT), also für ihre Klasse spezialisieren(Beispiel im nächsten Absatz). 
Wenn diese Methode (hier printName) reimplementiert wird sollte sie (MUSS NICHT) mit dem Schlüsselwort
"override" gekennzeichnet werden, damit es klar ist, dass es sich um eine reimplementierte Methode handelt.

Die Klasse Rectangle beschreibt die Figur(Shape) Rechteck. Die Figur Rechteck muss die Koordinaten die für die Initialisierung
Shape notwendig sind selber ausrechnen, außer es wird ihr default Konstruktor aufgerufen, dann ruft sie den Default
Konstruktor von Shape auf. Beim Aufruf des Default Konstruktors benutzt die Klasse die member mit ihren Default-Werten.
Bei Rectangle wären m_width = 1 und m_height=1, d.h. wenn die Breite ist 1 und die Höhe ebenfalls, weil der
Default-Konstruktor ohne Werte aufgerufen wird. Die Klasse Rectangle reimplemtiert die Shape::printName Methode um
ihren richtigen Name anzuzeigen "Rectangle" statt "Shape".

Die Klasse Circle beschreibt einen Kreis anhand von der geerbten Klasse Shape. Der Kreis hat statt Höhe und Breite, einen Radius.
Sie hat ebenfalls den Default Konstruktor aktiviert und defaultmäßig den Radius m_radius = 1.
Sie reimplementiert ebenfalls die Methode printName und gibt statt "Shape" "Circle" aus.

Die Klasse RectangleCircle erbt nicht direkt von Shape, sondern von Rectangle UND Circle und somit indirekt auch von Shape.
RectangleCircle beschreibt ein Rechteck mit einem eingebetteten Kreis. Obwohl RectangleCircle indirekt von Shape erbt, kann
es trotzdem die Shape::printName Methode reimplementieren. Wie man sieht hat RectangleCircle keine Member und somit kann es
auch keine Default-Werte haben, es benutzt falls der Default Konstruktor aufgerufen wird die Default Werte von Rectangle 
und Circle

Allgemein:
Jede abgeleitete Klasse (derived class) hat die Attribute/Eigenschaften der Basisklasse (base class)
*/
