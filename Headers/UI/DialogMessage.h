
#ifndef _____DIALOG_MESSAGE_H_____
#define _____DIALOG_MESSAGE_H_____

#include <iostream>
#include <vector>
#include <glm/vec4.hpp>

class DialogMessage
{
public:

	// Represents the current state of the dialog message box
	enum State
	{
		NOT_WRITING,				// The dialog message is not doing anything currently
		WRITING,					// The dialog message is currently writing out text
		WAITING,					// The dialog message is currently waiting with text on the screen
		WAITING_FOR_SELECTION,		// The dialog message is waiting for a selection to be made by the user
		COMPLETE,					// The dialog message has completed writing out its text and passed its waiting state, or a selection has been made
	};

	DialogMessage(std::string message, std::vector<std::string> options = std::vector<std::string>());
	~DialogMessage() {};

	// Updates the dialog message
	void Update(float deltaTime);

	// Set Functions
	void SetTimePerCharacter(float timePerCharacter);
	void SetWaitTime(float waitTime);
	void SetTextColor(glm::vec4 color);
	void SetSelectionColor(glm::vec4 color);

	// Get Functions
	State GetState();
	float GetTimePerCharacter();
	float GetWaitTime(float waitTime);
	glm::vec4 GetTextColor();
	glm::vec4 GetSelectionColor();

	// Gets the currently displayed text that is being printed out 
	std::string GetCurrentlyDisplayedText();

	// Returns true if this dialog message has options along with the message
	bool HasOptions();

	// Gets the vector of options in this dialog message
	std::vector<std::string> GetOptions();

	// Returns the currently selected option
	int GetSelectedOption();

	// Returns the index of the last option in the vector of options
	int GetLastOption();

	// Sets the selection of this dialog message
	void SetSelectedOption(int option);

	// Selects the currently selected option.
	// This will set the dialog message state to complete
	void SelectOption();

private:

	std::string m_displayText;			// The text to display when writing is complete
	float m_timePerCharacter;			// The time between each character writing onto the screen. A value of 0 indicates instant display.
	float m_waitTime;					// The amount of time the message stays in the dialog box once its completed.
	std::vector<std::string> m_options;	// The options avaliable
	glm::vec4 m_textColor;				// The color of the text
	glm::vec4 m_selectionColor;			// The color of the selected option

	State m_state;						// The current state of the message
	int m_currentCharacterIndex;		// The current index of the character we are up to writing out in the DisplayText
	std::string m_currentDisplayText;	// The text currently displayed. This is written to as time elapses.
	float m_timeSinceLastCharacter;	    // The amount of time elapsed since the last character was written
	float m_timeSpentWaiting;			// The amount of time elapsed that was spent waiting once writing completed
	int m_currentOptionSelection;		// The current option selected
};

#endif