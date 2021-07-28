
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "editor.h"
#include "htmlhelp.h"
#include "tabber.h"
#include <filesystem>

class MainFrame : public CFrameWnd,public HelpListener,EditorListener,TabberListener{
public:
	MainFrame();

	Editor *getEditor();
	void setTitle( const std::string &s );

	DECLARE_DYNAMIC( MainFrame )
	DECLARE_MESSAGE_MAP()

	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd( CDC *dc );
	afx_msg void OnSize( UINT type,int w,int h );
	afx_msg void quick_Help();

	afx_msg void fileNew();
	afx_msg void fileOpen();
	afx_msg void fileClose();
	afx_msg void fileCloseAll();
	afx_msg void fileSave();
	afx_msg void fileSaveAs();
	afx_msg void fileSaveAll();
	afx_msg void filePrint();
	afx_msg void fileExit();
	afx_msg void fileRecent( UINT id );

	afx_msg void editCut();
	afx_msg void editCopy();
	afx_msg void editPaste();
	afx_msg void editSelectAll();
	afx_msg void editFind();
	afx_msg void editFindNext();
	afx_msg void editReplace();

	afx_msg void programExecute();
	afx_msg void programReExecute();
	afx_msg void programCompile();
	afx_msg void programPublish();
	afx_msg void programCommandLine();
	afx_msg void programDebug();
	afx_msg void programLAA();

	afx_msg void helpHome();
	afx_msg void helpAutodoc();
	afx_msg void helpBack();
	afx_msg void helpForward();
	afx_msg void helpAbout();

	afx_msg void updateCmdUI( CCmdUI *ui );
	afx_msg void updateCmdUIRange( CCmdUI *ui );
	afx_msg void ctrlTab();
	afx_msg void ctrlShiftTab();
	afx_msg void escape();
	afx_msg void OnActivate( UINT state,CWnd *other,BOOL min );

private:
	Tabber tabber;
	CToolBar toolBar;
	CStatusBar statusBar;

	std::map<CWnd*,Editor*> editors;
	std::map<CWnd*,HtmlHelp*> helps;

	std::string last_quick_help;

	HtmlHelp *getHelp();
	HtmlHelp *getHelp( int index );
	HtmlHelp *findHelp();

	Editor *getEditor( int index );

	bool exit_flag;

	void insertRecent( const std::string &f );
	void newed( const std::string &t );
	bool open( const std::string &f );
	bool close( int n );
	bool save( int n );

	void compile( const std::string &cmd );
	void build( bool exec,bool publish );

	//does the dir exist? If not, create it.
	void checkAndCreateDir(std::string dir);

	//editorlistener
	void cursorMoved( Editor *editor );

	//tabberlistener
	void currentSet( Tabber *tabber,int index );

	//htmlhelplistener
	void helpOpen( HtmlHelp *help,const std::string &file );
	void helpTitleChange( HtmlHelp *help,const std::string &title );
};

#endif
