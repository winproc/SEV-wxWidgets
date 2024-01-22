


#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/notifmsg.h>
#include <wx/graphics.h>

#include "asio.hpp"

// Declarations

#include "Geometry.h"
#include "Gui.h"
#include "Source.h"

#include <fstream>

wxDEFINE_EVENT(OUTPUT_RECIEVED, wxThreadEvent);

void MainApp::HandleConnectionOutput(wxThreadEvent& Event) {
	wxMessageBox(Event.GetString(), "Debug");
}

wxThread::ExitCode PythonConnection::Entry() {
	while (!TestDestroy()) {

		asio::io_context context;
		asio::ip::tcp::resolver tcpResolver(context);

		asio::ip::tcp::socket socket(context);
		asio::connect(socket, tcpResolver.resolve("localhost", "6969"));
		
		try {
			for (;;) {
				char Buffer[1024] = { 0 };
				asio::error_code error;

				if (socket.read_some(asio::buffer(Buffer), error) > 0) {
					wxThreadEvent serverResponse(OUTPUT_RECIEVED);
					serverResponse.SetString(Buffer);

					wxTheApp->QueueEvent(&serverResponse);
				}

				if (error == asio::error::eof) {

					throw std::runtime_error("Connection closed abruptly");
					break;

				}
				else if (error) {
					throw std::system_error(error);
				}

			}
		}
		catch (std::exception& Error) {
			wxMessageBox(Error.what(), "Error");

			return (wxThread::ExitCode)0;
		}
		

	}

	return (wxThread::ExitCode)0;
}

void MainApp::StartConnectionThread() {

	PythonConnection* connection = new PythonConnection(this);

	if (connection->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox("Thread creation failed", "Error");

		delete connection;
		connection = NULL;
	}

}


void MainApp::ConnectToPython() {


	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char cmdLineEntry[512] = { 0 };
	GetFullPathNameA("Python\\Python312\\python.exe", sizeof(cmdLineEntry), cmdLineEntry, NULL);

	std::strcat(cmdLineEntry, " Server\\server.py");
	

	if (!CreateProcessA(
		NULL,
		cmdLineEntry,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	)) {
		wxMessageBox("Python invocation failed with system error code: "  + std::to_string(GetLastError()) + "\n See System Error Codes at learn.microsoft.com", "Error");
	}

	StartConnectionThread();

}



void MainApp::OnRMBClicked(const wxMouseEvent& Event) {
	wxWindow::FindWindowById(Event.GetId())->PopupMenu(toggleShortcutList);
}


void MainApp::OnMenuActivated(const wxCommandEvent& Event) {

	switch (Event.GetId()) {

	case ENABLE_SH_ENGINES:
	{
		for (EngineFrame* engineFrame : boosterEngines)
		{
			engineFrame->enabled = true;
			engineFrame->Refresh();

		}	

		break;
	}

	case DISABLE_SH_ENGINES:
	{
		for (EngineFrame* engineFrame : boosterEngines) 
		{
			engineFrame->enabled = false;
			engineFrame->Refresh();
		}
			
		break;
	}

	case 5:
	{
		ConnectToPython();
		break;
	}
	default:
	{
		wxMessageBox("Unintended behaviour; See error code " + std::to_string(GetLastError()), "Error");
	}

	}

}

EngineFrame::EngineFrame(int Radius, const wxPoint& Position, wxWindow* Parent, int Id) : wxButton()
{

	this->radius = Radius;
	this->position = Position;

	Create(Parent, Id, wxEmptyString, Position, wxSize(Radius, Radius), wxBU_NOTEXT || wxBORDER_NONE);

	//SetBackgroundColour(wxBLACK->GetAsString());

	Bind(wxEVT_PAINT, &EngineFrame::OnPaint, this);

}

void EngineFrame::Clear(wxGraphicsContext* GC) {
	
	GC->SetBrush(*wxBLACK_BRUSH);
	GC->DrawRectangle(0, 0, radius, radius);

}

