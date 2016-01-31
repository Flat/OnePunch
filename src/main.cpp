#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/aboutdlg.h>
#endif


const char* version = "0.1.0";

class OnePunch: public wxApp{
public:
	virtual bool OnInit();
};

class OnePunchFrame: public wxFrame{
public:
	OnePunchFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(OnePunchFrame, wxFrame)
	EVT_MENU(wxID_EXIT, OnePunchFrame::OnExit)
	EVT_MENU(wxID_ABOUT, OnePunchFrame::OnAbout)
	EVT_MENU(wxID_OPEN, OnePunchFrame::OnOpen)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(OnePunch);

bool OnePunch::OnInit(){
	OnePunchFrame *frame = new OnePunchFrame("OnePunch", wxPoint(50,50), wxSize(450,340));
	frame->Show(true);
	return true;
}

OnePunchFrame::OnePunchFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size){
		wxPanel *panel = new wxPanel(this, -1);
		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

		wxStaticText *ipLabel = new wxStaticText(panel, wxID_ANY, wxT("3ds IP Address"));
		hbox->Add(ipLabel, 0, wxRIGHT, 8);
		wxTextCtrl *ipTextBox = new wxTextCtrl(panel, wxID_ANY);
		hbox->Add(ipTextBox, 1);
		vbox->Add(hbox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
		vbox->Add(-1, 10);

		wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *ciaLabel = new wxStaticText(panel, wxID_ANY, wxT("CIA Queue"));
		hbox2->Add(ciaLabel, 0);
		vbox->Add(hbox2, 0, wxLEFT | wxTOP, 10);
		vbox->Add(-1, 10);

		wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);
		wxListBox *ciaBox = new wxListBox(panel, wxID_ANY, wxPoint(-1,-1), wxSize(-1, -1));
		hbox3->Add(ciaBox, 1, wxEXPAND);
		vbox->Add(hbox3, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

		vbox->Add(-1, 10);

		wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
		wxGauge *progress = new wxGauge(panel, wxID_ANY, 100);
		hbox5->Add(progress, 1, wxLEFT | wxRIGHT);
		vbox->Add(hbox5, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

		vbox->Add(-1, 10);

		wxBoxSizer *hbox4 = new wxBoxSizer(wxHORIZONTAL);
		wxButton *btnSend = new wxButton(panel, wxID_ANY, wxT("Send"));
		wxButton *btnCancel = new wxButton(panel, wxID_ANY, wxT("Cancel"));
		hbox4->Add(btnCancel, 0);
		hbox4->Add(btnSend, 0, wxLEFT | wxBOTTOM, 5);
		vbox->Add(hbox4, 0, wxALIGN_RIGHT | wxRIGHT, 10);


		wxMenu *menuFile = new wxMenu;
		menuFile->Append(wxID_OPEN, wxT("&Open\tCtrl+O"));
		menuFile->Append(wxID_EXIT);
		wxMenu *menuHelp = new wxMenu;
		menuHelp->Append(wxID_ABOUT);

		wxMenuBar *menuBar = new wxMenuBar;
		menuBar->Append(menuFile, "&File");
		menuBar->Append(menuHelp, "&Help");

		SetMenuBar(menuBar);
		panel->SetSizer(vbox);
		Center();
		CreateStatusBar();
		SetStatusText("Ready!");

	}

void OnePunchFrame::OnExit(wxCommandEvent& event){
	Close(true);
}

void OnePunchFrame::OnAbout(wxCommandEvent& event){
	wxAboutDialogInfo info;
	info.SetName(_("OnePunch"));
	info.SetVersion(_(version));
	info.SetDescription(_("An application to transmit cia files to a 3ds with FBI.\nhttp://github.com/Flat/OnePunch"));
	info.SetCopyright("(C) Kenneth Swenson 2016");
	info.AddDeveloper("Kenneth Swenson (Flat) <flat@imo.uto.moe>");
	wxAboutBox(info);
}
void OnePunchFrame::OnOpen(wxCommandEvent& event){

}