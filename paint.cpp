
#include "gp142.h"
#include <cmath>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;
#define FALSE 0
#define TRUE  1


int fillCol = 0;

struct Point {
	double x, y;
};

class Shape {
protected:
	Point* points;
	int colour;
	int numberOfPoints;
public:
	Shape(Point* p, int c, const int n)
	{
		points = new Point[n];
		numberOfPoints = n;
		colour = c;
		for (int i = 0; i < n; i++) {
			points[i].x = p[i].x;
			points[i].y = p[i].y;
		}
	}
	virtual void draw() = 0;
	virtual bool contains(Point p) = 0;

	virtual void print(ofstream& out) = 0;
	~Shape()
	{
		delete[] points;
	}
};

class polygon : public Shape {
protected:
	int fillColour;
public:
	polygon(Point* p, int c, int n, int fc = 1)
		:Shape(p, c, n), fillColour(fc)
	{};
	void draw() {
		for (int i = 0; i < numberOfPoints - 1; i++) {
			GP142_lineXY(
				colour,
				points[i].x, points[i].y,
				points[i + 1].x, points[i + 1].y,
				2);
		}

		GP142_lineXY(colour, points[0].x, points[0].y, points[numberOfPoints - 1].x, points[numberOfPoints - 1].y, 2);
		if (fillColour != 1) {
			contains(points[numberOfPoints]);
		}

	}
	virtual bool contains(Point p)
	{
		int i, j = numberOfPoints - 1;
		bool contain = false;

		for (i = 0; i < numberOfPoints; i++) {
			if (points[i].y < p.y && points[j].y >= p.y
				|| points[j].y < p.y && points[i].y >= p.y) {
				if (points[i].x + (p.y - points[i].y) / (points[j].y - points[i].y)*(points[j].x
					- points[i].x) < p.x) {
					contain = !contain;
				}
			}
			j = i;
		}
		if (contain) {
			Point *temp = new Point[numberOfPoints + 1];
			for (int i = 0; i < numberOfPoints; i++)
				temp[i] = points[i];
			temp[numberOfPoints] = p;
			delete[]points;
			points = temp;
		}
		if (contain) {
			fill(p.x, p.y, fillCol);
		}

		return contain;
	}
	bool contains(double x, double y) {
		int i, j = numberOfPoints - 1;
		bool contain = false;

		for (i = 0; i < numberOfPoints; i++) {
			if (points[i].y < y && points[j].y >= y
				|| points[j].y < y && points[i].y >= y) {
				if (points[i].x + (y - points[i].y) / (points[j].y - points[i].y)*(points[j].x
					- points[i].x) < x) {
					contain = !contain;
				}
			}
			j = i;
		}
		return contain;
	}
	virtual void fill(int color = 1)
	{
		fillColour = color;
	}
	void fill(double x, double y, int c)
	{
		int originalX = x;
		int originalY = y;
		fillColour = c;
		for (int j = originalY; contains(x, j); j++) {
			while (contains(x++, j)) {
				GP142_pixelXY(c, x, j);
			}
			x = originalX;
		}


		x = originalX;
		y = originalY;
		for (int j = y; contains(x, j); j++) {
			while (contains(x--, j)) {
				GP142_pixelXY(c, x, j);
			}
			x = originalX;
		}

		x = originalX;
		y = originalY;
		for (int j = y; contains(x, j); j--) {
			while (contains(x++, j)) {

				GP142_pixelXY(c, x, j);
			}
			x = originalX;
		}


		x = originalX;
		y = originalY;
		for (int j = y; contains(x, j); j--) {
			while (contains(x--, j)) {

				GP142_pixelXY(c, x, j);
			}
			x = originalX;

		}
		x = originalX;
		y = originalY;
		for (int i = y; contains(x, i); i++) {
			GP142_pixelXY(c, x, i);
		}
		x = originalX;
		y = originalY;
		for (int i = y; contains(x, i); i--) {
			GP142_pixelXY(c, x, i);
		}
	}
	
	virtual void print(ofstream& out) {
		out << "Polygon ";
		out << numberOfPoints << " ";
		for (int i = 0; i < numberOfPoints; i++)
			out << points[i].x <<" "<< points[i].y << " ";
		out << colour << " "<< fillColour << endl;
	}

};

