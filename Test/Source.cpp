

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// #include <krpc.hpp>

// Declarations

#include "Geometry.h"
#include "Gui.h"

class MainApp : public wxApp {
public:
	virtual bool OnInit();
private:

	wxMenu* ToggleShortcutList;

	std::vector<EngineFrame> BoosterEngines;
	std::vector<EngineFrame> StarshipEngines;


	void OnRMBClicked(const wxMouseEvent& Event);
	void OnMenuActivated(const wxCommandEvent& Event);
	
};

// Definitions

void MainApp::OnRMBClicked(const wxMouseEvent& Event) {

	wxWindow::FindWindowById(Event.GetId())->PopupMenu(ToggleShortcutList);
	
}


void MainApp::OnMenuActivated(const wxCommandEvent& Event) {

	wxMessageBox("Test", "no");

}


RenderWindow::RenderWindow(const std::string& Title) : wxFrame(NULL, 1, Title, wxPoint(100, 100), wxSize(1200, 600)) {
	
	SetBackgroundColour(wxBLACK->GetAsString());

}

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {

	RenderWindow* RenderingFrame = new RenderWindow("Starship Engine Layout");

	ToggleShortcutList = new wxMenu();
	ToggleShortcutList->Append(1, "Enable SH Engines");
	ToggleShortcutList->Append(2, "Disable SH Engines");

	ToggleShortcutList->Append(3, "Enable SS Engines");
	ToggleShortcutList->Append(4, "Disable SS Engines");

	Bind(wxEVT_RIGHT_UP, &MainApp::OnRMBClicked, this);
	Bind(wxEVT_MENU, &MainApp::OnMenuActivated, this);
	
	

	RenderingFrame->Show();

	return true;
}





