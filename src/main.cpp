#include <iostream>
#include <cstdint>
#include <fstream>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ascii luminance calculated by chungaloider on stackoverflow
std::string ascii = "`.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
float lumens[] = {0, 0.0751, 0.0829, 0.0848, 0.1227, 0.1403, 0.1559, 0.185, 0.2183, 0.2417, 0.2571, 0.2852, 0.2902, 0.2919, 0.3099, 0.3192, 0.3232, 0.3294, 0.3384, 0.3609, 0.3619, 0.3667, 0.3737, 0.3747, 0.3838, 0.3921, 0.396, 0.3984, 0.3993, 0.4075, 0.4091, 0.4101, 0.42, 0.423, 0.4247, 0.4274, 0.4293, 0.4328, 0.4382, 0.4385, 0.442, 0.4473, 0.4477, 0.4503, 0.4562, 0.458, 0.461, 0.4638, 0.4667, 0.4686, 0.4693, 0.4703, 0.4833, 0.4881, 0.4944, 0.4953, 0.4992, 0.5509, 0.5567, 0.5569, 0.5591, 0.5602, 0.5602, 0.565, 0.5776, 0.5777, 0.5818, 0.587, 0.5972, 0.5999, 0.6043, 0.6049, 0.6093, 0.6099, 0.6465, 0.6561, 0.6595, 0.6631, 0.6714, 0.6759, 0.6809, 0.6816, 0.6925, 0.7039, 0.7086, 0.7235, 0.7302, 0.7332, 0.7602, 0.7834, 0.8037, 0.9999};

char ascii_from_luma(float luma) {
	int start = 0;
	int end = 91;
	for(int i = end >> 1; i != end && i != start; ) {
		if(luma < lumens[i]) {
			end = i;
		}
		else {
			start = i;
		}

		i = (end + start) >> 1;
	}

	return ascii[(start + end) >> 1];
}

float rgb_to_luma(uint32_t pixel) {
	float r = (pixel & 0xFF) / 255.f;
	float g = ((pixel >> 8) & 0xFF) / 255.f;
	float b = ((pixel >> 16) & 0xFF) / 255.f;

	return (0.2126*r + 0.7152*g + 0.0722*b);
}

struct cmd_flags_parser {
	char** begin;
	char** end;

	struct flags {
		// -w : ascii image width ( < image width)
		// -h : ascii image height ( < image height)
		// -inv : invert image luma
		// -gc : gamma correction of ascii luma
		// -par : preserve aspect ratio
		// -car : character width to height ratio
		// -in : input image filename
		// -out : output ascii filename
		
		int width;
		int height;
		bool invert_luma;
		float gamma_correction;
		bool preserve_aspect_ratio;
		float character_aspect_ratio;
		std::string input_filename;
		std::string output_filename;
		bool help;
	};
	
	cmd_flags_parser(int argc, char** argv) : begin(argv), end(argv + argc) {}

	bool flag_present(std::string flag) {
		return std::find(begin, end, flag) != end;
	}

	char* flag_value(std::string flag) {
		char **it = std::find(begin, end, flag);
		if(it != end) {
			if(++it != end) {
				return *it;
			}
			std::cout << "ERROR: Value for flag [" << flag << "] not provided." << std::endl;
		
			return 0;
		}

		return nullptr;
	}

	flags parse_flags() {
		flags cmd_flags = {};
		cmd_flags.width = 80;
		cmd_flags.height = 40;
		cmd_flags.invert_luma = false;
		cmd_flags.gamma_correction = 1.f;
		cmd_flags.preserve_aspect_ratio = false;
		cmd_flags.character_aspect_ratio = 0.5f;
		cmd_flags.input_filename = "input.png";
		cmd_flags.output_filename = "output.txt";
		cmd_flags.help = false;

		char *value_result = nullptr;
		bool present_result = false;
		
		value_result = flag_value("-w");
		if(value_result != nullptr) cmd_flags.width = atoi(value_result);

		value_result = flag_value("-h");
		if(value_result != nullptr) cmd_flags.height = atoi(value_result);

		present_result = flag_present("-inv");
		if(present_result) cmd_flags.invert_luma = true;

		value_result = flag_value("-gc");
		if(value_result != nullptr) cmd_flags.gamma_correction = atof(value_result);

		present_result = flag_present("-par");
		if(present_result) cmd_flags.preserve_aspect_ratio = true;

		value_result = flag_value("-car");
		if(value_result != nullptr) cmd_flags.character_aspect_ratio = atof(value_result);

		value_result = flag_value("-in");
		if(value_result != nullptr) cmd_flags.input_filename = value_result;

		value_result = flag_value("-out");
		if(value_result != nullptr) cmd_flags.output_filename = value_result;

		present_result = flag_present("-help");
		if(present_result) cmd_flags.help = true;
		
		return cmd_flags;
	}
};

