#pragma once

#include <deque>

#include "ActivityBase.h"

class ZoomActivity : public ActivityBase
{
private: // Fields
	/// <summary>
	/// Indicates whether this activity represents zoom in.
	/// </summary>
	bool bZoomIn = true;

	/// <summary>
	/// The zoom factor of the activity.
	/// </summary>
	double zoomFactor = -1;

public: // Constructor
	ZoomActivity(double);

public: // Methods
	/// <summary>
	/// Returns whether this activity represents zoom in.
	/// </summary>
	/// <returns>True if it's zoom IN, false if zoom OUT.</returns>
	inline bool isZoomIn(void) const { return this->bZoomIn; }

	/// <summary>
	/// Get the zoom factor.
	/// </summary>
	/// <returns>The delta zoom factor.</returns>
	inline double getZoomFactor(void) const { return this->zoomFactor; }

	/// <summary>
	/// Chain another zoom activity.
	/// </summary>
	/// <param name="activity">Another zoom activity to chain.</param>
	void chain(ZoomActivity* const activity);

public: // Inherited via ActivityBase
	/// <summary>
	/// Get the type of the activity.
	/// </summary>
	/// <returns>ActivityBase::EType::ZOOM</returns>
	virtual EType getType() const override;

	/// <summary>
	/// Inverse this activity to switch undo/redo state.
	/// </summary>
	virtual void inverse() override;
};
