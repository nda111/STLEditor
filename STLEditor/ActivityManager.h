#pragma once

#include <stack>

#include "ActivityBase.h"
#include "ZoomActivity.h"

/// <summary>
/// A class that manages undo/redo of activities.
/// </summary>
class ActivityManager final
{
public: // Callbacks
	/// <summary>
	/// A callback of when the action with activity proceed.
	/// </summary>
	using EventHandler = void (*)(const ActivityBase*const);

private: // Fields
	/// <summary>
	/// A stack to store done activities to undo.
	/// </summary>
	std::stack<ActivityBase*>* doneActivities = nullptr;

	/// <summary>
	/// A stack to store undone activities to redo.
	/// </summary>
	std::stack<ActivityBase*>* rolledBackActivities = nullptr;

public: // Constructors & Destructors
	ActivityManager(void);
	~ActivityManager(void);

public: // Methods
	/// <summary>
	/// See if undoing is available.
	/// </summary>
	/// <returns>True is available, false if not.</returns>
	bool isUndoable(void);

	/// <summary>
	/// See if redoing is available.
	/// </summary>
	/// <returns>True is available, false if not.</returns>
	bool isRedoable(void);

	/// <summary>
	/// Do something initially for the activity.
	/// </summary>
	/// <param name="activity">The activity to execute with.</param>
	void push(ActivityBase* activity);

	/// <summary> 
	/// Undo last activity if exists.
	/// </summary>
	/// <returns>True if success, false, otherwise.</returns>
	ActivityBase* undo(void);

	/// <summary> 
	/// Redo last undone activity if exists. 
	/// </summary>
	/// <returns>True if success, false, otherwise.</returns>
	ActivityBase* redo(void);
};
