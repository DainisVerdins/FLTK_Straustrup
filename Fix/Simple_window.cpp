
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"

//------------------------------------------------------------------------------

Simple_window::Simple_window(Point xy, int w, int h, const string& title)
	: Window(xy, w, h, title),
																			next_button(Point{ x_max() - 70, 0 }, 70, 20, "Next", cb_next),
																			button_pushed(false) {
	attach(next_button);
}

//------------------------------------------------------------------------------
void Simple_window::wait_for_button()
// modified event loop:
// handle all events (as per default), quit when button_pushed becomes true
// this allows graphics without control inversion
{
	// Simpler handler
	while (!button_pushed)
		Fl::wait();
	button_pushed = false;
	Fl::redraw();
}

//------------------------------------------------------------------------------

void Simple_window::cb_next(Address, Address pw)
// call Simple_window::next() for the window located at pw
{
	reference_to<Simple_window>(pw).next();
}

//------------------------------------------------------------------------------

void Simple_window::next() {
	button_pushed = true;
}

//------------------------------------------------------------------------------

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
	: Window{ xy, w, h, title },
	  next_button{ Point{ x_max() - 150, 0 }, 70, 20, "Next point",
		  [](Address, Address pw) {
			  reference_to<Lines_window>(pw).next();
		  } },
	  quit_button{ Point{ x_max() - 70, 0 }, 70, 20, "Quit",
		  [](Address, Address pw) {
			  reference_to<Lines_window>(pw).quit();
		  } },
	  next_x{ Point{ x_max() - 310, 0 }, 50, 20, "next x:" },
	  next_y{ Point{ x_max() - 210, 0 }, 50, 20, "next y: " },
	  xy_out{ Point{ 100, 0 }, 100, 20, "current (х,y) :" },

	  color_menu{ Point{ x_max() - 70, 40 }, 70, 20, Menu::vertical, "color" },
	  menu_button{ Point{ x_max() - 80, 30 }, 80, 20, "color menu", cb_menu },

	  line_style_menu{ Point{ x_max() - 150, 40 }, 90, 20, Menu::vertical, "style" },
	  line_style_btn{ Point{ x_max() - 160, 30 }, 70, 20, "style menu", cb_style_menu } {
	attach(next_button);
	attach(quit_button);
	attach(next_x);
	attach(next_y);
	attach(xy_out);
	xy_out.put("no point");

	//probably change to lambda
	color_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "red", cb_red });
	color_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "blue", cb_blue });
	color_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "black", cb_black });

	attach(color_menu);
	color_menu.hide();
	attach(menu_button);
	attach(lines);

	//line style stuff
	line_style_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "solid", [](Address, Address pw) {
										  reference_to<Lines_window>(pw).solid_pressed();
									  } });
	line_style_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "dash", [](Address, Address pw) {
										  reference_to<Lines_window>(pw).dash_pressed();
									  } });
	line_style_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "dot", [](Address, Address pw) {
										  reference_to<Lines_window>(pw).dot_pressed();
									  } });

	attach(line_style_menu);
	line_style_menu.hide();
	attach(line_style_btn);
}

void Lines_window::cb_red(Address, Address pw) {

	reference_to<Lines_window>(pw).red_pressed();
}

void Lines_window::cb_blue(Address, Address pw) {
	reference_to<Lines_window>(pw).blue_pressed();
}

void Lines_window::cb_black(Address, Address pw) {
	reference_to<Lines_window>(pw).black_pressed();
}

void Lines_window::cb_menu(Address, Address pw) {
	reference_to<Lines_window>(pw).menu_pressed();
}

void Lines_window::cb_next(Address, Address pw) {
	reference_to<Lines_window>(pw).next();
}

void Lines_window::cb_quit(Address, Address pw) {
	reference_to<Lines_window>(pw).quit();
}

void Lines_window::cb_style_menu(Address, Address pw) {
	reference_to<Lines_window>(pw).style_menu_pressed();
}


void Lines_window::next() {
	int x = next_x.get_int();
	int y = next_y.get_int();
	lines.add(Point{ x, y });

	//update of current Point
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	redraw();
}

void Lines_window::quit() {
	hide(); //idiom of FLTK to delete window
}

//------------------------------------------------------------------------------

My_window::My_window(Point xy, int w, int h, const string& title)
	: Window{ xy, w, h, title },
	  next_btn{ Point{ x_max() - 150, 0 }, 70, 20, "Next",
		  [](Address, Address pw) {
			  reference_to<My_window>(pw).next();
		  } },
	  quit_btn{ Point{ x_max() - 70, 0 }, 70, 20, "Quit",
		  [](Address, Address pw) {
			  reference_to<My_window>(pw).quit();
		  } } {
	next_btn_pushed = false;
	attach(next_btn);
	attach(quit_btn);
}

void My_window::next() {
	next_btn_pushed = true;
	redraw();
}

void My_window::quit() {
	hide(); //idiom of FLTK to delete window
}