class rectangle :public polygon {
public:
	rectangle(Point* p, int c, int n, int fc)
		:polygon(p, c, n, fc)
	{};
	bool contains(Point p) {
		bool contain = false;

		if ((p.y >= points[0].y && p.y <= points[1].y && p.x <= points[0].x && p.x >= points[1].x) ||
			(p.y >= points[1].y && p.y <= points[0].y && p.x <= points[1].x && p.x >= points[0].x) ||
			p.y>=points[0].y && p.y<=points[1].y && p.x>=points[0].x && p.x<=points[1].x ||
			p.y>=points[1].y && p.y <= points[0].y && p.x>=points[1].x &&p.x<=points[0].x)
			contain = true;
		
		if (p.x >= -500 && p.x <= -465);
		else {
			if (contain)
				fill(fillCol);
		}

		return contain;
	}
	void draw()
	{
		GP142_lineXY(colour, points[0].x, points[0].y, points[0].x, points[1].y, 2);
		GP142_lineXY(colour, points[0].x, points[0].y, points[1].x, points[0].y, 2);
		GP142_lineXY(colour, points[1].x, points[0].y, points[1].x, points[1].y, 2);
		GP142_lineXY(colour, points[1].x, points[1].y, points[0].x, points[1].y, 2);
		if (fillColour != 1)
			fill(fillColour);
	}
	void fill(int color = 1)
	{
		int x1, x2, y1, y2;
		if (points[0].x < points[1].x) {
			x1 = points[0].x;
			x2 = points[1].x;
		}
		else {
			x1 = points[1].x;
			x2 = points[0].x;
		}
		if (points[0].y > points[1].y) {
			y1 = points[0].y;
			y2 = points[1].y;
		}
		else {
			y1 = points[1].y;
			y2 = points[0].y;
		}

		int y = y1-2;
		while (y > y2) {
			GP142_lineXY(color, x1, y, x2-1, y, 2);
			y -= 2;
		}

		fillColour = color;
	}
	void print(ofstream& out) {
		out << "Rectangle ";
		out << numberOfPoints << " ";
		for (int i = 0; i < numberOfPoints; i++)
			out << points[i].x << " " << points[i].y << " ";
		out << colour << " " << fillColour << endl;
	}
};

class OpenShapes : public Shape {
protected:
	bool style;
public:
	OpenShapes(Point* p, int c, int n, int s)
		:Shape(p, c, n), style(s)
	{};
	virtual void changeColour(int c) = 0;
};

bool pointLiesOnLine(Point a, Point b, Point c)
{
	bool flag = false;

	double m;
	double dy = b.y - a.y;
	double dx = b.x - a.x;
	m = dy / dx;
	double yIntercept;
	yIntercept = b.y - m*b.x;

	if ( abs( c.y - (m*c.x + yIntercept)) <= 2)
		flag = true;

	return flag;

}

class line : public OpenShapes {
public:
	line(Point* p, int c, int n, int s)
		:OpenShapes(p, c, n, s)
	{};
	void draw() {

			GP142_lineXY(							 /* Draw a line segment on the graphics window. */
				colour,									   /* color */
				points[0].x, points[0].y,               /* x, y coordinates of starting point */
				points[1].x, points[1].y,           /* x, y coordinates of ending point */
				2);
		
	}
	bool contains(Point p)
	{
		bool contain = false;

		if (pointLiesOnLine(points[0], points[1], p))
			contain = true;

		if (contain == true)
			changeColour(fillCol);

		return contain;
	}
	void changeColour(int c)
	{
		GP142_lineXY(							 /* Draw a line segment on the graphics window. */
			c,									   /* color */
			points[0].x, points[0].y,               /* x, y coordinates of starting point */
			points[1].x, points[1].y,           /* x, y coordinates of ending point */
			2);
		colour = c;
	}
	void print(ofstream& out) {
		out << "Line ";
		out << numberOfPoints << " ";
		for (int i = 0; i < numberOfPoints; i++)
			out << points[i].x << " " << points[i].y << " ";
		out << colour << " "<<style<< endl;
	}
};

class curve :public OpenShapes {
public:
	curve(Point* p, int c, int n, int s)
		:OpenShapes(p, c, n, s)
	{};
	void draw() {
		for (int i = 0; i < numberOfPoints - 1; i++) {
			GP142_lineXY(							 /* Draw a line segment on the graphics window. */
				colour,									   /* color */
				points[i].x, points[i].y,               /* x, y coordinates of starting point */
				points[i + 1].x, points[i + 1].y,           /* x, y coordinates of ending point */
				2);									    /* line width */

			
		}
	}
	bool contains(Point p)
	{
		bool contain = false;

		for (int i = 0; i < numberOfPoints - 1; i++)
			if (pointLiesOnLine(points[i], points[i + 1], p))
				contain = true;

		if (contain == true)
			changeColour(fillCol);

		return contain;
	}
	void changeColour(int c)
	{
		for (int i = 0; i < numberOfPoints - 1; i++) {
			GP142_lineXY(							 /* Draw a line segment on the graphics window. */
				c,									   /* color */
				points[i].x, points[i].y,               /* x, y coordinates of starting point */
				points[i + 1].x, points[i + 1].y,           /* x, y coordinates of ending point */
				2);									    /* line width */
		}
		colour = c;

	}
	void print(ofstream& out) {
		out << "Curve ";
		out << numberOfPoints << " ";
		for (int i = 0; i < numberOfPoints; i++)
			out << points[i].x << " " << points[i].y << " ";
		out << colour << " " << style << endl;
	}
};

