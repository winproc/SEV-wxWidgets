

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

class MainApp : public wxApp {
public:

	virtual bool OnInit();

private:

	krpc::Client ServerConnection{};
	bool ConnectionInitialized = false;

	wxMenu* ToggleShortcutList                      = NULL;
	
	std::vector<EngineFrame> BoosterEngines;
	std::vector<EngineFrame> StarshipEngines;

	void RPCConnect();
	void OnRMBClicked(const wxMouseEvent& Event);
	void OnMenuActivated(const wxCommandEvent& Event);
	
};

// Definitions

void MainApp::RPCConnect() {
	try {
		ServerConnection = krpc::connect("Starship Layout Emulator", "localhost");
		ConnectionInitialized = true;
	}
	catch (std::runtime_error Error) {
		wxMessageBox("The program was unable to connect to the kRPC server.\n\nCheck if your server is running (on RPC port 50000 and stream port 50001)", "Connection Error");
		
		ServerConnection = {};
		ConnectionInitialized = false;
	}

}

void MainApp::OnRMBClicked(const wxMouseEvent& Event) {

	wxWindow::FindWindowById(Event.GetId())->PopupMenu(ToggleShortcutList);
	
}


void MainApp::OnMenuActivated(const wxCommandEvent& Event) {

	switch (Event.GetId()) {
	case 5:
		RPCConnect();
	}

}

EngineFrame::EngineFrame(int Radius, const wxPoint& Position, wxWindow* Parent, int Id) : 
	wxButton(
		Parent, 
		Id, 
		wxEmptyString,
		Position,
		wxSize(Radius, Radius),
		wxBU_NOTEXT || wxBORDER_NONE
	) 
{
	
	this->Radius = Radius;
	this->Pos = Position;

	Bind(wxEVT_PAINT, &EngineFrame::OnPaint, this);

}

void EngineFrame::Clear(wxGraphicsContext* GC) {

	GC->SetBrush(*wxBLACK_BRUSH);
	GC->DrawRectangle(this->Pos.x, this->Pos.y, Radius, Radius);

}

void EngineFrame::OnPaint(const wxPaintEvent& Event) {

	wxGraphicsRenderer* D2DRenderer = wxGraphicsRenderer::GetDirect2DRenderer();

	wxPaintDC PaintContext(this);

	wxGraphicsContext* WinContext = D2DRenderer->CreateContextFromUnknownDC(PaintContext);

	Clear(WinContext);

	WinContext->SetBrush(*wxBLACK_BRUSH);
	WinContext->SetPen(*wxWHITE_PEN);

	WinContext->DrawEllipse(this->Pos.x, this->Pos.y, Radius, Radius);
	WinContext->Flush();

	delete WinContext;
}



RenderWindow::RenderWindow(const std::string& Title) : wxFrame(NULL, 1, Title, wxPoint(100, 100), wxSize(1200, 600)) {
	
	SetBackgroundColour(wxBLACK->GetAsString());

}

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {

	RPCConnect();

	RenderWindow* RenderingFrame = new RenderWindow("Starship Engine Layout");
	wxPanel* ControlUI = new wxPanel(RenderingFrame);

	EngineFrame* Engine = new EngineFrame(500, wxPoint(1, 3), ControlUI, 18);

	ToggleShortcutList = new wxMenu();
	ToggleShortcutList->Append(1, "Enable SH Engines");
	ToggleShortcutList->Append(2, "Disable SH Engines");

	ToggleShortcutList->Append(3, "Enable SS Engines");
	ToggleShortcutList->Append(4, "Disable SS Engines");

	ToggleShortcutList->Append(5, "Reconnect to kRPC");

	Bind(wxEVT_RIGHT_UP, &MainApp::OnRMBClicked, this);
	Bind(wxEVT_MENU, &MainApp::OnMenuActivated, this);

	RenderingFrame->Show();
	
	return true;
}





