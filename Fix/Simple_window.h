
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef SIMPLE_WINDOW_GUARD
#define SIMPLE_WINDOW_GUARD 1

#include "GUI.h" // for Simple_window only (doesn't really belong in Window.h)
#include "Graph.h"

using namespace Graph_lib;

//------------------------------------------------------------------------------

struct Simple_window : Graph_lib::Window {
	Simple_window(Point xy, int w, int h, const string& title);

	void wait_for_button(); // simple event loop

private:
	Button next_button; // the "next" button
	bool button_pushed; // implementation detail

	static void cb_next(Address, Address); // callback for next_button
	void next();						   // action to be done when next_button is pressed
};

//------------------------------------------------------------------------------
struct Lines_window : Graph_lib::Window {
	Lines_window(Point xy, int w, int h, const string& title);

private:
	//data
	Open_polyline lines;

	//widgets
	Button next_button; //places pair (next_x,next_y) into obj lines
	Button quit_button;
	In_box next_x;
	In_box next_y;
	Out_box xy_out;
	Menu color_menu;
	Button menu_button;

	Menu line_style_menu;//pop pup menu for showing diferent lines
	Button line_style_btn;

	void hide_menu() {
		color_menu.hide();
		menu_button.show();
	}

	void red_pressed() {
		change(Color::red);
		hide_menu();
	}
	void blue_pressed() {
		change(Color::blue);
		hide_menu();
	}
	void black_pressed() {
		change(Color::black);
		hide_menu();
	}
	void menu_pressed() {
		menu_button.hide();
		color_menu.show();
	}
	void change(Color c) {
		lines.set_color(c);
		hide_menu();
	}

	void style_menu_pressed() {
		line_style_btn.hide();
		line_style_menu.show();
	}
	void change_style(Line_style s) {
		lines.set_style(s);
		hide_lines_menu();
	}
	//solid , dash dot
	void solid_pressed() {
		change_style(Line_style::solid);
		hide_lines_menu();
	}
	void dash_pressed() {
		change_style(Line_style::dash);
		hide_lines_menu();
	}
	void dot_pressed() {
		change_style(Line_style::dot);
		hide_lines_menu();
	}
	void hide_lines_menu() {
		line_style_menu.hide();
		line_style_btn.show();
	}

	void next();
	void quit();


	//callback for red btn
	static void cb_red(Address, Address pw);
	//callback for blue btn
	static void cb_blue(Address, Address pw);
	//callback for black btn
	static void cb_black(Address, Address pw);



	static void cb_menu(Address, Address pw);
	static void cb_next(Address, Address pw);
	static void cb_quit(Address, Address pw);

	static void cb_style_menu(Address, Address pw);
};

//------------------------------------------------------------------------------
#endif // SIMPLE_WINDOW_GUARD
