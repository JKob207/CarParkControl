
// CarParkMonitorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CarParkMonitor.h"
#include "CarParkMonitorDlg.h"
#include "afxdialogex.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

Mat frame, image, carParkSpots, carParkPlace, grayFrame, blurFrame, thresholdFrame, medianFrame, dilateFrame; // Main and side mats for capturing the frames, single frame, all car park spots, single car park place and various mats for processing the image
VideoCapture cap; // Video capture variable
bool videoCapture, selectingSpots, selectingOnePlace; // Boolean varaiables controlling selecting and capturing activities
vector<Rect> parkingSpots; // Vector of rects for each selected car park places
int carPlaces, carPlacesTaken; // Int variables for counting car park places
int placeWidth, placeHeight; // Width and height of car park place

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCarParkMonitorDlg dialog



CCarParkMonitorDlg::CCarParkMonitorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CARPARKMONITOR_DIALOG, pParent)
	, m_VideoName(_T(""))
	, m_FilePath(_T(""))
	, m_FileToLoad(_T(""))
	, m_SavingName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCarParkMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VIDEONAME, m_VideoName);
	DDX_Control(pDX, IDC_CHECKSAVEFILE, m_SaveToFile);
	DDX_Text(pDX, IDC_EDIT1, m_FilePath);
	DDX_Text(pDX, IDC_FILETOLOAD, m_FileToLoad);
	DDX_Text(pDX, IDC_NAMEOFFILETOSAVE, m_SavingName);
}

BEGIN_MESSAGE_MAP(CCarParkMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTVIDEO, &CCarParkMonitorDlg::OnBnClickedSelectvideo)
	ON_BN_CLICKED(IDC_SELECTSPOTS, &CCarParkMonitorDlg::OnBnClickedSelectspots)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SELECTFILE, &CCarParkMonitorDlg::OnBnClickedSelectfile)
	ON_BN_CLICKED(IDC_SELECTFILETOLOAD, &CCarParkMonitorDlg::OnBnClickedSelectfiletoload)
	ON_BN_CLICKED(IDOK, &CCarParkMonitorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SELECTONEPLACE, &CCarParkMonitorDlg::OnBnClickedSelectoneplace)
END_MESSAGE_MAP()


// CCarParkMonitorDlg message handlers

BOOL CCarParkMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	selectingSpots = false;
	carPlaces = 0;
	carPlacesTaken = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCarParkMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCarParkMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCarParkMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Selecting car park video file
void CCarParkMonitorDlg::OnBnClickedSelectvideo()
{
	UpdateData(TRUE);
	CFileDialog select(TRUE, NULL, m_VideoName);
	if (select.DoModal() == IDOK)
	{
		m_VideoName = select.GetPathName();
		UpdateData(FALSE);
	}
}

// Selecting car park spaces and saving it to vector and deleting it
void onMouse(int event, int x, int y, int flags, void* userdata)
{
	// Save car park place to vector when click left mouse button
	if (event == EVENT_LBUTTONDOWN)
	{
		cv::Rect newRect(x, y, placeWidth, placeHeight);
		parkingSpots.push_back(newRect);

	}
	else if (event == EVENT_RBUTTONDOWN) // Delete car park place from vector
	{
		int width = placeWidth;
		int height = placeHeight;
		for (auto it = parkingSpots.begin(); it != parkingSpots.end(); ++it)
		{
			int x1 = it->x;
			int y1 = it->y;

			if ((x1 < x) && (x < (x1 + width)) && (y1 < y) && (y < (y1 + height))) // If position x and y of saved car park places are within range of a possible marked spot delete it
			{
				carParkSpots = image.clone();
				parkingSpots.erase(it);
				break;
			}
		}
	}
}

// Saving all car park spots to txt file
void saveCarParkSpotsToFile(const std::vector<cv::Rect>& rectangles, const std::string& filePath, const std::string& fileName)
{
	std::string fullPath = filePath + "/" + fileName;
	std::ofstream file(fullPath);
	if (!file.is_open())
	{
		AfxMessageBox(_T("Failed to open the file for writing!"), MB_OK);
		return;
	}

	// Write the car spots to the file
	for (const auto& rect : rectangles)
	{
		file << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << std::endl;
	}

	file.close();

	AfxMessageBox(_T("Car places saved to file"), MB_OK);
}


// Selecting a car park places from first frame of video
void CCarParkMonitorDlg::OnBnClickedSelectspots()
{
	if (!selectingSpots)
	{
		std::string ss;
		ss = CW2A((LPCTSTR)m_VideoName);
		cap.open(ss);

		// Check if video is open
		if (!cap.isOpened())
		{
			AfxMessageBox(_T("Impossible to open a video"), MB_OK);
			return;
		}

		cap >> image;
		carParkSpots = image.clone();

		selectingSpots = true;
		GetDlgItem(IDC_SELECTSPOTS)->SetWindowTextW(_T("Stop selecting"));

		namedWindow("Select spots");
		setMouseCallback("Select spots", onMouse); // Set callback for clicking the buttons on mouse in the image
		imshow("Select spots", carParkSpots);

		while (selectingSpots)
		{
			if (waitKey(1) == 'q')
			{
				selectingSpots = false;
				GetDlgItem(IDC_SELECTSPOTS)->SetWindowTextW(_T("Select spots"));
				destroyWindow("Select spots");
				break;
			}

			for (const auto& rect : parkingSpots)
			{
				cv::rectangle(carParkSpots, rect, cv::Scalar(0, 255, 0), 2);
			}

			imshow("Select spots", carParkSpots);
		}
	}
	else
	{
		selectingSpots = false;
		GetDlgItem(IDC_SELECTSPOTS)->SetWindowTextW(_T("Select spots"));
		cap.release();

		// Save to file
		int checkState = m_SaveToFile.GetCheck();
		if (checkState == BST_CHECKED)
		{
			string stringFilePath, fileName;
			stringFilePath = CW2A((LPCTSTR)m_FilePath);
			
			CString fileNameCString;
			GetDlgItemText(IDC_NAMEOFFILETOSAVE, fileNameCString);
			fileName = CW2A((LPCTSTR)fileNameCString) + ".txt";
			if (fileName.empty())
			{
				AfxMessageBox(_T("Specify the name of the file to save"), MB_OK);
			}

			saveCarParkSpotsToFile(parkingSpots, stringFilePath, fileName);
		}

		return;
	}
}

