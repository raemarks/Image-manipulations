#include <iostream>
#include <fstream>
#include <ctime>

#include "Image.h"
#define MAXMAX 255  // Maximum value allowed.

/* Start code which is prepended to desired encoded string.  The start
 * code should *NOT* include \x00 -- at least not when constructed
 * using a string literal such as this.
PROGRAMMER: Raeanne Marks.
Prototypes provided by John Schneider at the class website. Contents of some functions written by Raeanne Marks.
 */
static string const start_code = "\x01\xFF\x01\xFF";
using namespace std;

int Image::rrThreshold = 160; //setting the thresholds outside of class definition
int Image::ggThreshold = 160;
int Image::bbThreshold = 160;



// Default constructor.
Image::Image(): rrPtr(0), ggPtr(0), bbPtr(0), grayPtr(0), 
	filename(""), type(""), size_x(0), size_y(0),
	max(0)
{
	cout << "Constructor called." << endl;
	return;
} // Image()


// Constructor when input file name is given.
Image::Image(string the_filename) 
{
	read(the_filename);
	return;
} // Image(string)

Image::Image(const Image &image){ //COPY CONSTRUCTOR
	type = image.type; //start copying things over
	size_x = image.size_x;
	size_y = image.size_y;
	filename = image.filename;
	max = image.max;
	rrThreshold = image.rrThreshold;
	ggThreshold = image.ggThreshold;
	bbThreshold = image.bbThreshold;

	if (type == "P5") //pgm file
	{
		grayPtr = new unsigned char[size_x * size_y]; //allocate memory
		for (int i = 0; i < (size_x*size_y); i++)
		{
			grayPtr[i] = image.grayPtr[i]; //must manually assign everything. Deep copy
		}
	}
	else //ppm file
	{
		rrPtr = new unsigned char [size_x*size_y]; //allocate memory
		ggPtr = new unsigned char [size_x*size_y];
		bbPtr = new unsigned char [size_x*size_y];
		for (int i = 0; i < (size_x*size_y); i++)
		{
			rrPtr[i] = image.rrPtr[i]; //deep copy
			ggPtr[i] = image.ggPtr[i];
			bbPtr[i] = image.bbPtr[i];
		}
	}
}



// Destructor.
Image::~Image() 
{
	if (type == "P6") //ppm
	{
		delete [] rrPtr; //delete all three arrays
		delete [] ggPtr;
		delete [] bbPtr;
	}
	else //pgm
	{
		delete [] grayPtr; //just one array to delete.
	}
	return;
} // ~Image()


// Read image information from the specified file.
void Image::read(string the_filename) 
{
	filename = the_filename;
	ifstream in(filename.c_str(), ios_base::binary); //open to read binary
	if (!in) //check if it opened properly
	{
		cerr << "Error opening " << filename << endl;
		return;
	}
	in >> type >> size_x >> size_y >> max;

	if (type == "P2") //pgm
	{
		type = "P5"; //change the type to P5
		grayPtr = new unsigned char[size_x*size_y]; //allocate enough space
		in.ignore();
		int g; //have a place holder variable for typecasting
		for (int y = 0; y < size_y; y++)
		{
			for (int x = 0; x < size_x; x++)
			{
				in >> g; //g for gray.
				grayPtr[y * size_x + x] = (unsigned char) g; //must typecast it
			}
		}
	}
	else if (type == "P5") //pgm binary
	{
		grayPtr = new unsigned char[size_x*size_y]; //allocate space. 
		in.ignore();
		for (int y = 0; y < size_y; y++)
		{
			for (int x = 0; x < size_x; x++)
			{
				grayPtr[y * size_x + x] = in.get(); //just get the unsigned char. Already in the right format.
			}
		}
	}
	else if (type == "P3") //ppm
	{
		type = "P6"; //change to binary header
		rrPtr = new unsigned char[size_x*size_y]; //allocate space for red, green, and blue arrays
		ggPtr = new unsigned char[size_x*size_y];
		bbPtr = new unsigned char[size_x*size_y];
		in.ignore();
		int r, g, b;  //r for red, g for green, b for blue. 
		for (int y = 0; y < size_y; y++)
		{
			for (int x = 0; x < size_x; x++)
			{
				in >> r >> g >> b;				 //scan in as integer values
				rrPtr[y * size_x + x] = (unsigned char) r;	//typecast as unsigned chars
				ggPtr[y * size_x + x] = (unsigned char) g;
				bbPtr[y * size_x + x] = (unsigned char) b;
			}
		}
	}
	else //P6, ppm binary
	{
		rrPtr = new unsigned char[size_x*size_y];  //allocate space
		ggPtr = new unsigned char[size_x*size_y];
		bbPtr = new unsigned char[size_x*size_y];
		in.ignore();

		for (int y = 0; y < size_y; y++)
		{
			for (int x = 0; x < size_x; x++)
			{
				rrPtr[y * size_x + x] = in.get();	//already unsigned chars, so just get them
				ggPtr[y * size_x + x] = in.get();				
				bbPtr[y * size_x + x] = in.get();				
			}
		}

	}
	in.close(); //close the file
	return;
} // read()


