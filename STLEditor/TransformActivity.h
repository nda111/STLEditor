#pragma once

#include <vtkMatrix4x4.h>
#include <vtkNew.h>
#include <vtkCamera.h>

#include "ActivityBase.h"

/// <summary>
/// An activity that represents a 3D linear transformation.
/// </summary>
class TransformActivity : public ActivityBase
{
private: // Fields
	/// <summary>
	/// A camera that contains matrix for the transformation of undoing.
	/// Initially nullptr.
	/// </summary>
	vtkCamera* before;

	/// <summary>
	/// A camera that contains matrix for the transformation of redoing.
	/// Initially nullptr.
	/// </summary>
	vtkCamera* after;

public: // Constructors & Destructor
	TransformActivity(vtkCamera*const, vtkCamera*const);
	TransformActivity(const TransformActivity&);
	~TransformActivity(void);

public:
	/// <summary>
	/// Get the camera that contains linear transform matrix.
	/// </summary>
	/// <returns>The camera that contains the matrix.</returns>
	vtkCamera* getView(void);

public: // Inherited via ActivityBase
	/// <summary>
	/// Get the type of the activity.
	/// </summary>
	/// <returns>ActivityBase::EType::TRANSFORMATION</returns>
	EType getType(void) const override;

	/// <summary>
	/// Inverse this activity to switch undo/redo state.
	/// </summary>
	void inverse(void) override;
};

