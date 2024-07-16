import cv2
import sys

# Initial slider values
top_slider = 10
top_slider_max = 200

# Load the image
image = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
border = None

# Callback function for trackbar
def on_trackbar_canny(val):
    global border
    border = cv2.Canny(image, val, 3*val)
    cv2.imshow("Canny", border)

# Create window and trackbar
cv2.namedWindow("Canny", 1)
cv2.createTrackbar("Threshold inferior", "Canny", top_slider, top_slider_max, on_trackbar_canny)

# Initialize the canny display
on_trackbar_canny(top_slider)

# Wait for a key press and save the result
cv2.waitKey(0)
cv2.imwrite("cannyborders.png", border)