
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"

//------------------------------------------------------------------------------

Simple_window::Simple_window(Point xy, int w, int h, const string& title) : Window(xy, w, h, title),
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
	  menu_button{ Point{ x_max() - 80, 30 }, 80, 20, "color menu",cb_menu }
		  {
	attach(next_button);
	attach(quit_button);
	attach(next_x);
	attach(next_y);
	attach(xy_out);
	xy_out.put("no point");

	//probably change to lambda
	color_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "red",cb_red });
	color_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "blue", cb_blue });
	color_menu.attach(new Button{ Point{ 0, 0 }, 0, 0, "black", cb_black });

	attach(color_menu);
	color_menu.hide();
	attach(menu_button);
	attach(lines);
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
	hide();//idiom of FLTK to delete window
}

//------------------------------------------------------------------------------