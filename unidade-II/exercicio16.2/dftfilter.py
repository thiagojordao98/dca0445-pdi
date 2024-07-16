import cv2
import numpy as np
import sys

def swap_quadrants(image):
    # If the image has an odd size, crop the region to the largest
    # even size possible (-2 = 1111...1110)
    image = image[0:image.shape[0] & -2, 0:image.shape[1] & -2]

    center_x = image.shape[1] // 2
    center_y = image.shape[0] // 2

    # Rearrange the quadrants of the Fourier transform so that the
    # origin is at the center of the image
    # A B   ->  D C
    # C D       B A
    A = image[0:center_y, 0:center_x]
    B = image[0:center_y, center_x:image.shape[1]]
    C = image[center_y:image.shape[0], 0:center_x]
    D = image[center_y:image.shape[0], center_x:image.shape[1]]

    # Swap quadrants (Top-Left with Bottom-Right)
    image[0:center_y, 0:center_x] = D
    image[center_y:image.shape[0], center_x:image.shape[1]] = A

    # Swap quadrant (Top-Right with Bottom-Left)
    image[0:center_y, center_x:image.shape[1]] = C
    image[center_y:image.shape[0], 0:center_x] = B

def make_homomorphic_filter(image, gamma_l, gamma_h, d0, c):
    filter_2d = np.zeros_like(image, dtype=np.float32)
    center_x = image.shape[1] // 2
    center_y = image.shape[0] // 2

    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            d = np.sqrt((i - center_y) ** 2 + (j - center_x) ** 2)
            h = (gamma_h - gamma_l) * (1.0 - np.exp(-c * (d ** 2) / (d0 ** 2))) + gamma_l
            filter_2d[i, j] = h

    filter_complex = np.dstack((filter_2d, np.zeros_like(filter_2d)))
    return filter_complex


if __name__ == "__main__":
    image = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
    if image is None:
        print("Error opening image", sys.argv[1])

    # Expand the input image to the optimal size for the DFT
    dft_m = cv2.getOptimalDFTSize(image.shape[0])
    dft_n = cv2.getOptimalDFTSize(image.shape[1])
    padded = cv2.copyMakeBorder(image, 0, dft_m - image.shape[0], 0, dft_n - image.shape[1], cv2.BORDER_CONSTANT, value=0)

    # Prepare the complex matrix to be filled
    # First the real part, containing the input image
    # Then the imaginary part with null values
    complex_image = np.zeros((padded.shape[0], padded.shape[1], 2), dtype=np.float32)
    complex_image[:, :, 0] = padded.astype(np.float32)

    # Calculate the DFT
    cv2.dft(complex_image, complex_image)
    swap_quadrants(complex_image[:, :, 0])

    # Create the homomorphic filter and apply the frequency filtering
    gamma_l, gamma_h, d0, c = 0.5, 2.0, 30.0, 1.0
    filter_complex = make_homomorphic_filter(complex_image[:, :, 0], gamma_l, gamma_h, d0, c)
    cv2.mulSpectrums(complex_image, filter_complex, complex_image, 0)

    # Calculate the inverse DFT
    swap_quadrants(complex_image[:, :, 0])
    cv2.idft(complex_image, complex_image)

    # Extract the real part of the filtered image
    result = complex_image[:, :, 0]

    # Normalize the real part for display
    cv2.normalize(result, result, 0, 1, cv2.NORM_MINMAX)

    cv2.imshow("image", result)
    cv2.imwrite("image-with-irregular-illumination.png", result * 255)
    cv2.waitKey()

