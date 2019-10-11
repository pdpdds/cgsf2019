#pragma once

#include "GUIState.h"

class GUILogin : public GUIState
{
public:
	GUILogin(eGUIState State);
	~GUILogin();

	virtual bool ProcessInput(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnEnter() override;
	virtual bool OnLeave() override;

	bool isVisible() const;

	virtual bool Notify(BasePacket* pPacket) override;

private:
	// these must match the IDs assigned in the layout
	static const unsigned int NameEditBoxID;
	static const unsigned int PasswordEditBoxID;
	static const unsigned int SubmitButtionID;
	
	bool handleSubmit(const CEGUI::EventArgs& args);
	bool handleKeyDown(const CEGUI::EventArgs& args);

	CEGUI::Window* d_root;
	int d_historyPos;
	std::vector<CEGUI::String> d_history;
};
