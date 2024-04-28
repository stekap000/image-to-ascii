#include <iostream>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
	int width, height, num_channels;
	uint32_t *data = (uint32_t *)stbi_load("test.jpg", &width, &height, &num_channels, 0);

	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << "Channels: " << num_channels << std::endl;

	stbi_image_free(data);
	
	return 0;
}
