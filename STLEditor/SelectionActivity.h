#pragma once

#include <vtkActor.h>

#include "ActivityBase.h"

/// <summary>
/// An activity that represents selection of an object.
/// </summary>
class SelectionActivity : public ActivityBase
{
private: // Fields
	/// <summary>
	/// An actor for undoing.
	/// Initially nullptr.
	/// </summary>
	vtkActor* before;

	/// <summary>
	/// An actor for redoing.
	/// Initially nullptr.
	/// </summary>
	vtkActor* after;

public: // Constructor
	SelectionActivity(vtkActor* const before, vtkActor* const after);

public: // Method
	/// <summary>
	/// Get the actor.
	/// </summary>
	/// <returns>The actor.</returns>
	vtkActor* getActor(void) const;

public: // Inherited via ActivityBase
	/// <summary>
	/// Get the type of the activity.
	/// </summary>
	/// <returns>ActivityBase::EType::SELECT</returns>
	virtual EType getType(void) const override;

	/// <summary>
	/// Inverse this activity to switch undo/redo state.
	/// </summary>
	virtual void inverse(void) override;
};

