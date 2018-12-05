
#include "../../Headers/ResourceManager/FontData.h"

void FontData::SetCharacterMap(std::map<GLchar, Character> map)
{
	m_characterMap = map;
}

std::map<GLchar, FontData::Character>& FontData::GetCharacterMap()
{
	return m_characterMap;
}