#pragma once

#define ENABLE_SH_ENGINES 1
#define DISABLE_SH_ENGINES 2

class MainApp : public wxApp {
public:

	virtual bool OnInit();

private:

	
	bool isConnectionInitialized = false;

	wxMenu* toggleShortcutList = NULL;

	std::vector<EngineFrame*> boosterEngines;
	std::vector<EngineFrame*> starshipEngines;

	void ConnectToPython();
	

	void OnRMBClicked(const wxMouseEvent& Event);
	void OnMenuActivated(const wxCommandEvent& Event);

};

