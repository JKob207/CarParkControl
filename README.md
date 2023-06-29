# CarParkControl

The CarParkControl project is based on the implementation of computer vision for parking occupancy control. The project uses a drone camera shot above the car park imitating a live recording from a good quality camera installed at a high point in the car park. The program allows you to select parking spaces, save them to a file, load them from a file and (as the main function) monitor the parking lot by graphically showing which spaces are occupied and which are not.

## System Requirements
- Windows operating system (e.g., Windows 10)
- Visual Studio (compatible with MFC)
- OpenCV library (version 4.7.0 or higher)
- Microsoft Foundation Classes (MFC)

## Prerequisites
Before running the application, please ensure that you have the following prerequisites installed:

1. Visual Studio: Download and install Visual Studio from the official Microsoft website (https://visualstudio.microsoft.com).

2. OpenCV: Install OpenCV library on your machine. You can download the latest version from the OpenCV website (https://opencv.org). Make sure to configure the necessary environment variables to link OpenCV with your project.

## Setup
1. Clone the repository or download the source code as a ZIP file.
2. Launch Visual Studio and open the project file (.sln) located in the CarParkControl folder.
3. Configure Project Properties:
   - Make sure the project is set to use the correct version of Visual Studio.
   - Verify that the project dependencies and libraries are correctly linked, including OpenCV and MFC.
4. Build the Project:
   - Select the debug configuration.
   - Build the project using Visual Studio by clicking the "Build" or "Rebuild Solution" option.

## Running the Application
At the very beginning, the user should select one parking space to use as a template. Then, select all the parking spaces in the shot from the recording that interest you (If you make a mistake while selecting parking spaces, you can right-click to delete it). However, this step is not required every time. The selection of parking spaces can be easily saved to a txt file and saved to the computer's memory. With each launch of the application, previously selected parking spaces can be loaded. As a result the application allows you to monitor the occupancy of parking spaces. At the very beginning, each frame is processed separately. Such a frame is successively divided into interesting fragments, which are analyzed separately. If bright pixels prevail in a given fragment, it means that the car is in this parking space. The application marks the analyzed places in red if this place is occupied or in green if it is empty.

## Extra files
In main project folder you can find video example for testing and saved text file with all parking spaces in that file.

## Troubleshooting
If you are using your own videos remember that the camera recording must be recorded at the best right angle for the best effect. The solution does not work at night and in poorly lit parkings.

If you encounter any other issues while building or running the application, consider the following troubleshooting tips:
  - Ensure that all dependencies, including OpenCV and MFC, are correctly installed and configured.
  - Verify that the project settings, such as include paths and library dependencies, are properly configured in Visual Studio.
  - If you encounter specific error messages, refer to the troubleshooting section of the OpenCV documentation or search online forums for solutions.
