//how to install fmltk https://bumpyroadtocode.com/2017/08/29/how-to-install-and-use-fltk-1-3-4-in-visual-studio-2017-complete-guide-2-0-no-cross-contamination/
#define _USE_MATH_DEFINES
#include "Fix/Simple_window.h"
#include "Fix/Graph.h"
#include "Fix/std_lib_facilities.h"
#include <iostream>
#include <cmath>

constexpr auto WIN_LENGTH = 600; //in pixels
constexpr auto WIN_WIDTH = 400;


using namespace Graph_lib; //our graphics facilities are in Graph_lib (defined in Graph.h)

int main() {
	try {

		Point top_left{ 100, 100 }; //will be top left corner of window
		Simple_window win{ top_left, WIN_LENGTH, WIN_WIDTH, "simple window" };

		
		Graph_lib::TextBox window_box{ Point{ 80, 50 }, 70, "Window" };
		window_box.set_color(Color::black);
		Graph_lib::TextBox simple_window_box{ Point{ 50, 150 }, 120, "Simple_window" };
		simple_window_box.set_color(Color::black);
		Graph_lib::Arrow win_sim_win_arrow{ Point{ 110, 150 }, Point{ 110,75 } };
		win_sim_win_arrow.set_color(Color::black);

		//wIndow and simple window
		win.attach(window_box);
		win.attach(simple_window_box);
		win.attach(win_sim_win_arrow);

		win.wait_for_button(); //give control to the display engine
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		return 2;
	}
	return 0;
}
