

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/notifmsg.h>
#include <wx/graphics.h>

#include <krpc.hpp>
#include <krpc/error.hpp>
#include "krpc/services/krpc.hpp"

// Declarations

#include "Geometry.h"
#include "Gui.h"
#include "Source.h"



void MainApp::RPCConnect() {
	try {
		serverConnection = krpc::connect("Starship Layout Emulator", "localhost");
		isConnectionInitialized = true;
	}
	catch (std::runtime_error Error) {
		wxMessageBox("The program was unable to connect to the kRPC server.\n\nCheck if your server is running (on RPC port 50000 and stream port 50001)", "Connection Error");
		
		serverConnection = {};
		isConnectionInitialized = false;
	}

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
		RPCConnect();
		break;
	}
	}

}

EngineFrame::EngineFrame(int Radius, const wxPoint& Position, wxWindow* Parent, int Id) : wxButton()
{

	this->radius = Radius;
	this->position = Position;

	Create(Parent, Id, wxEmptyString, Position, wxSize(Radius, Radius), wxBU_NOTEXT || wxBORDER_NONE);

	SetBackgroundColour(wxBLACK->GetAsString());

	Bind(wxEVT_PAINT, &EngineFrame::OnPaint, this);

}

void EngineFrame::Clear(wxGraphicsContext* GC) {

	GC->SetBrush(*wxBLACK_BRUSH);
	GC->DrawEllipse(0, 0, radius, radius);

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
		winContext->SetBrush(wxBrush(*wxBLACK_BRUSH));

	winContext->DrawEllipse(0, 0, radius - 1, radius - 1);

	winContext->Flush();

	delete winContext;
}

void EngineFrame::OnPaint(const wxPaintEvent& Event) { Redraw(); };


RenderWindow::RenderWindow(const std::string& Title) : wxFrame(NULL, 1, Title, wxPoint(100, 100), wxSize(1200, 600)) {
	
	SetBackgroundColour(wxBLACK->GetAsString());

}

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {

	RPCConnect();

	RenderWindow* renderingFrame = new RenderWindow("Starship Engine Layout");
	wxPanel* controlUI = new wxPanel(renderingFrame);

	std::vector<wxPoint> innerRingEnginePositions = ComputePointsOnCircle(30, 3, 1);
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

	toggleShortcutList = new wxMenu();
	toggleShortcutList->Append(1, "Enable SH Engines");
	toggleShortcutList->Append(2, "Disable SH Engines");

	toggleShortcutList->Append(3, "Enable SS Engines");
	toggleShortcutList->Append(4, "Disable SS Engines");

	toggleShortcutList->Append(5, "Reconnect to kRPC");

	Bind(wxEVT_RIGHT_UP, &MainApp::OnRMBClicked, this);
	Bind(wxEVT_MENU, &MainApp::OnMenuActivated, this);

	renderingFrame->Show();
	
	return true;
}





