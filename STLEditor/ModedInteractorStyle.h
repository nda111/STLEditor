#pragma once

#include <vtkInteractorStyleTrackballCamera.h>

/// <summary>
/// A class that represents mode-customizable interaction style.
/// </summary>
class ModedInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public: // Inner class
	/// <summary>
	/// An enumeration class that represents the mode of interactor.
	/// </summary>
	enum class EMode
	{
		/// <summary>
		/// Transformation mode
		/// </summary>
		TRANSFORM,

		/// <summary>
		/// Selection mode
		/// </summary>
		SELECTION,
	};

private:
	/// <summary>
	/// The current interaction mode of the interactor.
	/// </summary>
	EMode mode = EMode::TRANSFORM;

public:
	/// <summary>
	/// Get the current interactor style.
	/// </summary>
	/// <returns>The current interactor style.</returns>
	EMode GetMode(void) const
	{
		return this->mode;
	}

	/// <summary>
	/// Set the current interactor style.
	/// </summary>
	/// <param name="mode">The new interactor style.</param>
	void SetMode(const EMode mode)
	{
		this->mode = mode;
	}

public:
	virtual void OnLeftButtonDown()
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}

	virtual void OnLeftButtonUp()
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}

	virtual void OnRightButtonDown()
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}

	virtual void OnRightButtonUp()
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}

	virtual void OnMiddleButtonDown() 
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			/* EMPTY */
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}

	virtual void OnMiddleButtonUp() 
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			/* EMPTY */
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}

	virtual void OnMouseWheelForward() 
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			/* EMPTY */
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}

	virtual void OnMouseWheelBackward() 
	{
		switch (this->mode)
		{
		case EMode::TRANSFORM:
			/* EMPTY */
			break;

		case EMode::SELECTION:
			/* EMPTY */
			break;
		}
	}
};

