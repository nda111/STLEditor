#include "ZoomActivity.h"

class ZoomActivity;

ZoomActivity::ZoomActivity(double zoomFactor) : zoomFactor(zoomFactor), bZoomIn(zoomFactor > 1)
{
	_ASSERT(zoomFactor >= 0);
	_ASSERT(zoomFactor != 1);
}

void ZoomActivity::chain(ZoomActivity* const activity)
{
	this->zoomFactor *= activity->zoomFactor;
}

ActivityBase::EType ZoomActivity::getType() const
{
	return EType::ZOOM;
}

void ZoomActivity::inverse()
{
	bZoomIn = !bZoomIn;
	this->zoomFactor = 1.0 / this->zoomFactor;
}
