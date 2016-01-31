#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/aboutdlg.h>
	#include <wx/filename.h>
	#include <wx/dir.h>
#endif


const char* version = "0.1.0";

class OnePunch: public wxApp{
public:
	virtual bool OnInit();
};

class OnePunchFrame: public wxFrame{
public:
	OnePunchFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void addCia(wxArrayString &strs) {ciaBox->InsertItems(strs, ciaBox->GetCount());}
	void rmCia(unsigned int pos){ciaBox->Delete(pos);}
	wxArrayInt getCiaSelection(){wxArrayInt selections; ciaBox->GetSelections(selections); return selections;}
	void setProgress(int progressValue){ progress->SetRange(100); progress->SetValue(progressValue); }
	void setProgressLoading(){progress->Pulse();}

private:
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnDropFiles(wxDropFilesEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnSend(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	wxListBox *ciaBox;
	wxGauge *progress;

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(OnePunchFrame, wxFrame)
	EVT_MENU(wxID_EXIT, OnePunchFrame::OnExit)
	EVT_MENU(wxID_ABOUT, OnePunchFrame::OnAbout)
	EVT_MENU(wxID_OPEN, OnePunchFrame::OnOpen)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(OnePunch);

bool OnePunch::OnInit(){
	OnePunchFrame *frame = new OnePunchFrame("OnePunch", wxPoint(50,50), wxSize(450,500));
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
		ciaBox = new wxListBox(panel, wxID_ANY, wxPoint(-1,-1), wxSize(-1, -1), 0, NULL, wxLB_EXTENDED);
		ciaBox->DragAcceptFiles(true);
		ciaBox->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(OnePunchFrame::OnDropFiles), NULL, this);
		ciaBox->Connect(wxEVT_CHAR, wxKeyEventHandler(OnePunchFrame::OnChar), NULL, this);
		hbox3->Add(ciaBox, 1, wxEXPAND);
		vbox->Add(hbox3, 10, wxLEFT | wxRIGHT | wxEXPAND, 10);

		vbox->Add(-1, 10);

		wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
		progress = new wxGauge(panel, wxID_ANY, 100);
		hbox5->Add(progress, 1, wxLEFT | wxRIGHT);
		vbox->Add(hbox5, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

		vbox->Add(-1, 10);

		wxBoxSizer *hbox4 = new wxBoxSizer(wxHORIZONTAL);
		wxButton *btnSend = new wxButton(panel, wxID_ANY, wxT("Send"));
		btnSend->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OnePunchFrame::OnSend));
		wxButton *btnCancel = new wxButton(panel, wxID_ANY, wxT("Cancel"));
		btnCancel->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OnePunchFrame::OnCancel));
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
	wxFileDialog openFileDialog(this, _("Select CIA file"), "", "", "cia files (*.cia)|*.cia", wxFD_OPEN|wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	if(openFileDialog.ShowModal() == wxID_CANCEL){
		return;
	}
	wxArrayString selectedFiles;
	openFileDialog.GetPaths(selectedFiles);
	addCia(selectedFiles);
}

void OnePunchFrame::OnDropFiles(wxDropFilesEvent& event){
	if(event.GetNumberOfFiles() > 0){
		wxString* droppedfiles = event.GetFiles();
		wxASSERT(droppedfiles);

		wxString filename;
		wxString extension;
		wxString filespec = _T("*.cia");
		wxArrayString files;

		for(int i=0; i< event.GetNumberOfFiles(); i++){
			filename = droppedfiles[i];
			wxFileName::SplitPath(filename, NULL, NULL, NULL, &extension);
			if(wxFileExists(filename) && extension == _T("cia")){
				files.push_back(filename);
			}
			else if(wxDirExists(filename)){
				wxDir::GetAllFiles(filename, &files, filespec);
			}
		}
		if(!files.IsEmpty()){
			addCia(files);
		}

	}	
}
void OnePunchFrame::OnChar(wxKeyEvent& event){
	if(event.GetKeyCode() == WXK_DELETE){
		wxArrayInt selections = getCiaSelection();
		int removed = 0;
		for(size_t i=0; i< selections.size(); i++){
			rmCia(selections[i - removed]);
			removed++;
		}
	}
}

void OnePunchFrame::OnSend(wxCommandEvent& event){
	//todo send button logic
	wxMessageBox(wxT("Send Button"));
}

void OnePunchFrame::OnCancel(wxCommandEvent& event){
	//todo cancel button logic
	wxMessageBox(wxT("Cancel button"));
}