// Selecting folder for saving the car park places file
void CCarParkMonitorDlg::OnBnClickedSelectfile()
{
	UpdateData(TRUE);
	CFolderPickerDialog dlg(_T("Choose folder"), NULL, this);
	if (dlg.DoModal() == IDOK)
	{
		m_FilePath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

// Load and return the car park places from given file
vector<cv::Rect> loadParkSpotsFromFile(const std::string& filePath)
{
	vector<cv::Rect> rectangles;
	ifstream file(filePath);
	if (!file.is_open())
	{
		AfxMessageBox(_T("Failed to open the file for reading!"), MB_OK);
		return rectangles;
	}

	// Read the car park places from the file
	int x, y, width, height;
	while (file >> x >> y >> width >> height)
	{
		rectangles.emplace_back(x, y, width, height);
	}

	file.close();

	return rectangles;
}


// Load car park places from file
void CCarParkMonitorDlg::OnBnClickedSelectfiletoload()
{
	UpdateData(TRUE);
	CFileDialog select(TRUE, NULL, m_FileToLoad);
	if (select.DoModal() == IDOK)
	{
		m_FileToLoad = select.GetPathName();

		if (parkingSpots.empty())
		{
			std::string stringFileName;
			stringFileName = CW2A((LPCTSTR)m_FileToLoad);
			parkingSpots = loadParkSpotsFromFile(stringFileName);
			AfxMessageBox(_T("Parking spots loaded sucessfully from file!"), MB_OK);
		}
		UpdateData(FALSE);
	}
}

// Play a car park video
void CCarParkMonitorDlg::OnBnClickedOk()
{
	if (!videoCapture)
	{
		if (parkingSpots.empty())
		{
			AfxMessageBox(_T("Load parkings spots first!"), MB_OK);
			return;
		}

		std::string ss;
		ss = CW2A((LPCTSTR)m_VideoName);
		cap.open(ss);

		if (!cap.isOpened())
		{
			AfxMessageBox(_T("Impossible to open a video"), MB_OK);
			return;
		}
		carPlaces = parkingSpots.size();
		SetTimer(1, (uint)(10000.0 / 60.0), NULL);
		GetDlgItem(IDOK)->SetWindowTextW(_T("Stop"));
		videoCapture = true;
	}
	else {
		KillTimer(1);
		GetDlgItem(IDOK)->SetWindowTextW(_T("Play"));
		videoCapture = false;
	}
}

// Function for checking if car park place is taken or free
void checkParkSpaces()
{
	for (const auto& rect : parkingSpots)
	{
		int x = rect.x;
		int y = rect.y;

		Mat croppedFrame = dilateFrame(rect);
		string videoTitle = to_string(x) + "." + to_string(y);

		int count = countNonZero(croppedFrame); // Checking non zero pixels in the cropped image
		int detectingCar = 800; // If that value is higher than 800 (limit value giving the best results) then there is a car in that place
		
		Scalar color;
		int thickness;
		if (count < 800) // free parking
		{
			color = Scalar(0, 255, 0);
			thickness = 5;
		}
		else // car in the spot
		{
			color = Scalar(0, 0, 255);
			thickness = 2;
			carPlacesTaken++;
		}
		
		rectangle(frame, rect, color, thickness);

		//imshow(videoTitle, croppedFrame);
	}
}

void CCarParkMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (cap.isOpened())
	{
		cap.read(frame);
		
		// Frame image processing
		cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
		GaussianBlur(grayFrame, blurFrame, Size(3, 3), 1);
		adaptiveThreshold(blurFrame, thresholdFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 25, 16);
		medianBlur(thresholdFrame, medianFrame, 5);
		dilate(medianFrame, dilateFrame, Mat(), Point(-1, -1), 1);

		checkParkSpaces();

		// Put a text to a frame
		string carPlacesResult = "Parking spaces taken: " + to_string(carPlacesTaken) + " / " + to_string(carPlaces);
		putText(frame, carPlacesResult, Point(50, 50), cv::FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 0, 0), 1, cv::LINE_AA);

		imshow("Car park video", frame);
		carPlacesTaken = 0;
		
		//imshow("Threshold", thresholdFrame);
	}

	CDialogEx::OnTimer(nIDEvent);
}

// Select one pattern cark park place
void CCarParkMonitorDlg::OnBnClickedSelectoneplace()
{
	std::string ss;
	ss = CW2A((LPCTSTR)m_VideoName);
	cap.open(ss);

	if (!cap.isOpened())
	{
		AfxMessageBox(_T("Impossible to open a video"), MB_OK);
		return;
	}

	cap >> image;
	carParkPlace = image.clone();

	imshow("Select space", carParkPlace);

	// Giving the user the opportunity to choose a template parking space
	Rect roi = selectROI("Select space", carParkPlace);

	// Save width and height from selected roi
	placeWidth = roi.width;
	placeHeight = roi.height;

	cap.release();
	destroyWindow("Select space");
	return;
}
