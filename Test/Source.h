#pragma once

#define ENABLE_SH_ENGINES 1
#define DISABLE_SH_ENGINES 2
#define PYCONNECTION 3


class MainApp : public wxApp {
public:

	virtual bool OnInit();

private:

	
	bool isConnectionInitialized = false;

	wxMenu* toggleShortcutList = NULL;

	std::vector<EngineFrame*> boosterEngines;
	std::vector<EngineFrame*> starshipEngines;

	void ConnectToPython();
	void StartConnection();

	void HandleConnectionOutput(wxSocketEvent& Event);
	void OnRMBClicked(const wxMouseEvent& Event);
	void OnMenuActivated(const wxCommandEvent& Event);

	wxDECLARE_EVENT_TABLE();

};

class PythonConnection : public wxThread {
public:
	PythonConnection(wxApp* app) : wxThread(wxTHREAD_DETACHED) {
	
		handler = app;
	
	};
	

protected:

	wxApp* handler;
	virtual ExitCode Entry();

};

