#pragma once


class RenderWindow : public wxFrame {

public:

	/*
	Constructor for the top-level window

	@param Title Name of the window
	*/
	RenderWindow(const std::string& Title);

private:

	
};

class EngineFrame : public wxButton {

public:

	/*
	
	Constructor for the custom control representing the engine toggle state
	
	@param Radius Radius of the circle representing the engine toggle state
	@param Position Position of the control relative to the window client area

	*/
	EngineFrame(int Radius, const wxPoint& Position, wxWindow* Parent, int Id);

	bool Enabled = false;
	int Radius;

private:

	void OnPaint(const wxPaintEvent& Event);

};