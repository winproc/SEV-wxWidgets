#pragma once

#define ENABLE_SH_ENGINES 1
#define DISABLE_SH_ENGINES 2

class MainApp : public wxApp {
public:

	virtual bool OnInit();

private:

	krpc::Client serverConnection{};
	bool isConnectionInitialized = false;

	wxMenu* toggleShortcutList = NULL;

	std::vector<EngineFrame*> boosterEngines;
	std::vector<EngineFrame*> starshipEngines;

	void RPCConnect();
	void OnRMBClicked(const wxMouseEvent& Event);
	void OnMenuActivated(const wxCommandEvent& Event);

};