#pragma once


class RenderWindow : public wxFrame {

public:

	/// <summary>
	/// <c>RenderWindow</c> is a class for the custom control representing the engine toggle state
	/// </summary>
	/// <param name="Title">Name of the window</param>
	RenderWindow(const std::string& Title);

private:

	
};

/// <summary>
/// <c>EngineFrame</c> is a class for the custom control representing the engine toggle state
/// </summary>

/// <permission>Publicly accessible</permission>
class EngineFrame : public wxButton {

public:
	
	/// <param name="Radius">Radius of the circle representing the engine toggle state</param>
	/// <param name="Position">Position of the control relative to the window client area</param>
	EngineFrame(int Radius, const wxPoint& Position, wxWindow* Parent, int Id);

	bool enabled = false;

	wxPoint position;
	int radius;

	void Redraw();

private:

	
	void OnPaint(const wxPaintEvent& Event);
	void Clear(wxGraphicsContext* GC);

};