// Convert current image to PGM format.
void Image::ppm2pgm(void) 
{
	if (type == "P6") //otherwise it is already a pgm file and we just return.
	{
		grayPtr = new unsigned char[size_x * size_y]; //allocate space for the array to hold converted gray values
		for (int index = 0; index < (size_x * size_y); index++)  
		{
			grayPtr[index] = 0.21 * rrPtr[index] + 0.71 * ggPtr[index] + 0.08 * bbPtr[index]; //use the equation to convert rgb values to grayscale values
		}
		type = "P5"; //it is now a .pgm file, so must change type.
		delete [] rrPtr; //no more need for rgb arrays. 
		delete [] ggPtr;
		delete [] bbPtr;
	}	

	return;
} // ppm2pgm()


void Image::write(string filename, bool plain) const 
{

	ofstream out(filename.c_str(), ios_base::binary); //open file for writing.
	if (!out)  //check if it is open
	{
		cerr << "Error opening " << filename << endl;
		return;
	}
	if (type == "P6" && !plain) //ppm binary file
	{
		out << "P6" << '\n';
		out << size_x << ' ' << size_y << '\n' << max << '\n'; //writing the header
		for (int i = 0; i < (size_x * size_y); i++)
		{
			out << rrPtr[i] << ggPtr[i] << bbPtr[i];		//already in binary format, just write it out.	
		}
	}
	else if (type == "P6" && plain) //ppm plain text file
	{
		out << "P3" << '\n';
		out << size_x << ' ' << size_y << '\n' << max << '\n';
		for (int i = 0; i < (size_x * size_y); i++)
		{
			out << (int)rrPtr[i] << " " << (int)ggPtr[i] << " " << (int)bbPtr[i] << " "; //must typecast as integers, because plain text file.
		}
	}
	else if (type == "P5" && !plain) //pgm binary file
	{
		out << "P5" << '\n';
		out << size_x << ' ' << size_y << '\n' << max << '\n';
		for (int i = 0; i < (size_x * size_y); i++)
		{
			out << grayPtr[i];			//.pgm so gray values only, already is an unsigned char, so just write.
		}
	}
	else //pgm plain file
	{
		out << "P2" << '\n';
		out << size_x << ' ' << size_y << '\n' << max << '\n';
		for (int i = 0; i < (size_x * size_y); i++)
		{
			out << (int)grayPtr[i] << " ";	//plain text, so must typecast. 
		}
	}
	out.close(); //close the file
	return;
} // write()


// Encode string into image data.
void Image::encode_string(string str, int byte_offset) 
{
	if (type == " ") //make sure we actually have a file!
	{
		cerr << "No data in image. Need to read() a file first." << endl;
		return;
	}

	srand(time(NULL)); //keep it random

	str = start_code + str; //append start code to front of string.
	//Append
	str.push_back('\0'); //add a null character, good for decoding. 

	int pixel_index = 8 * byte_offset; //1 char is 1 byte, spread over 8 pixels
	int want_odd;

	//encode given text.
	for (string::size_type i = 0; (i < str.length() && pixel_index < size_x * size_y); i++) //go through whole string to encode, don't write past end of file.
	{
		int mask = 128; //start at 128 to get first binary digit
		for (int j = 0; (j < 8 && pixel_index < size_x * size_y); j++) //set each bit value, don't write past end of file
		{
			want_odd = (int) str.at(i) & mask; //non-zero value if we want a 1 (odd), 0 if we want a 0 (even)
			if (want_odd) //encoding a one!
			{
				if (type == "P5") //.pgm file
				{
					set_pgm_pixel_odd(pixel_index); //if we want a one, make it odd. 
				}
				else //type == "P6"
				{
					set_ppm_pixel_odd(pixel_index);
				}
			}
			else //we want an even, so we encode a zero!
			{
				if (type == "P5") //.pgm
				{
					set_pgm_pixel_even(pixel_index); //if we want a zero, make it even.
				}
				else //type == "P6", .ppm
				{
					set_ppm_pixel_even(pixel_index);
				}
			}
			mask >>= 1; //shift so we can get next bit.
			pixel_index++; //move to next pixel
		}
	}
	if (pixel_index >= (size_x * size_y - 1)) //there isn't room for even one char!
	{
		cout << "Error: byte offset too large to encode entire message. Entire message was not encoded." << endl;
	}
	return;
} // encode_string()