class Circle : public Shape {
	int fillColour;
	double radius;
public:
	Circle(Point* p, int c, int n, int fc, double rad)
		:Shape(p, c, n), fillColour(fc), radius(rad)
	{};
	void draw()
	{
		GP142_circleXY(					  /* Draw a filled circle on the graphics window. */
			colour,						      /* color */
			points[0].x, points[0].y,     /* x, y coordinates of center */
			radius);
		
		if (radius >= 3) {
			GP142_circleXY(					  /* Draw a filled circle on the graphics window. */
				1,						      /* color */
				points[0].x, points[0].y,     /* x, y coordinates of center */
				radius - 2);
		}

		if (fillColour != 1)
			fill(fillColour);
	}
	bool contains(Point p)
	{
		bool contain = false;

		if (sqrt((p.x - points[0].x)*(p.x - points[0].x) + (p.y - points[0].y)*(p.y - points[0].y)) <= radius)
			contain = true;

		if (contain == true)
			fill(fillCol);

		return contain;
	}
	void fill(int c)
	{
		int x = points[0].x;
		int y = points[0].y;
		fill(x, y, c);
		fillColour = c;

	}
	void fill(int x, int y, int c)
	{
		/*if (sqrt((x - points[0].x)*(x - points[0].x) + (y - points[0].y)*(y - points[0].y)) < radius) {
			GP142_pixelXY(c, x, y);
			fill(x + 1, y, c);
		//	fill(x - 1, y, c);
		//	fill(x, y + 1, c);
		//	fill(x, y - 1, c);
		//	fill(x + 1, y + 1, c);
		//	fill(x - 1, y - 1, c);
		//	fill(x + 1, y - 1, c);
		//	fill(x - 1, y + 1, c);
		}*/
		int originalX = x;
		int originalY = y;
		while (abs(y)- abs(points[0].y) < radius) {
			if (sqrt((x - points[0].x)*(x - points[0].x) + (y - points[0].y)*(y - points[0].y)) < radius)
				GP142_pixelXY(c, x++, y);
			else {
				x = points[0].x;
				y++;
			}
		}
		x = originalX;
		y = originalY;
		while (abs(y) - abs(points[0].y) < radius) {
			if (sqrt((x - points[0].x)*(x - points[0].x) + (y - points[0].y)*(y - points[0].y)) < radius)
				GP142_pixelXY(c, x--, y);
			else {
				x = points[0].x;
				y++;
			}
		}

		x = originalX;
		y = originalY;
		while (abs(y) - abs(points[0].y) < radius) {
			if (sqrt((x - points[0].x)*(x - points[0].x) + (y - points[0].y)*(y - points[0].y)) < radius)
				GP142_pixelXY(c, x++, y);
			else {
				x = points[0].x;
				y--;
			}
		}
		x = originalX;
		y = originalY;
		while (abs(y) - abs(points[0].y) < radius) {
			if (sqrt((x - points[0].x)*(x - points[0].x) + (y - points[0].y)*(y - points[0].y)) < radius)
				GP142_pixelXY(c, x--, y);
			else {
				x = points[0].x;
				y--;
			}
		}

	}
	void print(ofstream& out) {
		out << "Circle ";
		out << numberOfPoints << " ";
		for (int i = 0; i < numberOfPoints; i++)
			out << points[i].x << " " << points[i].y << " ";
		out << colour << " " << fillColour<<" "<< radius << endl;
	}

};

class Text : public Shape {
	char result[100];
public:
	Text(Point* p, int c, int n)
		:Shape(p, c, n)
	{};
	Text(Point* p, int c, int n, string str)
		:Shape(p, c, n)
	{
		int j;
		for (j = 0; str[j]; j++)
			result[j] = str[j];
		result[j] = '\0';
	};
	void draw()
	{
		GP142_textXY(colour, points[0].x, points[0].y, 25, result);
	}
	