// TODO: Handle vertical and horizontal artifacts.
int main(int argc, char **argv) {
	cmd_flags_parser parser(argc, argv);
	cmd_flags_parser::flags cmd_flags = parser.parse_flags();

	std::cout << "----------FLAGS---------- " << std::endl;
	std::cout << "ASCII Image Width _______ " << cmd_flags.width << std::endl;
	std::cout << "ASCII Image Height ______ " << cmd_flags.height << std::endl;
	std::cout << "Invert Luminosity _______ " << cmd_flags.invert_luma << std::endl;
	std::cout << "Gamma Correction ________ " << cmd_flags.gamma_correction << std::endl;
	std::cout << "Preserve Aspect Ratio ___ " << cmd_flags.preserve_aspect_ratio << std::endl;
	std::cout << "Character Aspect Ratio __ " << cmd_flags.character_aspect_ratio << std::endl;
	std::cout << "Input Filename __________ " << cmd_flags.input_filename << std::endl;
	std::cout << "Output Filename _________ " << cmd_flags.output_filename << std::endl;

	if(cmd_flags.help) {
		std::cout << std::endl;
		std::cout << "-w <positive integer number> ____ Width in characters for resulting ASCII image." << std::endl;
		std::cout << "                                  Default value is 80." << std::endl;
		std::cout << "-h <positive integer number> ____ Height in characters for resulting ASCII image." << std::endl;
		std::cout << "                                  Default value is 40." << std::endl;
		std::cout << "-inv ____________________________ Inverts image luminosity values. Useful when" << std::endl;
		std::cout << "                                  object of interest is darker than the background." << std::endl;
		std::cout << "                                  By default, this flag is turned off." << std::endl;
		std::cout << "-gc <positive decimal number> ___ Gamma correction done by using powers of luminosity." << std::endl;
		std::cout << "                                  Values between 0 and 1 increase luminosity. Values" << std::endl;
		std::cout << "                                  higher than 1 decrease it." << std::endl;
		std::cout << "                                  If inversion is used ('-inv' flag), then impact of" << std::endl;
		std::cout << "                                  these values is inverted." << std::endl;
		std::cout << "                                  By default, value is 1.0." << std::endl;
		std::cout << "-par ____________________________ Preserves aspect ratio of original image by looking" << std::endl;
		std::cout << "                                  at defined width ('-w' flag)." << std::endl;
		std::cout << "                                  If this flag is used, then '-h' flag has no impact." << std::endl;
		std::cout << "                                  By default, this flag is turned off." << std::endl;
		std::cout << "-car <positive decimal number> __ This is used to inform program about aspect ratio" << std::endl;
		std::cout << "                                  of characters on given machine." << std::endl;
		std::cout << "                                  By default, value is 0.5, meaning that character" << std::endl;
		std::cout << "                                  width is 0.5 * character height." << std::endl;
		std::cout << "-in <string> ____________________ Defines input image name. By default, program will" << std::endl;
		std::cout << "                                  look for image named 'input.png'." << std::endl;
		std::cout << "-out <string> ___________________ Defines output text file name. By default, program" << std::endl;
		std::cout << "                                  will use 'output.txt' name." << std::endl;
		std::cout << "-help ___________________________ Prints this help manual and exits the program." << std::endl;
		std::cout << std::endl;

		return 0;
	}

	int image_width, image_height, image_num_channels;
	uint32_t *data = (uint32_t *)stbi_load(cmd_flags.input_filename.c_str(), &image_width, &image_height, &image_num_channels, 4);

	if(cmd_flags.preserve_aspect_ratio) {
		cmd_flags.height = static_cast<int>(cmd_flags.width * (image_height / static_cast<float>(image_width)) * cmd_flags.character_aspect_ratio);
	}
	else if(cmd_flags.height != 0) {
		cmd_flags.height *= cmd_flags.character_aspect_ratio;
	}
	
	int ascii_width_in_pixels = static_cast<int>(image_width / static_cast<float>(cmd_flags.width));
	int ascii_height_in_pixels = static_cast<int>(image_height / static_cast<float>(cmd_flags.height));
	
	float* ascii_lumens = new float[cmd_flags.width * cmd_flags.height]{};

	std::cout << "----------PARAMS---------" << std::endl;
	std::cout << "Image Width _____________ " << image_width << std::endl;
	std::cout << "Image Height ____________ " << image_height << std::endl;
	std::cout << "Channels ________________ " << image_num_channels << std::endl;
	std::cout << "ASCII Image Width _______ " << cmd_flags.width << std::endl;
	std::cout << "ASCII Image Height ______ " << cmd_flags.height << std::endl;
	std::cout << "ASCII Char Width: _______ " << ascii_width_in_pixels << std::endl;
	std::cout << "ASCII Char Height: ______ " << ascii_height_in_pixels << std::endl;

	for(int y = 0; y < image_height; ++y) {
		for(int x = 0; x < image_width; ++x) {
			float luma = rgb_to_luma(data[y * image_width + x]);
			
			int ascii_y = static_cast<int>((y / static_cast<float>(image_height)) * cmd_flags.height);
			int ascii_x = static_cast<int>((x / static_cast<float>(image_width)) * cmd_flags.width);
			
			ascii_lumens[ascii_y * cmd_flags.width + ascii_x] += (luma / (ascii_width_in_pixels * ascii_height_in_pixels));
		}
	}

	std::ofstream ascii_out;
	ascii_out.open(cmd_flags.output_filename);

	for(int y = 0; y < cmd_flags.height; ++y) {
		for(int x = 0; x < cmd_flags.width; ++x) {
			float gamma_corrected = std::pow(ascii_lumens[y * cmd_flags.width + x], cmd_flags.gamma_correction);
			if(cmd_flags.invert_luma)
				ascii_out << ascii_from_luma(1 - gamma_corrected);
			else
				ascii_out << ascii_from_luma(gamma_corrected);
		}
		ascii_out << std::endl;
	}
	
	return 0;
}
