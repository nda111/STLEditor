#pragma once

#include <vtkInteractorStyleTrackballCamera.h>

class CustomInteractorStyletrackballCamera : public vtkInteractorStyleTrackballCamera
{
    virtual void OnLeftButtonDown()
    {
        vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }

    virtual void OnLeftButtonUp()
    {
        vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
    }

    virtual void OnRightButtonDown()
    {
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    virtual void OnRightButtonUp()
    {
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
    }

    virtual void OnMiddleButtonDown() { }
    virtual void OnMiddleButtonUp() { }

    virtual void OnMouseWheelForward() { }
    virtual void OnMouseWheelBackward() { }
};

