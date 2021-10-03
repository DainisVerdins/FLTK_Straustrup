//how to install fmltk https://bumpyroadtocode.com/2017/08/29/how-to-install-and-use-fltk-1-3-4-in-visual-studio-2017-complete-guide-2-0-no-cross-contamination/

#include <iostream>

#include "Fix/Simple_window.h"
#include "Fix/std_lib_facilities.h"


constexpr int xmax = 600; //size of the window
constexpr int ymax = 400;
//Distance:
constexpr int xoffset = 100; //from left side of y axis
constexpr int yoffset = 60;	 //from down side of x axi

constexpr int xspace = 40; //Distance below axis
constexpr int yspace = 40;
//length of axis
constexpr int xlength = xmax - xoffset - xspace;
constexpr int ylength = ymax - yoffset - yspace;



using namespace std;
using namespace Graph_lib; //our graphics facilities are in Graph_lib (defined in Graph.h)



int main() {
	try {

		Point top_left{ xspace, yspace }; //will be top left corner of window
		My_window win{ top_left, xmax, ymax, "GUI polyliner" };//task 1 done

		
		return gui_main();

	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch (...) {
		return 2;
	}
	return 0;
}
