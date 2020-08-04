#pragma once

/// <summary>
/// A class that represents an activity: transformation, selection. 
/// </summary>
class ActivityBase
{
public: // Inner types
	/// <summary> 
	/// An enumeration that represents the type of the activity. 
	/// </summary>
	enum class EType
	{
		/// <summary> 
		/// For linear transformation activity.
		/// </summary>
		TRANSFORMATION,

		/// <summary>
		/// For zoom activity
		/// </summary>
		ZOOM,

		/// <summary>
		/// For a cube-selection activity in STL viewer.
		/// </summary>
		SELECT,
	};

public: // Pure virtual methods
	/// <summary>
	/// Get the type of the activity.
	/// </summary>
	/// <returns>The type of the activity.</returns>
	virtual EType getType() const = 0;

	/// <summary>
	/// Inverse this activity to switch undo/redo state.
	/// </summary>
	virtual void inverse() = 0;
};
