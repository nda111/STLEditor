#include "ActivityManager.h"

class ActivityManager;

ActivityManager::ActivityManager(void)
{
	this->doneActivities = new std::stack<ActivityBase*>;
	this->rolledBackActivities = new std::stack<ActivityBase*>;
}

ActivityManager::~ActivityManager(void)
{
	delete this->doneActivities;
	delete this->rolledBackActivities;
}

bool ActivityManager::isUndoable(void)
{
	return !this->doneActivities->empty();
}

bool ActivityManager::isRedoable(void)
{
	return !this->rolledBackActivities->empty();
}

void ActivityManager::push(ActivityBase* activity)
{
	while (this->rolledBackActivities->size())
	{
		delete this->rolledBackActivities->top();
		this->rolledBackActivities->pop();
	}
	
	if (this->doneActivities->size() != 0)
	{
		// Collapse multiple zoom activities
		if (activity->getType() == ActivityBase::EType::ZOOM &&
			this->doneActivities->top()->getType() == ActivityBase::EType::ZOOM)
		{
			ZoomActivity* const zActivity = (ZoomActivity*)activity;
			ZoomActivity* const prevActivity = (ZoomActivity*)this->doneActivities->top();

			// If the previous zoom doesn't against the current one.
			if (prevActivity->isZoomIn() == zActivity->isZoomIn()) 
			{
				zActivity->chain(prevActivity);
				delete this->doneActivities->top();
				this->doneActivities->pop();
			}
		}
	}

	this->doneActivities->push(activity);
}

ActivityBase* ActivityManager::undo(void)
{
	const bool result = isUndoable();

	if (result)
	{
		ActivityBase* activity = this->doneActivities->top();
		this->doneActivities->pop();

		this->rolledBackActivities->push(activity);

		return activity;
	}
	else
	{
		return nullptr;
	}
}

ActivityBase* ActivityManager::redo(void)
{
	const bool result = isRedoable();

	if (result)
	{
		ActivityBase* activity = this->rolledBackActivities->top();
		this->rolledBackActivities->pop();

		this->doneActivities->push(activity);

		return activity;
	}
	else
	{
		return nullptr;
	}
}