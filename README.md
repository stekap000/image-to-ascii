## image-to-ascii
Program for generating ASCII art from images. Allows specification of output image dimensions (with aspect ratio preservation), image brightness inversion and gamma correction.

### Compilation
Use compiler directly or use __make__.

### Command line arguments:
- __-w__ - ASCII image width (default __80__).
- __-h__ - ASCII image height (ignored if image aspect ratio is preserved) (default __40__).
- __-inv__ - Invert image luma values (default __false__).
- __-gc__ - Gamma correction. (default __1.0__)
- __-par__ - Preserve image aspect ratio (default __false__).
- __-car__ - Character aspect ratio (default __0.5__).
- __-in__ - Input image file name (default __input.png__).
- __-out__ - Output text file name (default __output.txt__).
- __-v__ - Prints verbose information about parameters used. (default __false__)
- __-help__ - Prints help manual. (default __false__)

### Examples:

<img src="https://github.com/stekap000/image-to-ascii/assets/80646042/fa57886e-013c-4e0d-8914-292546395187" width="800" height="auto">
<img src="https://github.com/stekap000/image-to-ascii/assets/80646042/9f3cd0eb-a48e-4555-915a-79df074c6880" width="800" height="auto">

#### Inversion (without gamma correction):
<img src="https://github.com/stekap000/image-to-ascii/assets/80646042/f68340cd-bcff-4b99-9023-8c656149e45e" width="397" height="auto">
<img src="https://github.com/stekap000/image-to-ascii/assets/80646042/ec5899ab-a83f-486d-9518-acc35ccc89f8" width="397" height="auto">