void EngineFrame::Redraw() {
	wxPaintDC paintContext(this);
	wxGraphicsRenderer* d2dRenderer = wxGraphicsRenderer::GetDirect2DRenderer();

	wxGraphicsContext* winContext = d2dRenderer->CreateContextFromUnknownDC(paintContext);

	Clear(winContext);

	winContext->SetPen(wxPen(wxColour(255, 255, 255), 1, wxPENSTYLE_SOLID));

	if (enabled)
		winContext->SetBrush(wxBrush(*wxWHITE_BRUSH));
	else
		winContext->SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));

	winContext->DrawEllipse(0, 0, radius - 1, radius - 1);

	winContext->Flush();

	delete winContext;
}

void EngineFrame::OnPaint(const wxPaintEvent& Event) { Redraw(); };

EngineFrame* EngineFrame::FindFrameByNametag(std::string Nametag, const std::vector<EngineFrame*>& List) {

	for (EngineFrame* Frame : List) {

		if (Frame->kRPCNametag == Nametag) {
			return Frame;
		}

	}

	return NULL;
}

RenderWindow::RenderWindow(const std::string& Title) : wxFrame(NULL, 1, Title, wxPoint(100, 100), wxSize(1200, 600)) {
	
	SetBackgroundColour(wxBLACK->GetAsString());

}

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {

	

	RenderWindow* renderingFrame = new RenderWindow("Starship Engine Layout");
	wxPanel* controlUI = new wxPanel(renderingFrame);

	std::vector<wxPoint> innerRingEnginePositions = ComputePointsOnCircle(31, 3, 1);
	innerRingEnginePositions.pop_back(); // division by 3 resolves to a number slightly less than the loop limit, so the loop repeats one more time; could be fixed by dividing by 3.0f
	std::vector<wxPoint> middleRingEnginePositions = ComputePointsOnCircle(100, 10, 1);
	std::vector<wxPoint> outerRingEnginePositions = ComputePointsOnCircle(170, 20, 1);
	
	// Combine the above vector containers into a single list
	std::vector<wxPoint> enginePositions;
	enginePositions.insert(enginePositions.end(), innerRingEnginePositions.begin(), innerRingEnginePositions.end());
	enginePositions.insert(enginePositions.end(), middleRingEnginePositions.begin(), middleRingEnginePositions.end());
	enginePositions.insert(enginePositions.end(), outerRingEnginePositions.begin(), outerRingEnginePositions.end());

	for (const wxPoint& enginePosition : enginePositions) {
		boosterEngines.push_back(new EngineFrame(45, enginePosition + wxPoint(300, 250), controlUI, wxID_ANY));
	}

	// A ring (0th index): 3 engines
	// B ring (1st index): 10 engines
	// C ring (2nd index): 20 engines
	const std::vector<std::vector<int>> engineRingRange = { {0, 2}, {3, 12}, {13, 33} };
	
	auto GetRangeFromIndex = [engineRingRange](int index) {
		for (int i = 0; i < engineRingRange.size(); i++) {
			if ((engineRingRange[i][0] <= index) && (engineRingRange[i][1] >= index)) {
				return i;
			}
			else continue;
		}

		return 0;
	};


	short currentRing = GetRangeFromIndex(0);
	
	// Generate kRPC nametags
	for (int i = 0; i < boosterEngines.size(); i++) {
		boosterEngines.at(i)->kRPCNametag = 'A' + GetRangeFromIndex(i);
		boosterEngines.at(i)->kRPCNametag.append(",");

		boosterEngines.at(i)->kRPCNametag.append(std::to_string(i - engineRingRange[GetRangeFromIndex(i)][0] + 1));
	}

	toggleShortcutList = new wxMenu();
	toggleShortcutList->Append(1, "Enable SH Engines");
	toggleShortcutList->Append(2, "Disable SH Engines");

	toggleShortcutList->Append(3, "Enable SS Engines");
	toggleShortcutList->Append(4, "Disable SS Engines");

	toggleShortcutList->Append(5, "Reconnect to kRPC Python Client");

	Bind(wxEVT_RIGHT_UP, &MainApp::OnRMBClicked, this);
	Bind(wxEVT_MENU, &MainApp::OnMenuActivated, this);
	Bind(OUTPUT_RECIEVED, &MainApp::HandleConnectionOutput, this);

	renderingFrame->Show();

	ConnectToPython();
	
	return true;
}





