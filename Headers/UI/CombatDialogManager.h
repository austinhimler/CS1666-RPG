#ifndef _____COMBAT_DIALOG_H_____
#define _____COMBAT_DIALOG_H_____

#include <iostream>
#include <vector>
#include <queue>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "DialogMessage.h"
#include "TextRenderer.h"

class CombatDialogManager
{
public:

	// Represents the inputs used by the combat dialog
	enum DialogInput
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SELECT
	};

	// Represents a selection event
	// These are generated when an selection occurs
	// Retrieve them using DequeueEvent()
	struct SelectionEvent
	{
		int selectedOption;
		std::vector<std::string> options;
	};

	CombatDialogManager();
	~CombatDialogManager() {};

	// Updates the dialog box
	void Update(float deltaTime);

	// Register a keyboard input
	void RegisterKeyInput(DialogInput input);

	// Returns a vector containing all the text to render for the current dialog
	std::vector<RenderableText> GetTextToRender();

	// Queues up a message for the dialog box
	void AddMessage(std::string message);

	// Queues up a selectable option message for the dialog box
	void AddSelectableOption(std::string message, std::vector<std::string> options);

	// Returns all events since the last clear
	std::queue<SelectionEvent> GetEvents();

	// Clears all recent events
	void ClearEvents();

	// Removes all messages from the dialog
	void Clear();

	// Sets the write speed of the dialog text.
	// All messages written out to the combat dialog will be affected by this multiplier.
	// Useful for fast-forward/slow down.
	void SetWriteSpeedMultiplier(float speed);

	// Sets the time between each letter being written out to the screen.
	// Any new dialog messages generated will use this time
	void SetTimePerCharacter(float timePerCharacter);

	// Sets the time taken for a message to stay on screen until it dissapears
	void SetWaitTime(float waitTime);

	// Sets the color to use for any newly generated dialog messages
	void SetColor(glm::vec4 color);

	// Sets the selection color to use for any newly generated dialog messages
	void SetSelectionColor(glm::vec4 color);

	// Sets the font to use for any newly generated dialog messages
	void SetFont(FontData& font);

private:



	// Creates and returns a dialog message
	DialogMessage CreateDialogMessage(std::string message, std::vector<std::string> options = std::vector<std::string>());

	// The current queue of dialog messages
	std::queue<DialogMessage> m_dialogMessages;

	// The current queue of selection events
	std::queue<SelectionEvent> m_selectionEvents;

	// Dialog Manager State
	float m_writeSpeedMultiplier;		// The speed multiplier to apply to all messages created by the dialog manager. Useful for fast forward etc.
	float m_currentTimePerCharacter;    // The current time per character of the dialog manager. Any new messages created will use this time per letter.
	float m_currentWaitTime;			// The current wait time state of the dialog manager. Any new messages created will use this wait time.
	glm::vec4 m_currentColor;			// The current color to use for text. Any new messages will use this color
	glm::vec4 m_currentSelectionColor;	// The current color to use for selection. Any new messages will use this color for selection
	FontData* m_currentFont;			// The current font to use for dialog messages. Any new messages will use this font.
};

#endif