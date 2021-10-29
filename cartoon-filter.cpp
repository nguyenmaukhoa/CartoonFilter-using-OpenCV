// Include libraries
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;
using namespace cv;

// Apply pencil-sketched filter
Mat pencilSketch(Mat image, int arguments=0){
    
    /// YOUR CODE HERE
    Mat pencilSketchImage, gray, blur, laplacian;
    
    // Perform grayscale
    cvtColor(image, gray, COLOR_BGR2GRAY);
    
    // Perform blurring
    GaussianBlur(gray, blur, Size(3, 3), 0);
    
    // Perform laplacian
    Laplacian(blur, laplacian, -1, 5);
    
    // Inverse the image
    laplacian = 255 - laplacian;
    
    // Perform thresholding
    threshold(laplacian, pencilSketchImage, 150, 255, THRESH_BINARY);
    
    return pencilSketchImage;
}

// Apply cartoon filter
Mat cartoonify(Mat image, int arguments=0){
    
    Mat cartoonImage, edgePreservingImage, edgeImage;
    
    /// YOUR CODE HERE
    // Perform edge preserving
    edgePreservingFilter(image, edgePreservingImage, 2, 50, 0.4);
    
    // Match edge-preserved image and pencil-sketched image
    edgeImage = pencilSketch(image);
    bitwise_and(edgePreservingImage, edgePreservingImage, cartoonImage, edgeImage);
    
    return cartoonImage;
}

int main(){
	// Create a VideoCapture object, and read and existing video
	VideoCapture cap("../video/bird.mp4");

	// Create video write object to generate output video
 	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
  	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
	VideoWriter outmp4("../output/bird-cartoonified.mp4", VideoWriter::fourcc('F','M','P','4'), 24, Size(frame_width,frame_height));
	
	// Check the video is loaded sucessfully
	if(!cap.isOpened()){
		cout << "Error opening the video..." << endl;
		return -1;
	}

	// Display the video until ESC key is pressed
	while(cap.isOpened()){

		// Matrix to hold each video frame
		Mat frame, cartoonifiedFrame;

		// Copy the video frame-by-frame from cap to frame
		cap >> frame;

		// If the frame is empty, stop the dislay
		if(frame.empty())
			break;

		// Apply cartoon filter on each frame
		cartoonifiedFrame = cartoonify(frame);

		// Display the frames
		imshow("Frame", cartoonifiedFrame);

		// Write output video with QRCode text 
		outmp4.write(cartoonifiedFrame);

		// Stop when ESC key is pressed
		char c = (char)waitKey(5);
		if(c == 27)
			break;
	}
	// Release the objects when done
	cap.release();
  	outmp4.release();

	// Close all the windows
	destroyAllWindows();
	return 0;
}