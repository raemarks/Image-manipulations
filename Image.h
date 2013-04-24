// Image.h
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>

using std::string;

class Image {
public:
  Image();                     // Default constructor.
  Image(string the_filename);  // Constructor reads data from given file.
  Image(const Image &image);   // Copy constructor.
  ~Image();                    // Destructor.

  void read(string the_filename); // Read image data from given file.
  void ppm2pgm(void);             // Convert image to PGM format.
  // Write image to a file.
  void write(string filename_out, bool plain=false) const;

  // Embed the given string into the image.
  void encode_string(string str, int byte_offset=0);
  // Decode an embedded string from the image.
  string decode_string(void) const;

  const Image &operator=(Image const &image);
  Image operator+(Image const &image);

  string get_filename(void) const;
  string get_type(void) const;
  int get_size_x(void) const;
  int get_size_y(void) const;
  int get_max(void) const;

  static int get_rrThreshold(void);
  static int get_ggThreshold(void);
  static int get_bbThreshold(void);
  static void set_rrThreshold(int the_rrThreshold);
  static void set_ggThreshold(int the_ggThreshold);
  static void set_bbThreshold(int the_bbThreshold);

private:
  // Values for green-screen thresholding.
  static int rrThreshold;  
  static int ggThreshold;
  static int bbThreshold;

  unsigned char* rrPtr;    // Red values.
  unsigned char* ggPtr;    // Green values.
  unsigned char* bbPtr;    // Blue values.
  unsigned char* grayPtr;  // Grayscale values.
  string filename;         // File name of original source file.
  string type;             // File type: "P5" or "P6"
  int size_x, size_y;      // Number of pixels in x and y directions.
  int max;                 // Maximum color/grayscale value.

  // Functions set a pixel value to odd or even when embedding a string.
  void set_pgm_pixel_even(int pixel_count);
  void set_pgm_pixel_odd(int pixel_count);

  void set_ppm_pixel_even(int pixel_count);
  void set_ppm_pixel_odd(int pixel_count);

}; // Image

#endif // __IMAGE_H__
