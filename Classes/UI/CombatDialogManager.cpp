#include "../../Headers/UI/CombatDialogManager.h"
#include <stdio.h>

CombatDialogManager::CombatDialogManager()
	: m_writeSpeedMultiplier(1.0)
	, m_currentTimePerCharacter(0.0)
	, m_currentWaitTime(0.0)
	, m_currentColor(0.0, 0.0, 0.0, 1.0)
	, m_currentSelectionColor(1.0, 0.0, 0.0, 1.0)
	, m_currentFont(&ResourceManager::getDefaultFontData())
{
}

void CombatDialogManager::Update(float deltaTime)
{
	// If theres no messages, we dont need to update.
	if (m_dialogMessages.size() == 0)
	{
		return;
	}

	// Get the current dialog message
	DialogMessage &d = m_dialogMessages.front();

	// Update the current dialog message
	d.Update(deltaTime);

	// If this message has completed, remove it from the queue
	if (d.GetState() == DialogMessage::COMPLETE)
	{
		// If this was an option select, generate an event
		if (d.HasOptions())
		{
			SelectionEvent event;
			event.selectedOption = d.GetSelectedOption();
			event.options = d.GetOptions();
			m_selectionEvents.push(event);
		}

		m_dialogMessages.pop();
	}
}

std::queue<CombatDialogManager::SelectionEvent> CombatDialogManager::GetEvents()
{
	// Copy events
	return m_selectionEvents;
}

void CombatDialogManager::ClearEvents()
{
	m_selectionEvents = std::queue<SelectionEvent>();
}

void CombatDialogManager::RegisterKeyInput(DialogInput input)
{
	// We only care about input if we have a message with options, and if we are in the waiting for input state
	if (m_dialogMessages.size() == 0)
	{
		return;
	}

	// Get the current dialog message
	DialogMessage &d = m_dialogMessages.front();

	if (d.HasOptions() && d.GetState() == DialogMessage::WAITING)
	{
		int currentSelection = d.GetSelectedOption();

		// For dialog messages with options, we display like the following:
		// 
		//   Content of message goes here
		//   Option 0       Option 1
		//   Option 2       Option 3
		//   Option 4		etc...

		// So if we press right, we should increase the selection by 1 only if its an even number
		// If we press left, decrease by 1 if odd number
		// If we press down, increase by 2
		// If we press up, decrease by 2
		// In the case where we press up on option 1, we dont want it to go to option 0, so we handle this case
		// Same goes for option 3 and pressing down.

		switch (input)
		{
		case DialogInput::LEFT:
			if (currentSelection % 2 != 0)
			{
				currentSelection--;
			}
			break;
		case DialogInput::RIGHT:
			if (currentSelection % 2 == 0)
			{
				currentSelection++;
			}
			break;
		case DialogInput::UP:
			if (currentSelection >= 2)
			{
				currentSelection -= 2;
			}
			break;
		case DialogInput::DOWN:
			if (currentSelection <= d.GetLastOption() - 2)
			{
				currentSelection += 2;
			}
			break;
		case DialogInput::SELECT:
			// Selection has been made!
			// This dialog message is now complete
			d.SelectOption();
			break;
		}

		d.SetSelectedOption(currentSelection);
	}
}

std::vector<RenderableText> CombatDialogManager::GetTextToRender()
{
	std::vector<RenderableText> textToRender;

	// If theres no messages, theres nothing to render
	if (m_dialogMessages.size() == 0)
	{
		return textToRender;
	}

	// Get the current dialog message
	DialogMessage &d = m_dialogMessages.front();

	// The content of each dialog message determines where we should display each part
	// This is where you will position the text based on how long it is, how many options there are etc.
	// Set up reference point positions
	glm::vec2 topLeftCorner(26.0f, 167.0f);
	float optionColumnSpacing = 342.0;
	float optionRowSpacing = 35.0;
	glm::vec2 column1Pos(topLeftCorner.x, topLeftCorner.y - optionRowSpacing);
	glm::vec2 column2Pos(topLeftCorner.x + optionColumnSpacing, topLeftCorner.y - optionRowSpacing);
	float yOffsetForSingleMessage = 70;

	// For dialog messages with no options, we just display a message
	if (!d.HasOptions())
	{
		// Offset for single messages
		glm::vec2 offsetPos(topLeftCorner.x, topLeftCorner.y - yOffsetForSingleMessage);
		textToRender.push_back(RenderableText(d.GetCurrentlyDisplayedText(), offsetPos, 0.5f, d.GetTextColor(), *m_currentFont));
		return textToRender;
	}

	// For dialog messages with options, we display like the following:
	// 
	//   Content of message goes here
	//   Option 0       Option 1
	//   Option 2       Option 3
	//   etc..
	// Ive just split them into 2 columns to make things easier
	// Even numbers in left column, odd in right

	// Message
	RenderableText msg(d.GetCurrentlyDisplayedText(), topLeftCorner, 0.5f, d.GetTextColor(), *m_currentFont);
	textToRender.push_back(msg);

	// We only display the options once the message has written out
	if (d.GetState() == DialogMessage::WAITING)
	{
		auto options = d.GetOptions();

		for (int i = 0; i < options.size(); i++)
		{
			// Determine text color based on whether this is the currently selected option
			glm::vec4 textColor = d.GetSelectedOption() == i ? d.GetSelectionColor() : d.GetTextColor();

			if (i % 2 == 0.0)
			{
				textToRender.push_back(RenderableText(options[i], column1Pos, 0.5f, textColor, *m_currentFont));
				column1Pos.y -= optionRowSpacing;
			}
			else
			{
				textToRender.push_back(RenderableText(options[i], column2Pos, 0.5f, textColor, *m_currentFont));
				column2Pos.y -= optionRowSpacing;
			}
		}
	}

	return textToRender;
}

void CombatDialogManager::AddMessage(std::string message)
{
	m_dialogMessages.push(CreateDialogMessage(message));
}

void CombatDialogManager::AddSelectableOption(std::string message, std::vector<std::string> options)
{
	m_dialogMessages.push(CreateDialogMessage(message, options));
}

void CombatDialogManager::Clear()
{
	m_dialogMessages = std::queue<DialogMessage>();
}

void CombatDialogManager::SetWriteSpeedMultiplier(float speed)
{ 
	m_writeSpeedMultiplier = speed;
}

void CombatDialogManager::SetTimePerCharacter(float timePerCharacter)
{
	m_currentTimePerCharacter = timePerCharacter;
}

void CombatDialogManager::SetWaitTime(float waitTime)
{
	m_currentWaitTime = waitTime;
}

void CombatDialogManager::SetColor(glm::vec4 color)
{
	m_currentColor = color;
}

void CombatDialogManager::SetSelectionColor(glm::vec4 color)
{
	m_currentSelectionColor = color;
}

void CombatDialogManager::SetFont(FontData& font)
{
	m_currentFont = &font;
}

DialogMessage CombatDialogManager::CreateDialogMessage(std::string message, std::vector<std::string> options)
{
	DialogMessage d(message, options);

	// Set the dialog message to use the current state configuration in the dialog manager
	d.SetTextColor(m_currentColor);
	d.SetSelectionColor(m_currentSelectionColor);
	d.SetTimePerCharacter(m_currentTimePerCharacter / m_writeSpeedMultiplier);
	d.SetWaitTime(m_currentWaitTime);
	return d;
}