	bool contains(Point p)
	{

		bool contain = false;
		int* height= new int;
		int* width = new int;
		GP142_getDimensions2(25, height, width, result);
		Point* pnt = new Point[2];
		pnt[0] = points[0];
		pnt[1].x = points[0].x + *width;
		pnt[1].y = points[0].y + *height;
		
		if (p.x>=pnt[0].x&&p.x<=pnt[1].x && p.y>=pnt[0].y && p.y<=pnt[1].y)
			contain = true;

		if (contain == true)
			changeColour(fillCol);

		return contain;
	}
	void changeColour(int c)
	{
		GP142_textXY(c, points[0].x, points[0].y, 25, result);
		colour = c;
	}
	void print(ofstream& out) {
		out << "Text ";
		out << numberOfPoints << " ";
		for (int i = 0; i < numberOfPoints; i++)
			out << points[i].x << " " << points[i].y << " ";
		out << colour << " ";
		int k = 0;
		while (result[k])
			out << result[k++];
		out << endl;
	}
};

void displayColours()
{
	GP142_rectangleXY(2, -465, 300, -435, 265, 0);
	GP142_rectangleXY(3, -435,300, -405, 265, 0);
	GP142_rectangleXY(4, -405, 300, -375, 265, 0);
	GP142_rectangleXY(5, -375, 300, -345, 265, 0);
	GP142_rectangleXY(13, -345, 300, -315, 265, 0);
	GP142_rectangleXY(0, -315, 300, -285, 265, 0);
	GP142_rectangleXY(8, -285, 300, -255, 265, 0);
	GP142_rectangleXY(7, -255, 300, -225, 265, 0);
	GP142_rectangleXY(22, -225, 300, -195, 265, 0);
	GP142_rectangleXY(18, -195, 300, -165, 265, 0);
}

int selectColour(Point a)
{
	int col;
	if (a.x >= -465 && a.x <= -435)
		col = 2;
	else if (a.x > -435 && a.x <= -405)
		col = 3;
	else if (a.x > -405 && a.x <= -375)
		col = 4;
	else if (a.x > -375 && a.x <= -345)
		col = 5;
	else if (a.x > -345 && a.x <= -315)
		col = 13;
	else if (a.x > -315 && a.x <= -285)
		col = 0;
	else if (a.x > -285 && a.x <= -255)
		col = 8;
	else if (a.x > -255 && a.x <= -225)
		col = 7;
	else if (a.x > -225 && a.x <= -195)
		col = 22;
	else if (a.x > -195 && a.x <= -165)
		col = 18;
	else
		col = 0;
	return col;

}

void displayInterface(rectangle& R1, rectangle& R2, rectangle& R3, rectangle& R4, rectangle& R5, 
	rectangle& R6,rectangle& R7, rectangle& R8, rectangle& R9, rectangle& R10)
{
	//first icon
	R1.draw();
	GP142_rectangleXY(0, -487, 285, -470, 270, 1);
	//second icon
	R2.draw();
	GP142_circleXY(0, -480, 250, 8);
	//third icon

	R3.draw();
	GP142_lineXY(0, -485, 210, -470, 210, 2);
	//fourth icon

	R4.draw();
	GP142_lineXY(0, -490, 180, -480, 190, 2);
	GP142_lineXY(0, -480, 190, -470, 180, 2);
	GP142_lineXY(0, -470, 180, -470, 175, 2);
	GP142_lineXY(0, -470, 175, -480, 165, 2);
	GP142_lineXY(0, -480, 165, -490, 180, 2);
	//fifth icon

	R5.draw();
	GP142_printfXY(0, -490, 125, 40, "~");
	//sixth icon

	R6.draw();
	GP142_rectangleXY(0, -487, 115, -470, 100, 0);
	//seventh icon

	R7.draw();
	GP142_textXY(0, -490, 55, 30, "T");
	//eighth icon

	R8.draw();
	GP142_textXY(0, -490, 30, 10, "Save");
	//nineth icon

	R9.draw();
	GP142_textXY(0, -490, -5, 10, "Load");

	R10.draw();
	GP142_lineXY(2, -487, -25, -470, -25, 3);
	GP142_lineXY(3, -487, -29, -470, -29, 3);
	GP142_lineXY(15, -487, -33, -470, -33, 3);
	GP142_lineXY(20, -487, -37, -470, -37, 3);
	GP142_lineXY(17, -487, -41, -470, -41, 3);
}