string Image::decode_string(void) const {
	unsigned char c;
	int mask, found = 0;
	int pixel_index = 0;
	string text (""), check("");

	while (!found && pixel_index < (size_x * size_y)) //finding the start code.
	{
		for (int i = 0; i < 4; i++) //cycle through four bytes to correspond to four chars in the start code
		{
			c = '\0';
			mask = 128;
			for (int bit_count = 0; (bit_count < 8) && pixel_index < (size_x * size_y); bit_count++)
			{
				if (type == "P5")
				{
					c += mask * (grayPtr[pixel_index] % 2);
				}
				else
				{
					c += mask * (((int) rrPtr[pixel_index] + (int) ggPtr[pixel_index] + (int) bbPtr[pixel_index]) % 2);
				}
				mask >>= 1;
				pixel_index++;
			}
			check.push_back(c); //pushing back each new char we obtain so we can compare to start_code
		}
		if (pixel_index >= (size_x * size_y - 1)) //there isn't room for even one char. No message.
		{
			return "Error: no encoded message.";
		}
		if (check == start_code)
		{
			break; //we found the start code, and are at the correct pixel index to begin decoding.
		}
		pixel_index -= 24; //decrement by three bytes to check the next potential set of four bytes. 
		check.clear(); //clear check so we can check the next four bytes. 
	}

	while (pixel_index < (size_x * size_y))
	{
		c = '\0';
		mask = 128; //128 so we can get 8 binary digits
		for (int bit_count = 0; bit_count < 8; bit_count++)
		{
			if (type == "P5") //.pgm
			{
				c += mask * (grayPtr[pixel_index] % 2); //see if it is odd or even.
			}
			else
			{
				c += mask * (((int) rrPtr[pixel_index] + (int) ggPtr[pixel_index] + (int) bbPtr[pixel_index]) % 2); //.ppm
			}
			mask >>= 1; //shift for next bit
			pixel_index++; //increment
		}
		if (c == '\0')
		{
			break; //at the end of the string
		}
		text.push_back(c); //add to our decoded string
	}
	return text; //return the decoded string.
} // decode_string()

const Image &Image::operator=(Image const &image) { //overloaded assignmnent operator
	if (grayPtr) { //start from scratch. Delete gray pixel data if it exists.
		delete [] grayPtr;
		grayPtr = 0; //set to NULL
	}
	if (rrPtr) { //it must be a .ppm file. Delete all three rgb value arrays.
		delete [] rrPtr;
		delete [] ggPtr;
		delete [] bbPtr;
		rrPtr = 0; //set to null
		ggPtr = 0;
		bbPtr = 0;
	}
	type = image.type; //copy memory that was not dynamically allocated
	filename = image.filename;
	rrThreshold = image.rrThreshold;
	ggThreshold = image.ggThreshold;
	bbThreshold = image.bbThreshold;
	size_x = image.size_x;
	size_y = image.size_y;
	max = image.max;
	if (type == "P5") { //.pgm file, so need to copy grayPtr array
		grayPtr = new unsigned char [size_x*size_y]; //allocate space
		for (int i = 0; i < (size_x*size_y); i++) {
			grayPtr[i] = image.grayPtr[i];
		}
	}
	else { //type must be P6, and so it is a .ppm file and we need rrPtr, ggPtr, bbPtr.
		rrPtr = new unsigned char [size_x*size_y];
		ggPtr = new unsigned char [size_x*size_y];
		bbPtr = new unsigned char [size_x*size_y];
		for (int i = 0; i < (size_x*size_y); i++) {
			rrPtr[i] = image.rrPtr[i];
			ggPtr[i] = image.ggPtr[i];
			bbPtr[i] = image.bbPtr[i];
		}
	}
	return *this;
}

