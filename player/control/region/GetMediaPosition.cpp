#include "GetMediaPosition.hpp"

#include "constants.hpp"

GetMediaPosition::GetMediaPosition(int regionWidth, int regionHeight) :
    regionWidth_(regionWidth),
    regionHeight_(regionHeight)
{
}

int GetMediaPosition::getMediaX(int width, MediaGeometry::Align align) const
{
    switch (align)
    {
        case MediaGeometry::Align::Center: return (regionWidth_ - width) / 2;
        case MediaGeometry::Align::Left: return DefaultXPos;
        case MediaGeometry::Align::Right: return regionWidth_ - width;
    }
    return InvalidPos;
}

int GetMediaPosition::getMediaY(int height, MediaGeometry::Valign valign) const
{
    switch (valign)
    {
        case MediaGeometry::Valign::Middle: return (regionHeight_ - height) / 2;
        case MediaGeometry::Valign::Top: return DefaultYPos;
        case MediaGeometry::Valign::Bottom: return regionHeight_ - height;
    }
    return InvalidPos;
}