Button_window::Button_window(Point xy, int w, int h, const string& title)
	: My_window(xy, w, h, title),
	  menu1(Point(0, 0), 100, 100, Menu::vertical, "menu1"),
	  menu2(Point(100, 0), 100, 100, Menu::vertical, "menu2"),
	  menu3(Point(200, 0), 100, 100, Menu::vertical, "menu3"),
	  menu4(Point(300, 0), 100, 100, Menu::vertical, "menu4"),
	  xy_out(Point(x_max() - 70, 60), 70, 20, "(x,y)") 
{
	menu1.attach(new Button(Point(0, 0), 0, 0, "1-1", cb_action11));
	menu1.attach(new Button(Point(0, 0), 0, 0, "1-2", cb_action12));
	menu1.attach(new Button(Point(0, 0), 0, 0, "1-3", cb_action13));
	menu1.attach(new Button(Point(0, 0), 0, 0, "1-4", cb_action14));
	attach(menu1);
	menu2.attach(new Button(Point(0, 0), 0, 0, "2-1", cb_action21));
	menu2.attach(new Button(Point(0, 0), 0, 0, "2-2", cb_action22));
	menu2.attach(new Button(Point(0, 0), 0, 0, "2-3", cb_action23));
	menu2.attach(new Button(Point(0, 0), 0, 0, "2-4", cb_action24));
	attach(menu2);
	menu3.attach(new Button(Point(0, 0), 0, 0, "3-1", cb_action31));
	menu3.attach(new Button(Point(0, 0), 0, 0, "3-2", cb_action32));
	menu3.attach(new Button(Point(0, 0), 0, 0, "3-3", cb_action33));
	menu3.attach(new Button(Point(0, 0), 0, 0, "3-4", cb_action34));
	attach(menu3);
	menu4.attach(new Button(Point(0, 0), 0, 0, "4-1", cb_action41));
	menu4.attach(new Button(Point(0, 0), 0, 0, "4-2", cb_action42));
	menu4.attach(new Button(Point(0, 0), 0, 0, "4-3", cb_action43));
	menu4.attach(new Button(Point(0, 0), 0, 0, "4-4", cb_action44));
	attach(menu4);
	attach(xy_out);
	xy_out.put("no point");

}

void Button_window::reset_labels() {
	for (int i = 0; i < 4; ++i) {
		menu1.selection[i].label = "1-" + to_string(i + 1);
		menu2.selection[i].label = "2-" + to_string(i + 1);
		menu3.selection[i].label = "3-" + to_string(i + 1);
		menu4.selection[i].label = "4-" + to_string(i + 1);
	}
	redraw();
}

void Button_window::action(Button& b) {
	reset_labels();
	b.label = "CLICKED";
	ostringstream os;
	os << '(' << b.loc.x << ',' << b.loc.y << ')';
	xy_out.put(os.str());
}

Btn_img_window::Btn_img_window(Point xy, int w, int h, const string& title, const string& image_name) 
:  My_window(xy, w, h, title),
	  image(Point{ 0 ,0 }, image_name),
	  btn{ Point{ x_max()- (w/2), (h/2) }, 100, 100, "",
			  [](Address, Address pw) {
			  reference_to<Btn_img_window>(pw).rand_cord();
			  } }
{
	btn_size = 100;
	win_width = w;
	win_height = h;
	xy.x= x_max() - (w / 2);
	xy.y= (h / 2);
	attach(btn);
	image.set_mask(Point{ x_max() - (w / 2), (h / 2) }, btn_size, btn_size);
	image.move(x_max() - (w / 2), (h / 2));
	attach(image);
	
	
}

void Btn_img_window::rand_cord() {
	//TODO fix out of window jumps!!!!!
	std::cout << "pressed!\n";
	int by_x = abs(rand_int(0,  btn_size) - int(xy.x) );
	int by_y = abs(rand_int(0,  btn_size) - int(xy.y));
	std::cout << by_x <<" " << by_y<< "\n ";
	btn.move(by_x, by_y);
	image.move(by_x, by_y);
}

Shape_window::Shape_window(Point xy, int w, int h, const string& title)
	: My_window{ xy, w, h, title },
	  shape_menu{ Point(0, 0), 100, 100, Menu::vertical, "Shapes" },
	  shape_x{ Point{ x_max() - 330, 0 }, 50, 20, "shape x:" },
	  shape_y{ Point{ x_max() - 210, 0 }, 50, 20, "shape y:" }
 {



	shape_menu.attach(new Button{ Point{ 0, 0 }, 50, 20, "Rect", cb_rect });
	shape_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "Circle", cb_circle});
	//shape_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "triangle", [](Address, Address pw) {
	//									  reference_to<Lines_window>(pw).dot_pressed();
	//								  } });

	attach(shape_menu);
	attach(shape_x);
	attach(shape_y);
	x = 0;
	y = 0;
}

void Shape_window::get_coordinates() {
	x = shape_x.get_int();
	y = shape_y.get_int();
}

void Shape_window::rect_pressed() {
	get_coordinates();
	shapes.push_back(new Graph_lib::Rectangle{ Point{ x, y }, rectangle_size, rectangle_size });
	attach(shapes[shapes.size() - 1]);
}

void Shape_window::circle_pressed() {
	get_coordinates();
	shapes.push_back(new Graph_lib::Circle{ Point{ x, y }, rectangle_size });
	attach(shapes[shapes.size() - 1]);
}