Image Image::operator+(Image const &image){ //overloaded addition operator. Adding two images together with greenscreening
	if (type == "P5" || image.type == "P5") { //both files are .pgm files. Error!
		cerr << "Error: both files are not .ppm files" <<endl;
		return *this;
	}
	else if (size_x != image.size_x && size_y != image.size_y) { //not the same size. This is an error.
		cerr << "Error: both images are not of the same size" <<endl;
		return *this;
	}
	else if (max != image.max) { //not the same max value. Warning
		cerr << "Warning: max intensity values are not the same" << endl;
		if (max < image.max) { //now we take the highest max value. If max is greater than image.max, do nothing. 
			max = image.max;
		}
	}
	Image *temp = new Image(*this); //making a temporary image with *this passed to the copy constructor

	for (int i = 0; i < (size_x*size_y); i++) { //now we add the images, pixel by pixel.
		if ((rrPtr[i] < rrThreshold) && (ggPtr[i] > ggThreshold) && (bbPtr[i] < ggThreshold)) { //if the pixel in the first image is above green threshold
			temp->rrPtr[i] = image.rrPtr[i]; //make that pixel be from image 2.
			temp->ggPtr[i] = image.ggPtr[i];
			temp->bbPtr[i] = image.bbPtr[i];
		}
	}
	return *temp; //return the temporary image
}

string Image::get_filename(void) const{
	return filename;
}

string Image::get_type(void) const {
	return type;
} // get_type()

int Image::get_size_x(void) const {
	return size_x;
} // get_size_x()

int Image::get_size_y(void) const {
	return size_y;
} // get_size_y()

int Image::get_max(void) const {
	return max;
} // get_max()

int Image::get_rrThreshold(void){ //just getters and setters 
	return rrThreshold;
}

int Image::get_ggThreshold(void) {
	return ggThreshold;
}

int Image::get_bbThreshold(void) {
	return bbThreshold;
}

void Image::set_rrThreshold(int the_rrThreshold) {
	rrThreshold = the_rrThreshold;
}

void Image::set_ggThreshold(int the_ggThreshold) {
	ggThreshold = the_ggThreshold;
}

void Image::set_bbThreshold(int the_bbThreshold) {
	bbThreshold = the_bbThreshold;
}

void Image::set_pgm_pixel_odd(int pixel_index) 
{
	// If pixel already odd, nothing to do
	if ((grayPtr[pixel_index] % 2) == 1)
		return;

	// If value isn't max, increment it by 1.
	if (grayPtr[pixel_index] != max)
		grayPtr[pixel_index]++;
	// If value is max, decrement it by 1.
	else 
		grayPtr[pixel_index]--;

	return;
} // set_pgm_pixel_odd()


void Image::set_pgm_pixel_even(int pixel_index) 
{
	// If pixel already even, nothing to do
	if ((grayPtr[pixel_index] % 2) == 0)
		return;

	// If value isn't max, increment it by 1.
	if (grayPtr[pixel_index] != max)
		grayPtr[pixel_index]++;
	// If value is max, decrement it by 1.
	else 
		grayPtr[pixel_index]--;

	return;
} // set_pgm_pixel_even()


void Image::set_ppm_pixel_odd(int pixel_index) {
	// If pixel already odd, nothing to do.
	if (((int) rrPtr[pixel_index] +
				(int) ggPtr[pixel_index] +
				(int) bbPtr[pixel_index]) % 2 == 1)
		return;

	// Choose, at random, either red, green, or blue to adjust.
	unsigned char* cPtr;
	unsigned char* cPtrs[3] = {rrPtr, ggPtr, bbPtr};
	cPtr = cPtrs[rand() % 3];

	// If value isn't 0, decrement it by 1.
	if (cPtr[pixel_index] != 0)
		cPtr[pixel_index]--;
	// If value is 0, increment it by 1
	else 
		cPtr[pixel_index]++;

	return;
} // set_ppm_pixel_odd()


void Image::set_ppm_pixel_even(int pixel_index) {
	// If pixel already even, nothing to do.
	if (((int) rrPtr[pixel_index] +
				(int) ggPtr[pixel_index] +
				(int) bbPtr[pixel_index]) % 2 == 0)
		return;

	// Choose, at random, either red, green, or blue to adjust.
	unsigned char* cPtr;
	unsigned char* cPtrs[3] = {rrPtr, ggPtr, bbPtr};
	cPtr = cPtrs[rand() % 3];

	// If value isn't 0, decrement it by 1.
	if (cPtr[pixel_index] != 0)
		cPtr[pixel_index]--;
	// If value is 0, increment it by 1
	else 
		cPtr[pixel_index]++;

	return;
}// set_ppm_pixel_even()
