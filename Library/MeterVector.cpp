/* Copyright (C) 2002 Rainmeter Project Developers
*
* This Source Code Form is subject to the terms of the GNU General Public
* License; either version 2 of the License, or (at your option) any later
* version. If a copy of the GPL was not distributed with this file, You can
* obtain one at <https://www.gnu.org/licenses/gpl-2.0.html>. */

#include "stdafx.h"
#include "MeterVector.h"
#include <sstream>
#include "Logger.h"

using namespace Gdiplus;

MeterVector::MeterVector(Skin * skin, const WCHAR * name) : Meter(skin, name),
		m_points()
{
}

MeterVector::~MeterVector()
{
}

void MeterVector::Initialize()
{
	Meter::Initialize();
}

bool MeterVector::Update()
{
	if (Meter::Update())
	{

		return true;
	}
	return false;
}

bool MeterVector::Draw(Gfx::Canvas & canvas)
{

	bool canDraw = Meter::Draw(canvas);
	if (!canDraw) return false;
	SolidBrush solid(m_fillColor);
	canvas.DrawPathVector(m_points, solid, m_outlineColor, m_Solid, m_lineWidth, m_connectedEdges);

	return true;
}

void MeterVector::ReadOptions(ConfigParser & parser, const WCHAR * section)
{
	Meter::ReadOptions(parser, section);

	m_fillColor = parser.ReadColor(section, L"FillColor", Color::MakeARGB(255, 0, 0, 0));
	m_outlineColor = parser.ReadColor(section, L"OutlineColor", Color::MakeARGB(255, 255, 255, 255));
	m_Solid = parser.ReadBool(section, L"Solid", true);
	m_connectedEdges = parser.ReadBool(section, L"ConnectEdges", true);
	m_lineWidth = parser.ReadFloat(section, L"LineWidth", 1);
	m_points.clear();
	std::vector<Gdiplus::REAL> point = parser.ReadFloats(section, L"Point1");
	if (point.size() >= 3)
	{
		if(point.size() == 3 && point[2] == 0)
		m_points.push_back(Gfx::Canvas::VectorPoint(point[0], point[1], point[2]));
		else if (point.size() == 6 && point[2] == 1)
			m_points.push_back(Gfx::Canvas::VectorPoint(point[0], point[1], point[2], point[3], point[4], point[5]));
		else if (point.size() == 7 && point[2] == 2)
			m_points.push_back(Gfx::Canvas::VectorPoint(point[0], point[1], point[2], point[3], point[4], point[5], point[6]));
	}
	else {
		return;
	}
	int index = 2;
	std::wostringstream wstringstream;
	while (point.size() >= 3)
	{
		wstringstream << L"Point" << index;
		point = parser.ReadFloats(section, wstringstream.str().c_str());
		if (point.size() >= 3)
		{
			if (point.size() == 3 && point[2] == 0)
				m_points.push_back(Gfx::Canvas::VectorPoint(point[0], point[1], point[2]));
			else if (point.size() == 6 && point[2] == 1)
				m_points.push_back(Gfx::Canvas::VectorPoint(point[0], point[1], point[2], point[3], point[4], point[5]));
			else if (point.size() == 7 && point[2] == 2)
				m_points.push_back(Gfx::Canvas::VectorPoint(point[0], point[1], point[2], point[3], point[4], point[5], point[6]));
		}
		wstringstream.str(L"");
		index++;
	}
	wstringstream << m_points.size();
	if (m_Initialized && m_Measures.empty() && !m_DynamicVariables)
	{
		Initialize();
		//m_NeedsRedraw = true;
	}
}

/*
** Overridden method. The Image meters need not to be bound on anything
**
*/
void MeterVector::BindMeasures(ConfigParser& parser, const WCHAR* section)
{
	if (BindPrimaryMeasure(parser, section, true))
	{
		BindSecondaryMeasures(parser, section);
	}
}
