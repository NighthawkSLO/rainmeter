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
	std::wstring Shapeidentifier = L"Shape";
	const std::wstring& curShape = parser.ReadString(section, Shapeidentifier.c_str(), L"");
	size_t i = 0;
	while (!curShape.empty())
	{
		std::vector<std::wstring> shapeTokens = parser.Tokenize(curShape, L"|");
		for (const std::wstring shape : shapeTokens)
		{
			std::vector<std::wstring> pointTokens = parser.Tokenize(parser.ReadString(section, shape.c_str(), L""), L"|");
			for (const std::wstring point : pointTokens)
				LogDebug(point.c_str());
			LogDebug(shape.c_str());
		}
		const std::wstring& curShape = parser.ReadString(section, (Shapeidentifier + std::to_wstring(++i)).c_str(), L"");
	}
}

void MeterVector::BindMeasures(ConfigParser& parser, const WCHAR* section)
{
	if (BindPrimaryMeasure(parser, section, true))
	{
		BindSecondaryMeasures(parser, section);
	}
}
