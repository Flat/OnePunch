#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/aboutdlg.h>
	#include <wx/filename.h>
	#include <wx/dir.h>
	#include <wx/socket.h>
	#include <wx/sckstrm.h>
	#include <wx/sckipc.h>
	#include <wx/ffile.h>
	#include <wx/wfstream.h>
	#include <wx/utils.h>
	#include <wx/log.h>
	#include <wx/datstrm.h>
#endif
#include <cstdio>
#include <cstdint>
#ifdef __unix__
#include <endian.h>
#endif


const char* version = "0.2.1";
const int sendBuffSize = 32768;
const int FBIport = 5000;