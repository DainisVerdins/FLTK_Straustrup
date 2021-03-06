
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include "Graph.h"

#include "std_lib_facilities.h"
#include <cmath>
//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	Shape::Shape(initializer_list<Point> lst) {
		for (Point p : lst)
			add(p);
	}

	//------------------------------------------------------------------------------

	void Shape::set_color(Color col) {
		lcolor = col;
	}

	//------------------------------------------------------------------------------

	Color Shape::color() const {
		return lcolor;
	}

	//------------------------------------------------------------------------------

	void Shape::set_style(Line_style sty) {
		ls = sty;
	}

	//------------------------------------------------------------------------------

	Line_style Shape::style() const {
		return ls;
	}

	//------------------------------------------------------------------------------

	void Shape::set_fill_color(Color col) {
		fcolor = col;
	}

	//------------------------------------------------------------------------------

	Color Shape::fill_color() const {
		return fcolor;
	}

	//------------------------------------------------------------------------------

	void Shape::add(Point p) // protected
	{
		points.push_back(p);
	}

	//------------------------------------------------------------------------------

	Point Shape::point(int i) const {
		return points[i];
	}

	//------------------------------------------------------------------------------

	int Shape::number_of_points() const {
		return int(points.size());
	}

	//------------------------------------------------------------------------------

	void Shape::set_point(int i, Point p) // not used; not necessary so far
	{
		points[i] = p;
	}

	//------------------------------------------------------------------------------
	void Shape::draw_lines() const {
		if (fill_color().visibility()) {
			fl_color(fill_color().as_int());
			fl_begin_complex_polygon();
			for (int i = 0; i < number_of_points(); ++i) {
				fl_vertex(point(i).x, point(i).y);
			}
			fl_end_complex_polygon();
			fl_color(color().as_int()); // reset color
		}

		if (color().visibility() && 1 < points.size()) // draw sole pixel?
			for (unsigned int i = 1; i < points.size(); ++i)
				fl_line(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
	}

	//------------------------------------------------------------------------------

	void Shape::draw() const {
		Fl_Color oldc = fl_color();
		// there is no good portable way of retrieving the current style
		fl_color(lcolor.as_int());			   // set color
		fl_line_style(ls.style(), ls.width()); // set style
		draw_lines();
		fl_color(oldc);	  // reset color (to previous)
		fl_line_style(0); // reset line style to default
	}

	//------------------------------------------------------------------------------


	void Shape::move(int dx, int dy) // move the shape +=dx and +=dy
	{
		for (size_t i = 0; i < points.size(); ++i) {
			points[i].x += dx;
			points[i].y += dy;
		}
	}

	//------------------------------------------------------------------------------

	Line::Line(Point p1, Point p2) // construct a line from two points
	{
		add(p1); // add p1 to this shape
		add(p2); // add p2 to this shape
	}

	//------------------------------------------------------------------------------

	void Lines::add(Point p1, Point p2) {
		Shape::add(p1);
		Shape::add(p2);
	}

	//------------------------------------------------------------------------------

	// draw lines connecting pairs of points
	void Lines::draw_lines() const {
		if (color().visibility())
			for (int i = 1; i < number_of_points(); i += 2)
				fl_line(point(i - 1).x, point(i - 1).y, point(i).x, point(i).y);
	}

	//------------------------------------------------------------------------------

	Lines::Lines(initializer_list<pair<Point, Point>> lst) {
		for (auto const& p : lst)
			add(p.first, p.second);
	}

	//------------------------------------------------------------------------------

	// does two lines (p1,p2) and (p3,p4) intersect?
	// if so, return the distance of the intersect point as distances from p1
	inline pair<double, double> line_intersect(Point p1, Point p2,
		Point p3, Point p4, bool& parallel) {
		double x1 = p1.x;
		double x2 = p2.x;
		double x3 = p3.x;
		double x4 = p4.x;
		double y1 = p1.y;
		double y2 = p2.y;
		double y3 = p3.y;
		double y4 = p4.y;

		double denom = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
		if (denom == 0) {
			parallel = true;
			return pair<double, double>(0, 0);
		}
		parallel = false;
		return pair<double, double>(((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom,
			((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom);
	}

	//------------------------------------------------------------------------------

	//intersection between two line segments
	//Returns true if the two segments intersect,
	//in which case intersection is set to the point of intersection
	bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection) {
		bool parallel;
		pair<double, double> u = line_intersect(p1, p2, p3, p4, parallel);
		if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1)
			return false;
		intersection.x = p1.x + u.first * (double(p2.x) - double(p1.x));
		intersection.y = p1.y + u.first * (double(p2.y) - double(p1.y));
		return true;
	}

	//------------------------------------------------------------------------------

	void Polygon::add(Point p) {
		int np = number_of_points();

		if (1 < np) { // check that thenew line isn't parallel to the previous one
			if (p == point(np - 1))
				error("polygon point equal to previous point");
			bool parallel;
			line_intersect(point(np - 1), p, point(np - 2), point(np - 1), parallel);
			if (parallel)
				error("two polygon points lie in a straight line");
		}
		for (int i = 1; i < np - 1; ++i) { // check that new segment doesn't interset and old point
			Point ignore{ 0, 0 };
			if (line_segment_intersect(point(np - 1), p, point(i - 1), point(i), ignore))
				error("intersect in polygon");
		}

		Closed_polyline::add(p);
	}

	//------------------------------------------------------------------------------

	Polygon::Polygon(initializer_list<Point> lst) {
		for (Point p : lst)
			add(p);
	}

	//------------------------------------------------------------------------------

	void Closed_polyline::draw_lines() const {
		Open_polyline::draw_lines(); // first draw the "open poly line part"

		// then draw closing line:
		if (2 < number_of_points() && color().visibility())
			fl_line(point(number_of_points() - 1).x,
				point(number_of_points() - 1).y,
				point(0).x,
				point(0).y);
	}

	//------------------------------------------------------------------------------

	void draw_mark(Point xy, char c) {
		static const int dx = 4;
		static const int dy = 4;

		string m(1, c); // string holding a single char c
		fl_draw(m.c_str(), xy.x - dx, xy.y + dy);
	}

	//------------------------------------------------------------------------------

	void Marked_polyline::draw_lines() const {
		Open_polyline::draw_lines();
		for (int i = 0; i < number_of_points(); ++i)
			draw_mark(point(i), mark[i % mark.size()]);
	}

	//------------------------------------------------------------------------------

	Marked_polyline::Marked_polyline(const string& m, initializer_list<Point> lst)
		: Open_polyline{ lst }, mark{ m } {
		if (m == "")
			mark = "*";
	}

	//------------------------------------------------------------------------------

	Rectangle::Rectangle(Point xy, int ww, int hh) : w(ww), h(hh) {
		if (h <= 0 || w <= 0)
			error("Bad rectangle: non-positive side");
		add(xy);
	}

	//------------------------------------------------------------------------------

	Rectangle::Rectangle(Point x, Point y) : w(y.x - x.x), h(y.y - x.y) {
		if (h <= 0 || w <= 0)
			error("Bad rectangle: first point is not top left");
		add(x);
	}

	//------------------------------------------------------------------------------

	void Rectangle::draw_lines() const {
		if (fill_color().visibility()) { // fill
			fl_color(fill_color().as_int());
			fl_rectf(point(0).x, point(0).y, w, h);
		}

		if (color().visibility()) { // lines on top of fill
			fl_color(color().as_int());
			fl_rect(point(0).x, point(0).y, w, h);
		}
	}

	//------------------------------------------------------------------------------

	Circle::Circle(Point p, int rr) // center and radius
		: r(rr) {
		add(Point{ p.x - r, p.y - r }); // store top-left corner
	}

	//------------------------------------------------------------------------------

	Point Circle::center() const {
		return Point{ point(0).x + r, point(0).y + r };
	}

	//------------------------------------------------------------------------------

	void Circle::draw_lines() const {

		if (fill_color().visibility()) { // fill
			fl_color(fill_color().as_int());
			fl_pie(point(0).x, point(0).y, r + r - 1, r + r - 1, 0, 360);
			fl_color(color().as_int()); // reset color
		}

		if (color().visibility()) {
			fl_color(color().as_int());
			fl_arc(point(0).x, point(0).y, r + r, r + r, 0, 360);
		}
	}

	//------------------------------------------------------------------------------

	Ellipse::Ellipse(Point p, int w, int h) : w{ w }, h{ h } {
		add(Point{ p.x - w, p.y - h });
	}

	//------------------------------------------------------------------------------

	Point Ellipse::center() const {
		return Point{ point(0).x + w, point(0).y + h };
	}

	//------------------------------------------------------------------------------

	Point Ellipse::focus1() const {
		if (h <= w) // foci are on the x axis
			return Point{ center().x + int(sqrt(double(w) * double(w) - double(h) * double(h))), center().y };
		else // foci are on the y axis
			return Point{ center().x, center().y + int(sqrt(double(h) * double(h) - double(w) * double(w))) };
	}

	//------------------------------------------------------------------------------


	Point Ellipse::focus2() const {
		if (h <= w) // foci are on the x axis
			return Point{ center().x - int(sqrt(double(w) * double(w) - double(h) * double(h))), center().y };
		else // foci are on the y axis
			return Point{ center().x, center().y - int(sqrt(double(h) * double(h) - double(w) * double(w))) };
	}

	//------------------------------------------------------------------------------

	void Ellipse::draw_lines() const {
		if (color().visibility())
			fl_arc(point(0).x, point(0).y, w + w, h + h, 0, 360);
	}

	//------------------------------------------------------------------------------

	void Text::draw_lines() const {
		int ofnt = fl_font();
		int osz = fl_size();
		fl_font(fnt.as_int(), fnt_sz);
		fl_draw(lab.c_str(), point(0).x, point(0).y);
		fl_font(ofnt, osz);
	}

	//------------------------------------------------------------------------------

	Axis::Axis(Orientation d, Point xy, int length, int n, string lab) : label(Point{ 0, 0 }, lab) {
		if (length < 0)
			error("bad axis length");
		switch (d) {
		case Axis::x: {
			Shape::add(xy); // axis line
			Shape::add(Point{ xy.x + length, xy.y });

			if (1 < n) { // add notches
				int dist = length / n;
				int x = xy.x + dist;
				for (int i = 0; i < n; ++i) {
					notches.add(Point{ x, xy.y }, Point{ x, xy.y - 5 });
					x += dist;
				}
			}
			// label under the line
			label.move(xy.x + length / 3, xy.y + 20);
			break;
		}
		case Axis::y: {
			Shape::add(xy); // a y-axis goes up
			Shape::add(Point{ xy.x, xy.y - length });

			if (1 < n) { // add notches
				int dist = length / n;
				int y = xy.y - dist;
				for (int i = 0; i < n; ++i) {
					notches.add(Point{ xy.x, y }, Point{ xy.x + 5, y });
					y -= dist;
				}
			}
			// label at top
			label.move(xy.x - 10, xy.y - length - 10);
			break;
		}
		case Axis::z:
			error("z axis not implemented");
		}
	}

	//------------------------------------------------------------------------------

	void Axis::draw_lines() const {
		Shape::draw_lines();
		notches.draw(); // the notches may have a different color from the line
		label.draw();	// the label may have a different color from the line
	}

	//------------------------------------------------------------------------------

	void Axis::set_color(Color c) {
		Shape::set_color(c);
		notches.set_color(c);
		label.set_color(c);
	}

	//------------------------------------------------------------------------------

	void Axis::move(int dx, int dy) {
		Shape::move(dx, dy);
		notches.move(dx, dy);
		label.move(dx, dy);
	}

	//------------------------------------------------------------------------------

	Function::Function(Fct f, double r1, double r2, Point xy,
		int count, double xscale, double yscale)
	// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
	// x coordinates are scaled by xscale and y coordinates scaled by yscale
	{
		if (r2 - r1 <= 0)
			error("bad graphing range");
		if (count <= 0)
			error("non-positive graphing count");
		double dist = (r2 - r1) / count;
		double r = r1;
		for (int i = 0; i < count; ++i) {
			add(Point{ xy.x + int(r * xscale), xy.y - int(f(r) * yscale) });
			r += dist;
		}
	}

	Function::Function(double (*f)(double), double r1, double r2, Point xy,
		int count, double xscale, double yscale) : Function{ static_cast<Fct>(f), r1, r2, xy, count, xscale, yscale } {
	}

	//------------------------------------------------------------------------------

	bool can_open(const string& s)
	// check if a file named s exists and can be opened for reading
	{
		ifstream ff(s);
		return ff.is_open();
	}

	//------------------------------------------------------------------------------

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

	Suffix get_encoding(const string& s) {
		struct SuffixMap {
			const char* extension;
			Suffix suffix;
		};

		static SuffixMap smap[] = {
			{ ".jpg", Suffix::jpg },
			{ ".jpeg", Suffix::jpg },
			{ ".gif", Suffix::gif },
		};

		for (int i = 0, n = ARRAY_SIZE(smap); i < n; i++) {
			int len = strlen(smap[i].extension);

			if (int(s.length()) >= len && s.substr(s.length() - len, len) == smap[i].extension)
				return smap[i].suffix;
		}

		return Suffix::none;
	}

	//------------------------------------------------------------------------------

	// somewhat over-elaborate constructor
	// because errors related to image files can be such a pain to debug
	Image::Image(Point xy, string s, Suffix e)
		: w(0), h(0), fn(xy, "") {
		add(xy);

		if (!can_open(s)) { // can we open s?
			fn.set_label("cannot open \"" + s + '"');
			p = new Bad_image(30, 20); // the "error image"
			return;
		}

		if (e == Suffix::none)
			e = get_encoding(s);

		switch (e) { // check if it is a known encoding
		case Suffix::jpg:
			p = new Fl_JPEG_Image(s.c_str());
			break;
		case Suffix::gif:
			p = new Fl_GIF_Image(s.c_str());
			break;
		default: // Unsupported image encoding
			fn.set_label("unsupported file type \"" + s + '"');
			p = new Bad_image(30, 20); // the "error image"
		}
	}

	//------------------------------------------------------------------------------

	void Image::draw_lines() const {
		if (fn.label() != "")
			fn.draw_lines();

		if (w && h)
			p->draw(point(0).x, point(0).y, w, h, cx, cy);
		else
			p->draw(point(0).x, point(0).y);
	}

	//------------------------------------------------------------------------------



	Arc::Arc(Point center_coords, int r, double start_angle, double end_angle)
		: center{ center_coords }, radius{ r }, start{ start_angle }, end{ end_angle }

	{

		if (start_angle >= end_angle) {
			error("value degree first param must be lower than second param");
		}
		if (r <= 0) {
			error("radius cannot be negative or zero");
		}
		add(Point{ center_coords.x - r, center_coords.y - r }); // middle point of arc from with things happens
																//add(center_coords); // if so then arc is moved at anu axis by +r
	}


	void Arc::set_angles(double start_angle, double end_angle) {
		if (end_angle <= start_angle) {
			error("second arc angle must be bigger than first angle");
		}
		start = start_angle;
		end = end_angle;
	}

	void Arc::draw_lines() const {
		if (color().visibility()) {
			int w = radius + radius;
			int h = w;
			fl_arc(point(0).x, point(0).y, w, h, start, end);
		}
	}

	Box::Box(Point xy, int ww, int hh)
		: w{ ww }, h{ hh } {

		if (h <= 0 || w <= 0)
			error("Bad rectangle: non-positive side WIDTH OR HEIGHT");
		add(xy);
		radius = w < h ? w / 4 : h / 4;
	}

	void Box::draw_lines() const {
		if (color().visibility()) {
			/*
			////part of lines
			///*
			//	A ----B
			//	|     |
			//	C-----D

			*/

			//TODO : remove variables

			int Ax = point(0).x;
			int Ay = point(0).y;
			int Bx = Ax + w;
			int By = Ay;


			int Cx = point(0).x;
			int Cy = point(0).y + h;

			int Dx = Bx;
			int Dy = By + h;

			//drawing lines
			// from A to B
			fl_line(Ax + (radius / 2), Ay, Bx - (radius / 2), By);
			//from B to D
			fl_line(Bx, By + (radius / 2), Dx, Dy - (radius / 2));
			//from A to C
			fl_line(Ax, Ay + (radius / 2), Cx, Cy - (radius / 2));
			//from C to D
			fl_line(Cx + (radius / 2), Cy, Dx - (radius / 2), Dy);

			//drawing arcs
			fl_arc(Bx - radius, By, radius, radius, 0, 90);
			fl_arc(Ax, Ay, radius, radius, 90, 180);
			fl_arc(Cx, Cy - radius, radius, radius, 180, 270);
			fl_arc(Dx - radius, Dy + -radius, radius, radius, 270, 0);
		}
	}


	void Arrow::draw_lines() const {
		if (color().visibility()) {
			// NOT MY CODE ATT ALL BUT IS WORKING
			//draw a line
			Line::draw_lines();

			// add arrowhead: p2 and two points
			double line_len =
				sqrt(double(pow(point(1).x - point(0).x, 2) + pow(point(1).y - point(0).y, 2))); // length of p1p2

			// coordinates of the a point on p1p2 with distance 8 from p2
			double pol_x = 8 / line_len * point(0).x + (1 - 8 / line_len) * point(1).x;
			double pol_y = 8 / line_len * point(0).y + (1 - 8 / line_len) * point(1).y;

			// pl is 4 away from p1p2 on the "left", pl_pol is orthogonal to p1p2
			double pl_x = pol_x + 4 / line_len * (double(point(1).y) - double(point(0).y));
			double pl_y = pol_y + 4 / line_len * (double(point(0).x) - double(point(1).x));

			// pr is 4 away from p1p2 on the "right", pr_pol is orthogonal to p1p2
			double pr_x = pol_x + 4 / line_len * (double(point(0).y) - double(point(1).y));
			double pr_y = pol_y + 4 / line_len * (double(point(1).x) - double(point(0).x));

			// draw arrowhead - is always filled in line color
			if (color().visibility()) {
				fl_begin_complex_polygon();
				fl_vertex(point(1).x, point(1).y);
				fl_vertex(pl_x, pl_y);
				fl_vertex(pr_x, pr_y);
				fl_end_complex_polygon();
			}
		}
	}


	//North angle of React
	Point n(const Graph_lib::Rectangle& r) {

		return Point{ r.point(0).x + (r.width() / 2), r.point(0).y };
	}

	//South of rect
	Point s(const Graph_lib::Rectangle& r) {
		return Point{ r.point(0).x + (r.width() / 2), r.point(0).y + (r.height() / 2) };
	}
	//east of rect
	Point e(const Graph_lib::Rectangle& r) {
		return Point{ r.point(0).x + r.width(), r.point(0).y + (r.height() / 2) };
	}
	//west of rect point
	Point w(const Graph_lib::Rectangle& r) {
		return Point{ r.point(0).x, r.point(0).y + (r.height() / 2) };
	}

	Point center(const Graph_lib::Rectangle& r) {

		return Point{ r.point(0).x + (r.width() / 2), r.point(0).y + (r.height() / 2) };
	}

	Point ne(const Graph_lib::Rectangle& r) {
		return Point{ r.point(0).x + r.width(), r.point(0).y };
	}

	Point se(const Graph_lib::Rectangle& r) {
		return Point{ r.point(0).x + r.width(), r.point(0).y + r.height() };
	}

	Point nw(const Graph_lib::Rectangle& r) {
		return r.point(0); //left corner
	}

	Point sw(const Graph_lib::Rectangle& r) {
		return Point{ r.point(0).x, r.point(0).y + r.height() };
	}

	Point n(const Graph_lib::Circle& c) {

		return Point{ c.point(0).x + (c.radius() / 2), c.point(0).y };
	}

	Point s(const Graph_lib::Circle& c) {
		return Point{ c.point(0).x + (c.radius() / 2), c.point(0).y + (c.radius() / 2) };
	}

	Point e(const Graph_lib::Circle& c) {
		return Point{ c.center().x + c.radius(), c.center().y };
	}

	Point w(const Graph_lib::Circle& c) {
		return Point{ c.center().x - c.radius(), c.center().y };
	}

	Point center(const Graph_lib::Circle& c) {
		return Point{ c.point(0).x + (c.radius() / 2), c.point(0).y + (c.radius() / 2) };
	}

	Point ne(const Graph_lib::Circle& c) {
		return Point{ c.point(0).x + c.radius(), c.point(0).y };
	}

	Point se(const Graph_lib::Circle& c) {
		return Point{ c.point(0).x + c.radius(), c.point(0).y + c.radius() };
	}

	Point nw(const Graph_lib::Circle& c) {
		return Point{ c.point(0).x, c.point(0).y };
	}

	Point sw(const Graph_lib::Circle& c) {
		return Point{ c.point(0).x, c.point(0).y + c.radius() };
	}

	Point n(const Graph_lib::Ellipse& el) {


		return Point{ el.center().x, el.center().y - el.minor() };
	}

	Point s(const Graph_lib::Ellipse& el) {
		return Point{ el.center().x, el.center().y + el.minor() };
	}

	Point e(const Graph_lib::Ellipse& el) {
		//calc distance from center to elipse end by horiznotal
		//https://www.softschools.com/math/calculus/finding_the_foci_of_an_ellipse/

		int a = std::sqrt(std::pow(el.minor(), 2) + std::pow(el.major(), 2));


		return Point{ el.center().x + a, el.center().y };
	}

	Point w(const Graph_lib::Ellipse& el) {
		//calc distance from center to elipse end by horiznotal
		int a = std::sqrt(std::pow(el.minor(), 2) + std::pow(el.major(), 2));
		return Point{ el.center().x - a, el.center().y };
	}

	Point center(const Graph_lib::Ellipse& el) {
		return el.center();
	}

	Point ne(const Graph_lib::Ellipse& el) {

		int a = std::sqrt(std::pow(el.minor(), 2) + std::pow(el.major(), 2));

		return Point{ el.center().x + a, el.center().y - el.minor() };
	}

	Point se(const Graph_lib::Ellipse& el) {
		int a = std::sqrt(std::pow(el.minor(), 2) + std::pow(el.major(), 2));
		return Point{ el.center().x + a, el.center().y + el.minor() };
	}

	Point nw(const Graph_lib::Ellipse& el) {
		int a = std::sqrt(std::pow(el.minor(), 2) + std::pow(el.major(), 2));
		return Point{ el.center().x - a, el.center().y - el.minor() };
	}

	Point sw(const Graph_lib::Ellipse& el) {
		int a = std::sqrt(std::pow(el.minor(), 2) + std::pow(el.major(), 2));
		return Point{ el.center().x - a, el.center().y + el.minor() };
	}

	TextBox::TextBox(Point xy, int ww, const std::string& s)
		: Box::Box{ xy, ww, H_TB }, text{ Point{ xy.x + 7, xy.y + 17 }, s } //7 and 17 just on eye was took
	{}

	void TextBox::draw_lines() const {
		if (color().visibility()) {
			Box::draw_lines();
			text.draw(); //draw text inside box
		}
	}

	void TextBox::set_color(Color c) {
		Box::set_color(c);
		text.set_color(c);
	}

	void TextBox::move(int dx, int dy) {
		Box::Box::move(dx, dy);
		text.move(dx, dy);
	}

	Regular_hexagon::Regular_hexagon(Point xy, int ss)
		: s(ss) {
		for (size_t i = 0; i < 6; i++) {
			add(Point{});
		}
		set_points(xy, ss);
	}

	void Regular_hexagon::draw_lines() const {
		if (fill_color().visibility()) {
			fl_color(fill_color().as_int());
			fl_begin_complex_polygon();
			for (int i = 0; i < number_of_points(); ++i)
				fl_vertex(point(i).x, point(i).y);
			fl_end_complex_polygon();
			fl_color(color().as_int()); // reset color
		}

		if (color().visibility()) {
			for (int i = 0; i < number_of_points() - 1; ++i)
				fl_line(point(i).x, point(i).y, point(i + 1).x, point(i + 1).y);
			fl_line(point(5).x, point(5).y, point(0).x, point(0).y); // close hexagon
		}
	}

	void Regular_hexagon::set_points(Point xy, int s) {
		set_point(0, Point(xy.x - s, xy.y)); // set leftmost point
		set_point(1, Point(xy.x - round(s / 2.0), xy.y + round(sqrt(3) / 2 * s)));
		set_point(2, Point(xy.x + round(s / 2.0), xy.y + round(sqrt(3) / 2 * s)));
		set_point(3, Point(xy.x + s, xy.y));
		set_point(4, Point(xy.x + round(s / 2.0), xy.y - round(sqrt(3) / 2 * s)));
		set_point(5, Point(xy.x - round(s / 2.0), xy.y - round(sqrt(3) / 2 * s)));


	}

	Smiley::Smiley(Point xy, int r)
		: Circle(xy, r) {
	}

	void Smiley::draw_lines() const {
		if (color().visibility()) {
			Circle::draw_lines();
			int eye_radius = eye_scale * Circle::radius();
			int mouth_radius = mouth_scale * Circle::radius();

			//lips of smile//probably just to need use ugly fl_arc() not extra class
			Arc lips{ Point{ Circle::center().x, Circle::center().y + (Circle::radius() / 2) },
				mouth_radius, 180, 360 };

			lips.draw_lines();

			int eye_height_koef = 4; //from radius how higth are eyes
			int distance = 5;		 //distance between eyes in pixels from radius

			Circle reye{ Point{ Circle::center().x - eye_radius - distance,
							 Circle::center().y - (Circle::radius() / eye_height_koef) },
				eye_radius };
			reye.draw_lines();

			Circle leye{ Point{ Circle::center().x + eye_radius + distance,
							 Circle::center().y - (Circle::radius() / eye_height_koef) },
				eye_radius };
			leye.draw_lines();
		}
	}

	Frowny::Frowny(Point xy, int r)
		: Circle(xy, r) {
	}

	void Frowny::draw_lines() const {
		if (color().visibility()) {
			Circle::draw_lines();
			int eye_radius = eye_scale * Circle::radius();
			int mouth_radius = mouth_scale * Circle::radius();

			//lips of smile//probably just to need use ugly fl_arc() not extra class
			Arc lips{ Point{ Circle::center().x, Circle::center().y + (Circle::radius() / 2) },
				mouth_radius, 0, 180 };

			lips.draw_lines();

			int eye_height_koef = 4; //from radius how higth are eyes
			int distance = 5;		 //distance between eyes in pixels from radius

			Circle reye{ Point{ Circle::center().x - eye_radius - distance,
							 Circle::center().y - (Circle::radius() / eye_height_koef) },
				eye_radius };
			reye.draw_lines();

			Circle leye{ Point{ Circle::center().x + eye_radius + distance,
							 Circle::center().y - (Circle::radius() / eye_height_koef) },
				eye_radius };
			leye.draw_lines();
		}
	}

	Frowny_hat::Frowny_hat(Point xy, int r)
		: Frowny::Frowny(xy, r) {}

	void Frowny_hat::draw_lines() const {
		Frowny::draw_lines();

		//triangular hat //
		Point top{ Frowny::center().x, Frowny::center().y - (Frowny::radius() * 2) };
		Point left_angle{ Frowny::center().x - Frowny::radius(), Frowny::center().y - Frowny::radius() };
		Point rigth_angle{ Frowny::center().x + Frowny::radius(), Frowny::center().y - Frowny::radius() };

		Closed_polyline hat{ top };
		hat.add(left_angle);
		hat.add(rigth_angle);

		hat.draw_lines();
	}

	Smiley_hat::Smiley_hat(Point xy, int r)
		: Smiley::Smiley{ xy, r } {
	}

	void Smiley_hat::draw_lines() const {
		Smiley::draw_lines();

		//triangular hat //
		Point top{ Smiley::center().x, Smiley::center().y - (Smiley::radius() * 2) };
		Point left_angle{ Smiley::center().x - Smiley::radius(), Smiley::center().y - Smiley::radius() };
		Point rigth_angle{ Smiley::center().x + Smiley::radius(), Smiley::center().y - Smiley::radius() };

		Closed_polyline hat{ top };
		hat.add(left_angle);
		hat.add(rigth_angle);

		hat.draw_lines();
	}

	Immobile_Circle::Immobile_Circle(Point xy, int r)
		: Circle::Circle{ xy, r } {
	}

	void Immobile_Circle::move(int dx, int dy) {
	}

	Striped_rectangle::Striped_rectangle(Point xy, int ww, int hh) : Rectangle::Rectangle{ xy, ww, hh } {
	}

	Striped_rectangle::Striped_rectangle(Point x, Point y) : Rectangle::Rectangle{ x, y } {
	}

	void Striped_rectangle::draw_lines() const {
		if (fill_color().visibility()) { // fill

			//line magick
			//for striped line rect
			for (size_t i = 0; i < Rectangle::width(); i++) {
				if (i % 2 == 0 && i != 0) {
					fl_color(fill_color().as_int());
					fl_line(point(0).x + i, point(0).y, point(0).x + i, point(0).y + Rectangle::height() - 1);
				}
			}
			fl_color(fill_color().as_int()); //reset color
		}

		if (color().visibility()) { // lines on top of fill
			fl_color(color().as_int());
			fl_rect(point(0).x, point(0).y, Rectangle::width(), Rectangle::height());
		}
	}

	Striped_circle::Striped_circle(Point xy, int r) : Circle{ xy, r } {
	}

	void Striped_circle::draw_lines() const {
		if (fill_color().visibility()) { // fill
			fl_color(fill_color().as_int());
			//line magick
			//for striped line rect
			int dy = 0;
			int dx = 0;
			for (size_t i = (Circle::center().x - Circle::radius()); 
				i < Circle::center().x + Circle::radius();i += 2) 
			{

				dy = center().y - i; //get line length of specific y in circle
				dx = round(sqrt(radius() * radius() - dy * dy)) - 1;

				fl_line(center().x - dx, i, center().x + dx, i);
			}
			fl_color(fill_color().as_int()); //reset color
		}

		if (color().visibility()) { // lines on top of fill
			fl_color(color().as_int());
			fl_arc(point(0).x, point(0).y, 2 * radius(), 2 * radius(), 0, 360);
		}
	}

} // of namespace Graph_lib
