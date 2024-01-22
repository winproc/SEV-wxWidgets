#pragma once

#define ENABLE_SH_ENGINES 1
#define DISABLE_SH_ENGINES 2

wxDECLARE_EVENT(OUTPUT_RECIEVED, wxThreadEvent);

class MainApp : public wxApp {
public:

	virtual bool OnInit();

private:

	
	bool isConnectionInitialized = false;

	wxMenu* toggleShortcutList = NULL;

	std::vector<EngineFrame*> boosterEngines;
	std::vector<EngineFrame*> starshipEngines;

	void ConnectToPython();
	void StartConnectionThread();
	void HandleConnectionOutput(wxThreadEvent& Event);

	void OnRMBClicked(const wxMouseEvent& Event);
	void OnMenuActivated(const wxCommandEvent& Event);

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

