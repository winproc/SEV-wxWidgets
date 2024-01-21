
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



void MainApp::ConnectToPython() {


	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char cmdLineEntry[512] = { 0 };
	GetFullPathNameA("Python\\Python312\\python.exe", sizeof(cmdLineEntry), cmdLineEntry, NULL);

	std::strcat(cmdLineEntry, " Server\\server.py");

	wxMessageBox(cmdLineEntry, "Test");

	if (!CreateProcessA(
		NULL,
		cmdLineEntry,
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi
	)) {
		wxMessageBox("Python invocation failed with system error code: "  + std::to_string(GetLastError()) + "\n See System Error Codes at learn.microsoft.com", "Error");
	}

}

/*
bool MainApp::RegisterEventCallbacks() {
	if (isConnectionInitialized) {

			krpc::services::KRPC krpc_server(&serverConnection);
			krpc::services::SpaceCenter spaceCenter(&serverConnection);

			krpc::services::SpaceCenter::Vessel Vessel;
			krpc::services::SpaceCenter::Parts Parts;

			Vessel = spaceCenter.active_vessel();
			Parts = Vessel.parts();
			
			bool debugIteration = true;
			for (EngineFrame* EngineUI : boosterEngines) {

				auto GetEngine = [&Parts, &EngineUI]() {
					
					auto EngineList = Parts.with_tag(EngineUI->kRPCNametag);

					if (EngineList.size() > 0) {
						return EngineList[0];
					}

					
				};

				krpc::services::SpaceCenter::Part Engine;
				Engine = GetEngine();

				auto engine = Engine.engine();

				auto thrust_call = engine.throttle_call();

				if (debugIteration) {
					wxMessageBox("KRPC Server test (throttle=0): " + std::to_string(engine.throttle()), "Debug test");
				}
				

				typedef krpc::services::KRPC::Expression Expr;

				std::mutex ThreadControl;

				auto eventHandler = ([&ThreadControl,&engine, EngineUI]() {

					ThreadControl.lock();
				
					EngineUI->enabled = (engine.throttle() == 0.0f) ? false : true;
					EngineUI->Redraw();
					
					wxMessageBox("Recieved message from server", "Recieved message");
					

					ThreadControl.unlock();

				});

				auto disablingEvent = krpc_server.add_event(Expr::equal(serverConnection, Expr::call(serverConnection, thrust_call), Expr::constant_float(serverConnection, 0.0f)));
				auto enablingEvent = krpc_server.add_event(Expr::greater_than(serverConnection, Expr::call(serverConnection, thrust_call), Expr::constant_float(serverConnection, 0.0f)));

				disablingEvent.add_callback(eventHandler);
				enablingEvent.add_callback(eventHandler);

				debugIteration = false;
			}
			
		
	}
	else return false;
}
*/

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

	renderingFrame->Show();

	ConnectToPython();
	
	return true;
}





