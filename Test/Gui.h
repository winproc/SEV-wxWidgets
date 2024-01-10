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

	/// <summary>
	/// <c>EngineFrame</c> is a constructor for the custom control representing the engine toggle state
	/// </summary>
	/// <param name="Radius">Radius of the circle representing the engine toggle state</param>
	/// <param name="Position">Position of the control relative to the window client area</param>
	/// <example>
	/// <code>
    /// int main()
    /// {
    ///    RenderWindow* RenderingFrame = new RenderWindow("Starship Engine Layout");
	///    wxPanel* ControlUI = new wxPanel(RenderingFrame);
	/// 
	///    EngineFrame* Engine = new EngineFrame(500, wxPoint(1, 3), ControlUI, 18);
    /// }
    /// </code>
    /// </example>
	/// <permission>Publicly accessible</permission>
	EngineFrame(int Radius, const wxPoint& Position, wxWindow* Parent, int Id);

	bool Enabled = false;

	wxPoint Pos;
	int Radius;

private:

	void OnPaint(const wxPaintEvent& Event);
	void Clear(wxGraphicsContext* GC);

};