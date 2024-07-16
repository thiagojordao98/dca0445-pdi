import cv2
import numpy as np
import matplotlib.pyplot as plt

def swap_quadrants(image):
    cx, cy = image.shape[1] // 2, image.shape[0] // 2
    image[:cy, :cx], image[cy:, cx:] = image[cy:, cx:].copy(), image[:cy, :cx].copy()
    image[:cy, cx:], image[cy:, :cx] = image[cy:, :cx].copy(), image[:cy, cx:].copy()

def main():
    fs = cv2.FileStorage("./senoide-256.yml", cv2.FILE_STORAGE_READ)
    image = fs.getNode("mat").mat()
    fs.release()

    complex_image = cv2.dft(image, flags=cv2.DFT_COMPLEX_OUTPUT)
    swap_quadrants(complex_image)

    planes = cv2.split(complex_image)
    magnitude = cv2.magnitude(*planes)
    magnitude += 1
    cv2.log(magnitude, magnitude)
    cv2.normalize(magnitude, magnitude, 0, 1, cv2.NORM_MINMAX)

    cv2.imshow("Original Image", image.astype(np.uint8))
    cv2.imshow("Magnitude Spectrum", magnitude)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
