
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
struct My_window:Graph_lib::Window {
	My_window(Point xy, int w, int h, const string& title);

private:
	Button next_btn; // the "next" button
	bool next_btn_pushed; // implementation detail

	Button quit_btn; // the "quit" button


	void next() ;	// action to be done when next_button is pressed
	void quit() ; // action to be done when next_button is pressed
	


};

//------------------------------------------------------------------------------
struct Button_window : My_window {
	Button_window(Point xy, int w, int h, const string& title);

private:
	// widgets
	Menu menu1;
	Menu menu2;
	Menu menu3;
	Menu menu4;
	Out_box xy_out;

	// actions invoked by callback: relabel button, print coordinates
	void reset_labels(); // helper function
	void action(Button& b);
	void action11() { action(menu1.selection[0]); }
	void action12() { action(menu1.selection[1]); }
	void action13() { action(menu1.selection[2]); }
	void action14() { action(menu1.selection[3]); }
	void action21() { action(menu2.selection[0]); }
	void action22() { action(menu2.selection[1]); }
	void action23() { action(menu2.selection[2]); }
	void action24() { action(menu2.selection[3]); }
	void action31() { action(menu3.selection[0]); }
	void action32() { action(menu3.selection[1]); }
	void action33() { action(menu3.selection[2]); }
	void action34() { action(menu3.selection[3]); }
	void action41() { action(menu4.selection[0]); }
	void action42() { action(menu4.selection[1]); }
	void action43() { action(menu4.selection[2]); }
	void action44() { action(menu4.selection[3]); }

	// callback functions
	static void cb_action11(Address, Address pw) { reference_to<Button_window>(pw).action11(); }
	static void cb_action12(Address, Address pw) { reference_to<Button_window>(pw).action12(); }
	static void cb_action13(Address, Address pw) { reference_to<Button_window>(pw).action13(); }
	static void cb_action14(Address, Address pw) { reference_to<Button_window>(pw).action14(); }
	static void cb_action21(Address, Address pw) { reference_to<Button_window>(pw).action21(); }
	static void cb_action22(Address, Address pw) { reference_to<Button_window>(pw).action22(); }
	static void cb_action23(Address, Address pw) { reference_to<Button_window>(pw).action23(); }
	static void cb_action24(Address, Address pw) { reference_to<Button_window>(pw).action24(); }
	static void cb_action31(Address, Address pw) { reference_to<Button_window>(pw).action31(); }
	static void cb_action32(Address, Address pw) { reference_to<Button_window>(pw).action32(); }
	static void cb_action33(Address, Address pw) { reference_to<Button_window>(pw).action33(); }
	static void cb_action34(Address, Address pw) { reference_to<Button_window>(pw).action34(); }
	static void cb_action41(Address, Address pw) { reference_to<Button_window>(pw).action41(); }
	static void cb_action42(Address, Address pw) { reference_to<Button_window>(pw).action42(); }
	static void cb_action43(Address, Address pw) { reference_to<Button_window>(pw).action43(); }
	static void cb_action44(Address, Address pw) { reference_to<Button_window>(pw).action44(); }
};

//------------------------------------------------------------------------------
struct Btn_img_window : My_window {
	Btn_img_window(Point xy, int w, int h, const string& title,  const string& image_name);

private:
	Button btn;	  // the "next" button


	Image image; // stores inside image


	void rand_cord(); // action to be done when next_button is pressed

	int win_width;
	int win_height;

	int btn_size;//btn is rect
	Point xy{};	  //left upper corner coordinates of button
};
//------------------------------------------------------------------------------
struct Shape_window : My_window {
	Shape_window(Point xy, int w, int h, const string& title);
	
	//TODO add more shapes 

	int x;
	int y;
	Vector_ref<Shape> shapes;//holds shapes to draw them

private:
	const int rectangle_size = 100;
	//widgets
	Menu shape_menu;//holds shapes buttons
	In_box shape_x;
	In_box shape_y;

	void get_coordinates();//helper func to place from user input into x,y

	void rect_pressed();
	void circle_pressed();

	static void cb_rect(Address, Address pw) { reference_to<Shape_window>(pw).rect_pressed(); }
	static void cb_circle(Address, Address pw) { reference_to<Shape_window>(pw).circle_pressed(); }
};

//------------------------------------------------------------------------------
#endif // SIMPLE_WINDOW_GUARD
