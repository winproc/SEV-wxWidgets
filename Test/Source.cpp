
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


// Declarations

class MainApp : public wxApp {
public:
	virtual bool OnInit();
};

#include "Gui.h"

// Definitions

RenderWindow::RenderWindow(const std::string& Title) : wxFrame(NULL, wxID_ANY, Title, wxPoint(100, 100), wxSize(600, 600)) {

}

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {

	RenderWindow* RenderingFrame = new RenderWindow("Test");
	RenderingFrame->Show();

	return true;
}





