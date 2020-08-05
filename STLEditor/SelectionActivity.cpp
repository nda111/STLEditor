#include "SelectionActivity.h"

class SelectionActivity;

SelectionActivity::SelectionActivity(vtkActor* const before, vtkActor* const after) : before(before), after(after)
{
	/* EMPTY */
}

vtkActor* SelectionActivity::getActor(void) const
{
	return before;
}

ActivityBase::EType SelectionActivity::getType() const
{
	return EType::SELECT;
}

void SelectionActivity::inverse()
{
	vtkActor* temp = this->before;
	this->before = this->after;
	this->after = temp;
}