int main(void)
{

	int quit;
	int event;
	int mouse_x, mouse_y;
	char key_pressed;

	Shape** AllShapes = new Shape*[1];
	int size = 1;
	int filled = 0;
	int index = 0;
	GP142_open();
	GP142_clear();


	int i = 0;
	quit = FALSE;
	Point* p = new Point[2];
	p[0] = { -500, 300 };
	p[1] = { -465, 265 };
	rectangle R1(p, 0, 2, 1);
	p[0] = { -500, 265 };
	p[1] = { -465, 230 };
	rectangle R2(p, 0, 2, 1);
	p[0] = { -500, 230 };
	p[1] = { -465, 195 };
	rectangle R3(p, 0, 2, 1);
	p[0] = { -500, 195 };
	p[1] = { -465, 160 };
	rectangle R4(p, 0, 2, 1);

	p[0] = { -500, 160 };
	p[1] = { -465, 125};
	rectangle R5(p, 0, 2, 1);
	p[0] = { -500, 125};
	p[1] = { -465, 90};
	rectangle R6(p, 0, 2, 1);
	p[0] = { -500, 90 };
	p[1] = { -465, 55 };
	rectangle R7(p, 0, 2, 1);
	p[0] = { -500, 55 };
	p[1] = { -465, 20 };
	rectangle R8(p, 0, 2, 1);
	p[0] = { -500, 20 };
	p[1] = { -465, -15 };
	rectangle R9(p, 0, 2, 1);
	p[0] = { -500, -15 };
	p[1] = { -465, -50 };
	rectangle R10(p, 0, 2, 1);

	displayInterface(R1, R2, R3, R4, R5, R6, R7, R8, R9, R10);

	//colours
	displayColours();
	int color = 0;
	bool select = true;

	event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
	Point a = { (double)mouse_x, (double)mouse_y };

	while (!quit) {

		if (event == GP142_MOUSE)
		{
			if ((a.y >= 265 && a.y <= 300 && a.x <= -165 && a.x >= -465)) {
				color = selectColour(a);

			}

			else if (R1.contains(a) || R2.contains(a) || R3.contains(a) || R4.contains(a) || R5.contains(a)
				|| R6.contains(a) || R7.contains(a) || R8.contains(a) || R9.contains(a) || R10.contains(a))
			{
				select = true;
			}
			if (R1.contains(a))    //rectangle
			{
				while (R1.contains(a)) {
					Point* p = new Point[2];
					for (int i = 0; i < 2; i++) {
						event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
						p[i] = { (double)mouse_x , (double)mouse_y };
						if (R1.contains(p[i]) || R2.contains(p[i]) || R3.contains(p[i]) || R4.contains(p[i]) || R5.contains(p[i])
							|| R6.contains(p[i]) || R7.contains(p[i]) || R8.contains(p[i]) || R9.contains(p[i])
							|| R10.contains(p[i])) {
							select = false;
							a = p[i];
							delete[]p;
							break;
						}
						else if ((p[i].y >= 265 && p[i].y <= 300 && p[i].x <= -165 && p[i].x >= -465)) {
							color = selectColour(p[i]);
							i = -1;
						}
					}

					if (select == true) {
						if (filled < size) {
							AllShapes[index] = new rectangle(p, color, 2, 1);
							AllShapes[index]->draw();
							index++;
							filled++;
						}
						else {
							Shape** temp = new Shape*[size * 2];
							for (int j = 0; j < size; j++) {
								temp[j] = AllShapes[j];
							}
							temp[index] = new rectangle(p, color, 2, 1);
							temp[index]->draw();
							delete[]AllShapes;
							AllShapes = temp;
							filled++;
							index++;
							size *= 2;
						}

					}
				}

			}
			else if (R2.contains(a))   //circle
			{
				while (R2.contains(a)) {
					Point* p = new Point[1];
					event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					p[0] = { (double)mouse_x , (double)mouse_y };

					if (R1.contains(p[0]) || R2.contains(p[0]) || R3.contains(p[0]) || R4.contains(p[0]) || R5.contains(p[0])
						|| R6.contains(p[0]) || R7.contains(p[0]) || R8.contains(p[0]) || R9.contains(p[0])
						|| R10.contains(p[0])) {
						select = false;
						a = p[0];

					}
					else if ((p[0].y >= 265 && p[0].y <= 300 && p[0].x <= -165 && p[0].x >= -465)) {
						color = selectColour(p[0]);

						event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
						p[0] = { (double)mouse_x , (double)mouse_y };
					}

					if (select == true) {
						event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
						Point x = { (double)mouse_x , (double)mouse_y };
						if (R1.contains(x) || R2.contains(x) || R3.contains(x) || R4.contains(x) || R5.contains(x)
							|| R6.contains(x) || R7.contains(x) || R8.contains(x) || R9.contains(x)
							|| R10.contains(x)) {
							select = false;
							a = x;

						}
						double rad = sqrt((x.x - p[0].x)*(x.x - p[0].x) + (x.y - p[0].y)*(x.y - p[0].y));

						if (select == true && rad<180) {
							if (filled < size) {
								AllShapes[index] = new Circle(p, color, 1, 1, rad);
								AllShapes[index]->draw();
								index++;
								filled++;
							}
							else {
								Shape** temp = new Shape*[size * 2];
								for (int j = 0; j < size; j++) {
									temp[j] = AllShapes[j];
								}
								temp[index] = new Circle(p, color, 1, 1, rad);
								temp[index]->draw();
								delete[]AllShapes;
								AllShapes = temp;
								filled++;
								index++;
								size *= 2;
							}

						}
					}
				}
			}
			else if (R3.contains(a))    //line
			{
				while (R3.contains(a)) {
					Point* p = new Point[2];
					for (int i = 0; i < 2; i++) {
						event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
						p[i] = { (double)mouse_x , (double)mouse_y };
						if (R1.contains(p[i]) || R2.contains(p[i]) || R3.contains(p[i]) || R4.contains(p[i]) || R5.contains(p[i])
							|| R6.contains(p[i]) || R7.contains(p[i]) || R8.contains(p[i]) || R9.contains(p[i])
							|| R10.contains(p[i])) {
							select = false;
							a = p[i];
							delete[]p;
							break;
						}
						else if ((p[i].y >= 265 && p[i].y <= 300 && p[i].x <= -165 && p[i].x >= -465)) {
							color = selectColour(p[i]);
							i = -1;
							
						}
					}
					if (select == true) {
						if (filled < size) {
							AllShapes[index] = new line(p, color, 2, 0);
							AllShapes[index]->draw();
							index++;
							filled++;
						}
						else {
							Shape** temp = new Shape*[size * 2];
							for (int j = 0; j < size; j++) {
								temp[j] = AllShapes[j];
							}
							temp[index] = new line(p, color, 2, 0);
							temp[index]->draw();
							delete[]AllShapes;
							AllShapes = temp;
							filled++;
							index++;
							size *= 2;
						}
					}
				}
			}
			else if (R4.contains(a))   //polygon
			{
				while (R4.contains(a)) {
					int side = 3;
					Point *p = new Point[side];
					int i = 0;
					event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					while (event == GP142_MOUSE) {
						if (i < side) {
							p[i] = { (double)mouse_x , (double)mouse_y };
							if (R1.contains(p[i]) || R2.contains(p[i]) || R3.contains(p[i]) || R4.contains(p[i]) || R5.contains(p[i])
								|| R6.contains(p[i]) || R7.contains(p[i]) || R8.contains(p[i]) || R9.contains(p[i])
								|| R10.contains(p[i])) {
								select = false;
								a = p[i];
								delete[]p;
								break;
							}
							else if ((p[i].y >= 265 && p[i].y <= 300 && p[i].x <= -165 && p[i].x >= -465)) {
								color = selectColour(p[i]);
								i = -1;
							}
							i++;
						}
						else {
							Point* temp = new Point[side * 2];

							for (int j = 0; j < side; j++)
								temp[j] = p[j];
							temp[i] = { (double)mouse_x , (double)mouse_y };
							if (R1.contains(temp[i]) || R2.contains(temp[i]) || R3.contains(temp[i]) || R4.contains(temp[i]) || R5.contains(temp[i])
								|| R6.contains(temp[i]) || R7.contains(temp[i]) || R8.contains(temp[i]) || R9.contains(temp[i])
								|| R10.contains(temp[i])) {
								select = false;
								a = p[i];
								delete[]p;
								break;
							}

							i++;
							side *= 2;
							delete[] p;
							p = temp;
						}
						event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					}

					if (select == true) {

						if (filled < size) {
							AllShapes[index] = new polygon(p, color, i);
							AllShapes[index]->draw();
							index++;
							filled++;
						}
						else {
							Shape** temp = new Shape*[size * 2];
							for (int j = 0; j < size; j++) {
								temp[j] = AllShapes[j];
							}
							temp[index] = new polygon(p, color, i);
							temp[index]->draw();
							delete[]AllShapes;
							AllShapes = temp;
							filled++;
							index++;
							size *= 2;
						}
					}
				}
			}
			else if (R5.contains(a))   //curve
			{
				while (R5.contains(a)) {
					int side = 3;
					Point *p = new Point[side];
					int i = 0;
					event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					while (event == GP142_MOUSE) {
						if (i < side) {
							p[i] = { (double)mouse_x , (double)mouse_y };
							if (R1.contains(p[i]) || R2.contains(p[i]) || R3.contains(p[i]) || R4.contains(p[i]) || R5.contains(p[i])
								|| R6.contains(p[i]) || R7.contains(p[i]) || R8.contains(p[i]) || R9.contains(p[i])
								|| R10.contains(p[i])) {
								select = false;
								a = p[i];
								delete[]p;
								break;
							}
							else if ((p[i].y >= 265 && p[i].y <= 300 && p[i].x <= -165 && p[i].x >= -465)) {
								color = selectColour(p[i]);
								i = -1;
							}
							i++;
						}
						else {
							Point* temp = new Point[side * 2];

							for (int j = 0; j < side; j++)
								temp[j] = p[j];
							temp[i] = { (double)mouse_x , (double)mouse_y };
							if (R1.contains(temp[i]) || R2.contains(temp[i]) || R3.contains(temp[i]) || R4.contains(temp[i]) || R5.contains(temp[i])
								|| R6.contains(temp[i]) || R7.contains(temp[i]) || R8.contains(temp[i]) || R9.contains(temp[i])
								|| R10.contains(temp[i])) {
								select = false;
								a = p[i];
								delete[]p;
								break;
							}
							i++;
							side *= 2;
							delete[] p;
							p = temp;
						}
						event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					}
					if (select == true) {

						if (filled < size) {
							AllShapes[index] = new curve(p, color, i, 0);
							AllShapes[index]->draw();

							index++;
							filled++;
						}
						else {
							Shape** temp = new Shape*[size * 2];
							for (int j = 0; j < size; j++) {
								temp[j] = AllShapes[j];
							}
							temp[index] = new curve(p, color, i, 0);
							temp[index]->draw();
							delete[]AllShapes;
							AllShapes = temp;
							filled++;
							index++;
							size *= 2;
						}
					}
				}
			}
			else if (R6.contains(a)) {   //erase 

				event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
				Point b = { (double)mouse_x, (double)mouse_y };
				while (R6.contains(a)) {
					Shape** temp = new Shape*[size];
					int x = filled - 2;
					int flag = 0;
					for (int j = filled - 1; j >= 0; j--) {
						if (flag ==0 && AllShapes[j]->contains(b) ) {
							delete AllShapes[j];
							flag = 1;
						}
						else {
							temp[x--] = AllShapes[j];
						}
					}
					if (flag == 1) {
						delete[] AllShapes;
						AllShapes = temp;

						filled--;
						index--;
					}

					if (size > filled * 2 && filled > 0) {
						Shape** temp2 = new Shape*[size / 2];
						for (int a = 0; a < filled; a++)
							temp2[a] = AllShapes[a];
						delete[]AllShapes;
						AllShapes = temp2;
						size /= 2;
					}

					if (flag == 1) {
						GP142_clear();
						displayInterface(R1, R2, R3, R4, R5, R6, R7, R8, R9, R10);
						displayColours();

						for (int y = 0; y < filled; y++)
							AllShapes[y]->draw();
					}


					event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					b.x = (double)mouse_x;
					b.y = (double)mouse_y;
					if (R1.contains(b) || R2.contains(b) || R3.contains(b) || R4.contains(b) || R5.contains(b)
						|| R7.contains(b) || R8.contains(b) || R9.contains(b) || R10.contains(b)) {
						select = false;
						a = b;
					}
				}
			}
			else if (R7.contains(a)) {          //text
				while (R7.contains(a)) {
					event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					Point b = { (double)mouse_x , (double)mouse_y };
					if (R1.contains(b) || R2.contains(b) || R3.contains(b) || R4.contains(b) || R5.contains(b)
						|| R6.contains(b) || R7.contains(b) || R8.contains(b) || R9.contains(b)
						|| R10.contains(b)) {
						select = false;
						a = b;
					}
					else if ((b.y >= 265 && b.y <= 300 && b.x <= -165 && b.x >= -465)) {
						color = selectColour(b);

						event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
						b = { (double)mouse_x , (double)mouse_y };
					}
					if (select == true) {
						Point* p = new Point[1];
						p[0] = b;
						if (filled < size) {
							const char prompt[14] = "Enter string\n";
							char res[100];
							GP142_gets(                  
								prompt,     
								res);
							AllShapes[index] = new Text(p, color, 1, res);
							AllShapes[index]->draw();
							index++;
							filled++;
						}
						else
						{
							Shape** temp = new Shape*[size * 2];
							for (int j = 0; j < size; j++)
								temp[j] = AllShapes[j];
							const char prompt[14] = "Enter string\n";
							char res[100];
							GP142_gets(                 
								prompt,     
								res);
							temp[index] = new Text(p, color, 1, res);
							temp[index]->draw();
							delete[]AllShapes;
							AllShapes = temp;
							index++;
							filled++;
							size *= 2;

						}
					}
				}
			}
			else if (R8.contains(a)) {  //save
				ofstream out;
				out.open("drawing.pb");
				out << filled << endl;
				for (int i = 0; i < filled; i++) {
					AllShapes[i]->print(out);
				}
				out.close();
			}
			else if (R9.contains(a)) {  //load
				ifstream in;
				in.open("drawing.pb");
				in >> size;
				filled = size;
				string type;
				delete[] AllShapes;
				AllShapes = new Shape*[size];
				for (int k = 0; k < size; k++) {
					in >> type;
					if (type == "Rectangle") {
						int vertices;
						in >> vertices;
						Point* arr = new Point[vertices];
						for (int j = 0; j < vertices; j++)
							in >> arr[j].x >> arr[j].y;
						int color, fillcol;
						in >> color >> fillcol;
						AllShapes[k] = new rectangle(arr, color, 2, fillcol);
						AllShapes[k]->draw();
					}
					else if (type == "Circle") {
						int vert;
						in >> vert;
						Point* arr = new Point[vert];
						in >> arr[0].x >> arr[0].y;
						int color, fillcol;
						double Cradius;
						in >> color >> fillcol >> Cradius;
						AllShapes[k] = new Circle(arr, color, 1, fillcol, Cradius);
						AllShapes[k]->draw();
					}
					else if (type == "Line") {
						int vert;
						in >> vert;
						Point* arr = new Point[vert];
						for (int j = 0; j < vert; j++)
							in >> arr[j].x >> arr[j].y;
						int color, sty;
						in >> color >> sty;
						AllShapes[k] = new line(arr, color, vert, sty);
						AllShapes[k]->draw();
					}
					else if (type == "Polygon") {
						int vert;
						in >> vert;
						Point* arr = new Point[vert];
						for (int j = 0; j < vert; j++)
							in >> arr[j].x >> arr[j].y;
						int col, fillcol;
						in >> col >> fillcol;
						AllShapes[k] = new polygon(arr, col, vert, fillcol);
						AllShapes[k]->draw();
					}
					else if (type == "Curve") {
						int vert;
						in >> vert;
						Point* arr = new Point[vert];
						for (int j = 0; j < vert; j++)
							in >> arr[j].x >> arr[j].y;
						int col, sty;
						in >> col >> sty;
						AllShapes[k] = new curve(arr, col, vert, sty);
						AllShapes[k]->draw();
					}
					else if (type == "Text") {
						int vert;
						in >> vert;
						Point* arr = new Point[1];
						in >> arr[0].x >> arr[0].y;
						int color;
						string str;
						in >> color >> str;
						AllShapes[k] = new Text(arr, color, 1, str);
						char outtext[100];
						int j;
						for (j = 0; str[j]; j++)
							outtext[j] = str[j];
						outtext[j] = '\0';
						GP142_textXY(color, arr[0].x, arr[0].y, 25, outtext);
					}
					index = k + 1;
				}
			}
			else if (R10.contains(a))   //bucket
			{
				event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
				Point b = { (double)mouse_x , (double)mouse_y };
				if ((b.y >= 215 && b.y <= 250 && b.x <= 35 && b.x >= -265))
					fillCol = selectColour(b);

				while (R10.contains(a)) {
					int check = 0;
					if (R1.contains(b) || R2.contains(b) || R3.contains(b) || R4.contains(b) || R5.contains(b)
						|| R6.contains(b) || R7.contains(b) || R8.contains(b) || R9.contains(b)
						|| R10.contains(b)) {
						select = false;
						a = b;
					}
					else if ((b.y >= 265 && b.y <= 300 && b.x <= -165 && b.x >= -465))
						fillCol = selectColour(b);


					else {
						for (int k = filled - 1; k >= 0 && check == 0; k--) {
							if(AllShapes[k]->contains(b))
								check = 1;
						}
					}

					event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
					b.x = (double)mouse_x;
					b.y = (double)mouse_y;
					if (R1.contains(b) || R2.contains(b) || R3.contains(b) || R4.contains(b) || R5.contains(b)
						|| R7.contains(b) || R8.contains(b) || R9.contains(b) || R10.contains(b)) {
						select = false;
						a = b;
					}
				}
			}

		}
		
		
		if (event == GP142_QUIT)
			quit = TRUE;

		if (select == true) {
			event = GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
			a.x = (double)mouse_x;
			a.y = (double)mouse_y;
		}

	}

	GP142_close();
	return 0;


}