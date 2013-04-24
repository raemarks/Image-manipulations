// imageadder.cpp
/* Add two PPM images together using a green-screen approach.
*
* John Schneider
*/

#include <iostream>

#include "Image.h"

using namespace std;

void print_help(void);

int main() {
	string filename;
	string yes_no;
	Image i1, i2, i3;
	char c;

	cout << "This program \"adds\" two images together using a green-\n"
		"screen technique.  Two PPM files of the same size are combined to\n"
		"form a third image.  The pixels in the third image come from the\n"
		"first image if the pixel value of the first image falls below the\n"
		"\"green threshold.\"  Otherwise the pixel comes from the second time."
		<< endl;

	print_help();
////TEST CODE to test overloaded operators.
//	cout << "Enter name of input file for image 1: ";
//			cin >> filename;
//			i1.read(filename);
//
//			cout << "Setting i2 = i1" << endl;
//			i2 = i1; 
//			cout << "Enter name of output file for image 2: ";
//			cin >> filename;
//			cout << "Do you want binary output? [yes/no] ";
//			cin >> yes_no;
//			if (yes_no[0] == 'Y' || yes_no[0] == 'y') 
//				i2.write(filename, false);
//			else
//				i2.write(filename, true);
//
//			cout << "Now setting i4 = new Image(i1)" <<endl;
//			Image *i4 = new Image(i1);
//			cout << "Enter name of output file for image 4: ";
//			cin >> filename;
//			cout << "Do you want binary output? [yes/no] ";
//			cin >> yes_no;
//			if (yes_no[0] == 'Y' || yes_no[0] == 'y') 
//				i4->write(filename, false);
//			else
//				i4->write(filename, true);
//			
////END TEST CODE
	cout << "cmd: ";

	while ((c = cin.get()) != EOF) {
		switch (c) {

		case '1':
			cout << "Enter name of input file for image 1: ";
			cin >> filename;
			i1.read(filename);
			
			break;

		case '2':
			cout << "Enter name of input file for image 2: ";
			cin >> filename;
			i2.read(filename);
			break;

		case 'W':
		case 'w':
			cout << "Enter name of output file for image 3: ";
			cin >> filename;
			cout << "Do you want binary output? [yes/no] ";
			cin >> yes_no;
			if (yes_no[0] == 'Y' || yes_no[0] == 'y') 
				i3.write(filename, false);
			else
				i3.write(filename, true);
			break;

		case 'D':
		case 'd':
			cout << "Red, Green, and Blue thresholds: " 
				<< Image::get_rrThreshold() << ", "
				<< Image::get_ggThreshold() << ", "
				<< Image::get_bbThreshold() << endl;
			break;

		case 'S':
		case 's':
			int rrThreshold, ggThreshold, bbThreshold;
			cout << "Enter the red, green, and blue thresholds: ";
			cin >>  rrThreshold >> ggThreshold >> bbThreshold;
			Image::set_rrThreshold(rrThreshold);
			Image::set_ggThreshold(ggThreshold);
			Image::set_bbThreshold(bbThreshold);
			break;

		case '+':
			cout << "adding images 1 and 2; storing in 3" << endl;
			i3 = i1 + i2;
			break;

		case 'H':
		case 'h':
			print_help();
			break;

		case 'Q':
		case 'q':
			exit(0);
			break;   // Should never get here.

		case '\n': // Ignore whitespace.
		case '\t':
		case ' ':
			cout << "cmd: ";
			break;

		default:
			cout << "Unrecognized command." << endl;

		}
	}

	return 0;
}

void print_help(void) {
	cout << "\nValid commands:\n"
		"  1 = read from file image 1\n"
		"  2 = read from file image 2\n"
		"  + = add images 1 and 2, store in image 3\n"
		"  w = write image 3\n"
		"  d = display threshold levels\n"
		"  s = set threshold levels\n"
		"  h = print this menu\n"
		"  q = quit\n" << endl;
	return;
}