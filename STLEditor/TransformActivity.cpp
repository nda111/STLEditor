#include "TransformActivity.h"

class TransformActivity;

TransformActivity::TransformActivity(vtkCamera* const before, vtkCamera* const after)
{
	this->before = vtkCamera::New();
	this->before->DeepCopy(before);

	this->after = vtkCamera::New();
	this->after->DeepCopy(after);
}

TransformActivity::TransformActivity(const TransformActivity& ref)
{
	this->before = vtkCamera::New();
	this->before->DeepCopy(ref.before);

	this->after = vtkCamera::New();
	this->after->DeepCopy(ref.after);
}

TransformActivity::~TransformActivity(void)
{
	this->before->Delete();
	this->after->Delete();
}

vtkCamera* TransformActivity::getView(void)
{
	return this->before;
}

ActivityBase::EType TransformActivity::getType(void) const
{
	return EType::TRANSFORMATION;
}

void TransformActivity::inverse(void)
{
	vtkCamera* temp = this->before;
	this->before = this->after;
	this->after = temp;
}

