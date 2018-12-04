#include "../../Headers/UI/DialogMessage.h"
#include <stdio.h>
#include <algorithm>

DialogMessage::DialogMessage(std::string message, std::vector<std::string> options)
	: m_displayText(message)
	, m_timePerCharacter(0)
	, m_waitTime(0)
	, m_options(options)
	, m_textColor(0.0, 0.0, 0.0, 1.0)
	, m_selectionColor(0.0, 1.0, 0.0, 1.0)
	, m_state(State::NOT_WRITING)
	, m_currentCharacterIndex(0)
	, m_currentDisplayText()
	, m_timeSinceLastCharacter(0.0)
	, m_timeSpentWaiting(0.0)
	, m_currentOptionSelection(-1)
{

}

void DialogMessage::Update(float deltaTime)
{
	if (m_state == NOT_WRITING)
	{
		// The text is now updating, so we should update the state accordingly
		m_state = WRITING;
	}

	if (m_state == WRITING)
	{
		// Update the time since last character
		m_timeSinceLastCharacter += deltaTime;

		// If the time since the last letter displayed is greater than the time per letter,
		// we should add a new letter onto our string
		// In the case where TimePerCharacter is 0, this loop will cause the text to instantly fill
		while (m_timeSinceLastCharacter >= m_timePerCharacter)
		{
			m_timeSinceLastCharacter -= m_timePerCharacter;
			m_currentCharacterIndex++;

			// If the current character index is at the end of the display text string, cap and exit the loop
			if (m_currentCharacterIndex >= m_displayText.length())
			{
				m_currentCharacterIndex = static_cast<int>(m_displayText.length());

				// Update the state to be waiting
				m_state = WAITING;

				// If this message has options, select the first 1
				if (HasOptions())
				{
					m_currentOptionSelection = 0;
				}
				break;
			}
		}
	}

	// If the dialog message is waiting, and doesnt have any options associated with it, then start the wait timer
	if (m_state == WAITING && !HasOptions())
	{
		m_timeSpentWaiting += deltaTime;

		// The text has been on screen for the designated period of time, it has completed its cycle
		if (m_timeSpentWaiting >= m_waitTime)
		{
			m_state = COMPLETE;
		}
	}

	// Update our current display string
	m_currentDisplayText = m_displayText.substr(0, m_currentCharacterIndex);
}

void DialogMessage::SetTimePerCharacter(float timePerCharacter)
{
	m_timePerCharacter = timePerCharacter;
}

void DialogMessage::SetWaitTime(float waitTime)
{
	m_waitTime = waitTime;
}

void DialogMessage::SetTextColor(glm::vec4 color)
{
	m_textColor = color;
}

void DialogMessage::SetSelectionColor(glm::vec4 color)
{
	m_selectionColor = color;
}

DialogMessage::State DialogMessage::GetState()
{
	return m_state;
}

float DialogMessage::GetTimePerCharacter()
{
	return m_timePerCharacter;
}

float DialogMessage::GetWaitTime(float waitTime)
{
	return m_waitTime;
}

glm::vec4 DialogMessage::GetTextColor()
{
	return m_textColor;
}

glm::vec4 DialogMessage::GetSelectionColor()
{
	return m_selectionColor;
}

std::string DialogMessage::GetCurrentlyDisplayedText()
{
	return m_currentDisplayText;
}

bool DialogMessage::HasOptions() 
{ 
	return m_options.size() > 0; 
}

std::vector<std::string> DialogMessage::GetOptions()
{
	return m_options;
}

int DialogMessage::GetSelectedOption()
{
	return m_currentOptionSelection;
}

int DialogMessage::GetLastOption()
{
	return m_options.size() - 1;
}

void DialogMessage::SetSelectedOption(int option)
{
	if (m_state != COMPLETE)
	{
		// Clamp option to avoid out of bounds access
		option = std::max(option, 0);
		option = std::min(option, static_cast<int>(m_options.size() - 1));
		m_currentOptionSelection = option;
	}
}

void DialogMessage::SelectOption()
{
	m_state = COMPLETE;
}